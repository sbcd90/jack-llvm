#include "IR_Codegen_Visitor.h"

#include <memory>
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/Support/Host.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Transforms/Utils.h"

IRCodegenVisitor::IRCodegenVisitor() {
    this->context = std::make_unique<llvm::LLVMContext>();
    this->module = std::make_unique<llvm::Module>("Module", *context);
    this->builder = std::make_unique<llvm::IRBuilder<>>(*context);
    this->varEnv = std::map<std::string, llvm::AllocaInst*>{};
}

void IRCodegenVisitor::dumpLLVMIR() {
    module->print(llvm::outs(), nullptr);
}

std::string IRCodegenVisitor::dumpLLVMIRToString() {
    std::string outstr;
    llvm::raw_string_ostream oss(outstr);

    module->print(oss, nullptr);

    return oss.str();
}

void IRCodegenVisitor::configureTarget() {
    auto targetTriple = llvm::sys::getDefaultTargetTriple();
    module->setTargetTriple(targetTriple);
}

void IRCodegenVisitor::codegenProgram(const ProgramIR &program) {
    codegenExternFunctionDeclarations();
    codegenClasses(program.classDefinitions);
    codegenFunctionProtos(program.funcDefinitions);
    codegenFunctionDefinitions(program.funcDefinitions);
    codegenMainExpr(program.mainExpr);
    runOptimizingPasses(program.funcDefinitions);
}

void IRCodegenVisitor::codegenMainExpr(const std::vector<std::unique_ptr<ExprIR>> &mainExpr) {
    auto mainType = llvm::FunctionType::get(llvm::IntegerType::getInt32Ty(*context),
                                            std::vector<llvm::Type*>{}, false);
    auto main = llvm::Function::Create(mainType, llvm::Function::ExternalLinkage, "main", module.get());
    auto mainBasicBlock = llvm::BasicBlock::Create(*context, "entry", main);
    builder->SetInsertPoint(mainBasicBlock);
    varEnv.clear();

    for (auto &expr: mainExpr) {
        expr->codegen(*this);
    }

    llvm::APInt retVal{32, (uint32_t) 0, true};
    builder->CreateRet(llvm::ConstantInt::get(*context, retVal));
    llvm::verifyFunction(*main);
}

void IRCodegenVisitor::codegenExternFunctionDeclarations() {
    module->getOrInsertFunction("printf",
                                llvm::FunctionType::get(llvm::IntegerType::getInt32Ty(*context),
                                                        llvm::Type::getInt8Ty(*context)->getPointerTo(),
                                                        true));
}

void IRCodegenVisitor::codegenClasses(const std::vector<std::unique_ptr<ClassIR>> &classes) {
    for (auto &currClass: classes) {
        auto classType = llvm::StructType::getTypeByName(*context, llvm::StringRef(currClass->className));
        std::vector<llvm::Type*> bodyTypes{};
        for (auto &field: currClass->fields) {
            bodyTypes.push_back(field->codegen(*this));
        }
        classType->setBody(llvm::ArrayRef<llvm::Type*>(bodyTypes));
    }
}

llvm::FunctionType* IRCodegenVisitor::codegenFunctionType(const FunctionIR &function) {
    std::vector<llvm::Type*> paramTypes{};
    for (auto &param: function.params) {
        paramTypes.push_back(param->paramType->codegen(*this));
    }
    auto returnType = function.returnType->codegen(*this);
    return llvm::FunctionType::get(returnType, paramTypes, false);
}

void IRCodegenVisitor::codegenFunctionProtos(const std::vector<std::unique_ptr<FunctionIR>> &functions) {
    for (auto &function: functions) {
        auto functionType = codegenFunctionType(*function);
        llvm::Function::Create(functionType, llvm::Function::ExternalLinkage,
                               function->functionName, module.get());
    }
}

void IRCodegenVisitor::codegenFunctionDefinition(const FunctionIR &function) {
    auto llvmFunction = module->getFunction(llvm::StringRef(function.functionName));
    auto entryBasicBlock = llvm::BasicBlock::Create(*context, "entry", llvmFunction);
    builder->SetInsertPoint(entryBasicBlock);

    varEnv.clear();
    for (auto &param: llvmFunction->args()) {
        int paramNo = param.getArgNo();
        auto paramName = function.params[paramNo]->paramName;
        auto paramType = llvmFunction->getFunctionType()->getParamType(paramNo);
        varEnv[paramName] = builder->CreateAlloca(paramType, nullptr, llvm::Twine(paramName));
        builder->CreateStore(&param, varEnv[paramName]);
    }

    llvm::Value* returnValue;
    for (auto &expr: function.bodyExpr) {
        returnValue = expr->codegen(*this);
    }

    if (llvmFunction->getReturnType()->isVoidTy()) {
        builder->CreateRetVoid();
    } else {
        builder->CreateRet(returnValue);
    }
    llvm::verifyFunction(*llvmFunction);
}

void IRCodegenVisitor::codegenFunctionDefinitions(const std::vector<std::unique_ptr<FunctionIR>> &functions) {
    for (auto &function: functions) {
        codegenFunctionDefinition(*function);
    }
}

void IRCodegenVisitor::runOptimizingPasses(const std::vector<std::unique_ptr<FunctionIR>> &functions) {
    auto functionPassManager = std::make_unique<llvm::legacy::FunctionPassManager>(module.get());
    functionPassManager->add(llvm::createPromoteMemoryToRegisterPass());
    functionPassManager->add(llvm::createInstructionCombiningPass());
    functionPassManager->add(llvm::createReassociatePass());
    functionPassManager->add(llvm::createGVNPass());
    functionPassManager->add(llvm::createCFGSimplificationPass());

    functionPassManager->doInitialization();

    for (auto &function: functions) {
        auto llvmFun = module->getFunction(llvm::StringRef(function->functionName));
        functionPassManager->run(*llvmFun);
    }

    auto llvmMainFun = module->getFunction(llvm::StringRef("main"));
    functionPassManager->run(*llvmMainFun);
}

llvm::Value* IRCodegenVisitor::codegen(const ExprIntegerIR &exprIr) {
    return llvm::ConstantInt::getSigned((llvm::Type::getInt32Ty(*context)), exprIr.val);
}

llvm::Value* IRCodegenVisitor::codegen(const ExprBinOpIR &exprIr) {
    auto expr1Val = exprIr.expr1->codegen(*this);
    auto expr2Val = exprIr.expr2->codegen(*this);
    if (expr1Val == nullptr || expr2Val == nullptr) {
        throw new IRCodegenException(std::string{"Bin-op operand is null"});
    }
    switch (exprIr.op) {
        case BinOpPlus:
            return builder->CreateAdd(expr1Val, expr2Val, "add");
    }
}

llvm::Value* IRCodegenVisitor::codegen(const ExprPrintfIR &exprIr) {
    auto printf = module->getFunction("printf");
    std::vector<llvm::Value*> printfArgs;
    printfArgs.push_back(builder->CreateGlobalStringPtr(exprIr.formatStr));
    for (auto &arg: exprIr.arguments) {
        auto argVal = arg->codegen(*this);
        if (argVal == nullptr) {
            throw IRCodegenException{"Printf has null arg"};
        }
        printfArgs.push_back(argVal);
    }
    return builder->CreateCall(printf, printfArgs);
}

llvm::Type* IRCodegenVisitor::codegen(const TypeIntIR &typeIr) {
    return llvm::Type::getInt32Ty(*context);
}

llvm::Type* IRCodegenVisitor::codegen(const TypeClassIR &typeIr) {
    return llvm::StructType::getTypeByName(*context, llvm::StringRef{typeIr.className})->getPointerTo();
}

llvm::Type* IRCodegenVisitor::codegen(const TypeVoidIR &typeIr) {
    return llvm::Type::getVoidTy(*context);
}

llvm::Type* IRCodegenVisitor::codegen(const TypeBoolIR &typeIr) {
    return llvm::Type::getInt1Ty(*context);
}

llvm::Value* IRCodegenVisitor::codegen(const IdentifierVarIR &var) {
    auto val = varEnv[var.varName];
    if (val == nullptr) {
        throw new IRCodegenException("Var not found: " + var.varName);
    }
    return val;
}

llvm::Value* IRCodegenVisitor::codegen(const IdentifierObjectVarIR &objField) {
    auto objPtr = varEnv[objField.varName];
    if (objPtr == nullptr) {
        throw new IRCodegenException("Object not found: " + objField.varName);
    }
    return builder->CreateStructGEP(
            objPtr->getAllocatedType()->getPointerElementType(),
            builder->CreateLoad(objPtr->getAllocatedType()->getPointerElementType(), objPtr),
            objField.fieldIndex + NUM_RESERVED_FIELDS);
}

llvm::Value* IRCodegenVisitor::codegen(const ExprAssignIR &expr) {
    if (expr.assignedExpr == nullptr) {
        throw new IRCodegenException("Assigning a null expr to " + expr.identifier->varName);
    }
    auto assignedVal = expr.assignedExpr->codegen(*this);
    auto id = expr.identifier->codegen(*this);
    if (id == nullptr) {
        throw new IRCodegenException("Trying to assign to a null id: " + expr.identifier->varName);
    }
    builder->CreateStore(assignedVal, id);
    return assignedVal;
}

llvm::Value* IRCodegenVisitor::codegen(const ExprIdentifierIR &expr) {
    auto id = reinterpret_cast<llvm::AllocaInst*>(expr.identifier->codegen(*this));
    if (id == nullptr) {
        throw new IRCodegenException("Identifier not found: " + expr.identifier->varName);
    }
    auto type = id->getAllocatedType();
    auto idVal = builder->CreateLoad(type, id);
    if (idVal == nullptr) {
        throw new IRCodegenException("Identifier not loaded: " + expr.identifier->varName);
    }
    return idVal;
}

llvm::Value* IRCodegenVisitor::codegen(const ExprLetIR &expr) {
    if (expr.boundExpr == nullptr) {
        throw new IRCodegenException("Let - binding a null expr to " + expr.varName);
    }
    auto boundVal = expr.boundExpr->codegen(*this);
    auto parentFunction = builder->GetInsertBlock()->getParent();

    auto tmpBuilder = llvm::IRBuilder<>{&(parentFunction->getEntryBlock()),
                                        parentFunction->getEntryBlock().begin()};
    auto var = tmpBuilder.CreateAlloca(boundVal->getType(), nullptr, llvm::Twine(expr.varName));
    varEnv[expr.varName] = var;
    builder->CreateStore(boundVal, var);
    return boundVal;
}

llvm::Value* IRCodegenVisitor::codegen(const ExprFunctionCallIR &expr) {
    auto calleeFunction = module->getFunction(llvm::StringRef{expr.functionName});
    if (calleeFunction == nullptr) {
        throw new IRCodegenException("Function doesn't exist: " + expr.functionName);
    }
    auto calleeFunctionType = calleeFunction->getFunctionType();

    std::vector<llvm::Value*> argValues{};
    int size = expr.args.size();
    for (int i = 0; i < size; ++i) {
        auto argVal = expr.args[i]->codegen(*this);
        if (argVal == nullptr) {
            throw new IRCodegenException("Null Argument when calling function " + expr.functionName);
        }
        auto paramType = calleeFunctionType->getParamType(i);
        auto bitCastArgVal = builder->CreateBitCast(argVal, paramType);
        argValues.push_back(bitCastArgVal);
    }
    return builder->CreateCall(calleeFunction, argValues);
}
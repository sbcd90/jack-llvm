#include "IR_Codegen_Visitor.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Verifier.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Transforms/Utils.h"

IRCodegenVisitor::IRCodegenVisitor() {
    this->context = std::make_unique<llvm::LLVMContext>();
    this->module = std::make_unique<llvm::Module>("Module", *context);
}

void IRCodegenVisitor::codegenProgram(const ProgramIR &program) {
    codegenExternFunctionDeclarations();
    codegenClasses(program.classDefinitions);
    codegenFunctionProtos(program.funcDefinitions);
    codegenFunctionDefinitions(program.funcDefinitions);
    codegenMainExpr(program.mainExpr);
    runOptimizingPasses(program.funcDefinitions);
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
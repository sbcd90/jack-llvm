#pragma once
#ifndef IR_CODEGEN_VISITOR_H_
#define IR_CODEGEN_VISITOR_H_

#include <map>
#include <memory>
#include <vector>
#include "llvm/IR/IRBuilder.h"
#include "IRVisitor.h"
#include "FunctionIR.h"
#include "ProgramIR.h"

class IRCodegenVisitor: public IRVisitor {
    const int NUM_RESERVED_FIELDS = 4;
protected:
    std::unique_ptr<llvm::LLVMContext> context;
    std::unique_ptr<llvm::Module> module;
    std::unique_ptr<llvm::IRBuilder<>> builder;
    std::map<std::string, llvm::AllocaInst*> varEnv;
public:
    IRCodegenVisitor();

    void configureTarget();
    void runOptimizingPasses(const std::vector<std::unique_ptr<FunctionIR>> &functions);
    void dumpLLVMIR();
    std::string dumpLLVMIRToString();

    void codegenProgram(const ProgramIR &program);
    void codegenMainExpr(const std::vector<std::unique_ptr<ExprIR>> &mainExpr);

    void codegenExternFunctionDeclarations();

    void codegenClasses(const std::vector<std::unique_ptr<ClassIR>> &classes);

    llvm::FunctionType* codegenFunctionType(const FunctionIR &function);
    void codegenFunctionProtos(const std::vector<std::unique_ptr<FunctionIR>> &functions);
    void codegenFunctionDefinition(const FunctionIR &function);
    void codegenFunctionDefinitions(const std::vector<std::unique_ptr<FunctionIR>> &functions);
};

#endif
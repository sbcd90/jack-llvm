#include <iostream>
#include "IR_Codegen_Visitor.h"

int main() {
    auto codegen = std::make_unique<IRCodegenVisitor>();
    auto classDefinitions = std::vector<std::unique_ptr<ClassIR>>{};
    auto funcDefinitions = std::vector<std::unique_ptr<FunctionIR>>{};
    auto mainExpr = std::vector<std::unique_ptr<ExprIR>>{};

    auto exprIr1 = std::make_unique<ExprIntegerIR>(1);
    auto exprIr2 = std::make_unique<ExprIntegerIR>(4);
    auto exprIr4 = std::make_unique<ExprBinOpIR>(BinOpPlus, std::move(exprIr1), std::move(exprIr2));
    auto printfArgs = std::vector<std::unique_ptr<ExprIR>>{};
    printfArgs.push_back(std::move(exprIr4));

    auto exprIr = std::make_unique<ExprPrintfIR>("%d\n", std::move(printfArgs));
    mainExpr.push_back(std::move(exprIr));

    auto programIr = ProgramIR{std::move(classDefinitions), std::move(funcDefinitions), std::move(mainExpr)};
    codegen->codegenProgram(programIr);
    codegen->configureTarget();
    codegen->dumpLLVMIR();
    return 0;
}

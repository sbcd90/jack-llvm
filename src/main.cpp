#include <iostream>
#include "IR_Codegen_Visitor.h"

int main() {
    auto codegen = std::make_unique<IRCodegenVisitor>();
    auto classDefinitions = std::vector<std::unique_ptr<ClassIR>>{};
    auto funcDefinitions = std::vector<std::unique_ptr<FunctionIR>>{};
    auto mainExpr = std::vector<std::unique_ptr<ExprIR>>{};

    /**
     * only main expr
     */
    auto exprIr1 = std::make_unique<ExprIntegerIR>(1);
    auto letExpr1 = std::make_unique<ExprLetIR>("x", std::move(exprIr1));
    auto exprIr2 = std::make_unique<ExprIntegerIR>(4);
    auto letExpr2 = std::make_unique<ExprLetIR>("y", std::move(exprIr2));
    auto exprIr4 = std::make_unique<ExprBinOpIR>(BinOpPlus, std::move(letExpr1), std::move(letExpr2));
    auto letExpr4 = std::make_unique<ExprLetIR>("z", std::move(exprIr4));
    mainExpr.push_back(std::move(letExpr4));

    auto identifier = std::make_unique<IdentifierVarIR>("z");
    auto identifierExpr = std::make_unique<ExprIdentifierIR>(std::move(identifier));
    auto printfArgs = std::vector<std::unique_ptr<ExprIR>>{};
    printfArgs.push_back(std::move(identifierExpr));

    auto exprIr = std::make_unique<ExprPrintfIR>("%d\n", std::move(printfArgs));
    mainExpr.push_back(std::move(exprIr));

    /**
     * main expr calling a function
     */
     auto paramType1 = std::make_unique<TypeIntIR>();
     auto param1 = std::make_unique<ParameterIR>(std::move(paramType1), "x");
     auto paramType2 = std::make_unique<TypeIntIR>();
     auto param2 = std::make_unique<ParameterIR>(std::move(paramType2), "y");
     auto params = std::vector<std::unique_ptr<ParameterIR>>{};
     params.push_back(std::move(param1));
     params.push_back(std::move(param2));

     auto returnType = std::make_unique<TypeIntIR>();

     auto identifier1 = std::make_unique<IdentifierVarIR>("x");
     auto identifierExpr1 = std::make_unique<ExprIdentifierIR>(std::move(identifier1));
     auto identifier2 = std::make_unique<IdentifierVarIR>("y");
     auto identifierExpr2 = std::make_unique<ExprIdentifierIR>(std::move(identifier2));
    auto binOpExpr = std::make_unique<ExprBinOpIR>(BinOpPlus, std::move(identifierExpr1), std::move(identifierExpr2));
    auto bodyExpr = std::vector<std::unique_ptr<ExprIR>>{};
    bodyExpr.push_back(std::move(binOpExpr));

    auto function = std::make_unique<FunctionIR>("add", std::move(returnType), std::move(params), std::move(bodyExpr));
    funcDefinitions.push_back(std::move(function));

    auto param11 = std::make_unique<ExprIntegerIR>(1);
    auto param12 = std::make_unique<ExprIntegerIR>(7);
    auto paramsList = std::vector<std::unique_ptr<ExprIR>>{};
    paramsList.push_back(std::move(param11));
    paramsList.push_back(std::move(param12));
    auto functionCallee = std::make_unique<ExprFunctionCallIR>("add", std::move(paramsList));

    auto letExpr5 = std::make_unique<ExprLetIR>("a", std::move(functionCallee));
    mainExpr.push_back(std::move(letExpr5));

    auto identifier11 = std::make_unique<IdentifierVarIR>("a");
    auto identifierExpr11 = std::make_unique<ExprIdentifierIR>(std::move(identifier11));
    auto printfArgs1 = std::vector<std::unique_ptr<ExprIR>>{};
    printfArgs1.push_back(std::move(identifierExpr11));

    auto exprIr11 = std::make_unique<ExprPrintfIR>("%d\n", std::move(printfArgs1));
    mainExpr.push_back(std::move(exprIr11));

    auto programIr = ProgramIR{std::move(classDefinitions), std::move(funcDefinitions), std::move(mainExpr)};
    codegen->codegenProgram(programIr);
    codegen->configureTarget();
    codegen->dumpLLVMIR();
    return 0;
}

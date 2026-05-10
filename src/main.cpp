#include <iostream>
#include "IR_Codegen_Visitor.h"

int main() {
    auto codegen = std::make_unique<IRCodegenVisitor>();
    auto classDefinitions = std::vector<std::unique_ptr<ClassIR>>{};
    auto funcDefinitions = std::vector<std::unique_ptr<FunctionIR>>{};
    auto mainExpr = std::vector<std::unique_ptr<ExprIR>>{};

//    /**
//     * only main expr
//     */
//    auto exprIr1 = std::make_unique<ExprIntegerIR>(1);
//    auto letExpr1 = std::make_unique<ExprLetIR>("x", std::move(exprIr1));
//    auto exprIr2 = std::make_unique<ExprIntegerIR>(4);
//    auto letExpr2 = std::make_unique<ExprLetIR>("y", std::move(exprIr2));
//    auto exprIr4 = std::make_unique<ExprBinOpIR>(BinOpPlus, std::move(letExpr1), std::move(letExpr2));
//    auto letExpr4 = std::make_unique<ExprLetIR>("z", std::move(exprIr4));
//    mainExpr.push_back(std::move(letExpr4));
//
//    auto identifier = std::make_unique<IdentifierVarIR>("z");
//    auto identifierExpr = std::make_unique<ExprIdentifierIR>(std::move(identifier));
//    auto printfArgs = std::vector<std::unique_ptr<ExprIR>>{};
//    printfArgs.push_back(std::move(identifierExpr));
//
//    auto exprIr = std::make_unique<ExprPrintfIR>("%d\n", std::move(printfArgs));
//    mainExpr.push_back(std::move(exprIr));
//
//    /**
//     * main expr calling a function
//     */
//     auto paramType1 = std::make_unique<TypeIntIR>();
//     auto param1 = std::make_unique<ParameterIR>(std::move(paramType1), "x");
//     auto paramType2 = std::make_unique<TypeIntIR>();
//     auto param2 = std::make_unique<ParameterIR>(std::move(paramType2), "y");
//     auto params = std::vector<std::unique_ptr<ParameterIR>>{};
//     params.push_back(std::move(param1));
//     params.push_back(std::move(param2));
//
//     auto returnType = std::make_unique<TypeIntIR>();
//
//     auto identifier1 = std::make_unique<IdentifierVarIR>("x");
//     auto identifierExpr1 = std::make_unique<ExprIdentifierIR>(std::move(identifier1));
//     auto identifier2 = std::make_unique<IdentifierVarIR>("y");
//     auto identifierExpr2 = std::make_unique<ExprIdentifierIR>(std::move(identifier2));
//    auto binOpExpr = std::make_unique<ExprBinOpIR>(BinOpPlus, std::move(identifierExpr1), std::move(identifierExpr2));
//    auto bodyExpr = std::vector<std::unique_ptr<ExprIR>>{};
//    bodyExpr.push_back(std::move(binOpExpr));
//
//    auto function = std::make_unique<FunctionIR>("add", std::move(returnType), std::move(params), std::move(bodyExpr));
//    funcDefinitions.push_back(std::move(function));
//
//    auto param11 = std::make_unique<ExprIntegerIR>(1);
//    auto param12 = std::make_unique<ExprIntegerIR>(7);
//    auto paramsList = std::vector<std::unique_ptr<ExprIR>>{};
//    paramsList.push_back(std::move(param11));
//    paramsList.push_back(std::move(param12));
//    auto functionCallee = std::make_unique<ExprFunctionCallIR>("add", std::move(paramsList));
//
//    auto letExpr5 = std::make_unique<ExprLetIR>("a", std::move(functionCallee));
//    mainExpr.push_back(std::move(letExpr5));
//
//    auto identifier11 = std::make_unique<IdentifierVarIR>("a");
//    auto identifierExpr11 = std::make_unique<ExprIdentifierIR>(std::move(identifier11));
//    auto printfArgs1 = std::vector<std::unique_ptr<ExprIR>>{};
//    printfArgs1.push_back(std::move(identifierExpr11));
//
//    auto exprIr11 = std::make_unique<ExprPrintfIR>("%d\n", std::move(printfArgs1));
//    mainExpr.push_back(std::move(exprIr11));

    /**
     * main expr calling a class method
     */
     auto paramType20 = std::make_unique<TypeClassIR>("Class1");
     auto param20 = std::make_unique<ParameterIR>(std::move(paramType20), "this");
     auto paramType21 = std::make_unique<TypeIntIR>();
     auto param21 = std::make_unique<ParameterIR>(std::move(paramType21), "x");
     auto paramType22 = std::make_unique<TypeIntIR>();
     auto param22 = std::make_unique<ParameterIR>(std::move(paramType22), "y");
    auto params2 = std::vector<std:: unique_ptr<ParameterIR>>{};
    params2.push_back(std::move(param20));
    params2.push_back(std::move(param21));
    params2.push_back(std::move(param22));
    auto returnType2 = std::make_unique<TypeIntIR>();

    auto identifier21 = std::make_unique<IdentifierVarIR>("x");
    auto identifierExpr21 = std::make_unique<ExprIdentifierIR>(std::move(identifier21));
    auto identifier22 = std::make_unique<IdentifierVarIR>("y");
    auto identifierExpr22 = std::make_unique<ExprIdentifierIR>(std::move(identifier22));

    auto binOpExpr2 = std::make_unique<ExprBinOpIR>(BinOpPlus, std::move(identifierExpr21), std::move(identifierExpr22));
    auto bodyExpr2 = std::vector<std::unique_ptr<ExprIR>>{};
    bodyExpr2.push_back(std::move(binOpExpr2));

    auto function2 = std::make_unique<FunctionIR>("add2", std::move(returnType2), std::move(params2), std::move(bodyExpr2));
    funcDefinitions.push_back(std::move(function2));

    auto paramType40 = std::make_unique<TypeClassIR>("Class1");
    auto param40 = std::make_unique<ParameterIR>(std::move(paramType40), "this");
    auto paramType41 = std::make_unique<TypeIntIR>();
    auto param41 = std::make_unique<ParameterIR>(std::move(paramType41), "x");
    auto paramType42 = std::make_unique<TypeIntIR>();
    auto param42 = std::make_unique<ParameterIR>(std::move(paramType42), "y");
    auto params4 = std::vector<std:: unique_ptr<ParameterIR>>{};
    params4.push_back(std::move(param40));
    params4.push_back(std::move(param41));
    params4.push_back(std::move(param42));
    auto returnType4 = std::make_unique<TypeIntIR>();

    auto identifier41 = std::make_unique<IdentifierVarIR>("x");
    auto identifierExpr41 = std::make_unique<ExprIdentifierIR>(std::move(identifier41));
    auto identifier42 = std::make_unique<IdentifierVarIR>("y");
    auto identifierExpr42 = std::make_unique<ExprIdentifierIR>(std::move(identifier42));

    auto binOpExpr4 = std::make_unique<ExprBinOpIR>(BinOpPlus, std::move(identifierExpr41), std::move(identifierExpr42));
    auto bodyExpr4 = std::vector<std::unique_ptr<ExprIR>>{};
    bodyExpr4.push_back(std::move(binOpExpr4));

    auto function4 = std::make_unique<FunctionIR>("add4", std::move(returnType4), std::move(params4), std::move(bodyExpr4));
    funcDefinitions.push_back(std::move(function4));

    auto paramClassType1 = std::make_unique<TypeIntIR>();
    auto paramClassType2 = std::make_unique<TypeIntIR>();
    auto paramClassTypes = std::vector<std::unique_ptr<TypeIR>>{};
    paramClassTypes.push_back(std::move(paramClassType1));
    paramClassTypes.push_back(std::move(paramClassType2));

    auto classDefinition = std::make_unique<ClassIR>(std::string{"Class1"}, std::move(paramClassTypes), std::vector<std::string>{"add2"});
    classDefinitions.push_back(std::move(classDefinition));

    auto constructorExpr = std::make_unique<ExprConstructorIR>(std::string {"Class1"}, std::vector<std::unique_ptr<ConstructorArgIR>>{});
    auto letExpr2 = std::make_unique<ExprLetIR>("class1", std::move(constructorExpr));
    mainExpr.push_back(std::move(letExpr2));

    auto exprInt21 = std::make_unique<ExprIntegerIR>(1);
    auto exprInt22 = std::make_unique<ExprIntegerIR>(5);
    auto exprInts2 = std::vector<std::unique_ptr<ExprIR>>{};
    exprInts2.push_back(std::move(exprInt21));
    exprInts2.push_back(std::move(exprInt22));
    auto methodExpr = std::make_unique<ExprMethodAppIR>("class1", "add2", 0, std::move(exprInts2));
    auto letExpr4 = std::make_unique<ExprLetIR>("add2", std::move(methodExpr));
    mainExpr.push_back(std::move(letExpr4));

    auto identifier411 = std::make_unique<IdentifierVarIR>("add2");
    auto identifierExpr411 = std::make_unique<ExprIdentifierIR>(std::move(identifier411));
    auto printfArgs4 = std::vector<std::unique_ptr<ExprIR>>{};
    printfArgs4.push_back(std::move(identifierExpr411));
    auto exprIr41 = std::make_unique<ExprPrintfIR>("%d\n", std::move(printfArgs4));
    mainExpr.push_back(std::move(exprIr41));

    auto exprInt41 = std::make_unique<ExprIntegerIR>(2);
    auto exprInt42 = std::make_unique<ExprIntegerIR>(5);
    auto exprInts4 = std::vector<std::unique_ptr<ExprIR>>{};
    exprInts4.push_back(std::move(exprInt41));
    exprInts4.push_back(std::move(exprInt42));
    auto methodExpr2 = std::make_unique<ExprMethodAppIR>("class1", "add4", 0, std::move(exprInts4));
    auto letExpr41 = std::make_unique<ExprLetIR>("add4", std::move(methodExpr2));
    mainExpr.push_back(std::move(letExpr41));

    auto identifier412 = std::make_unique<IdentifierVarIR>("add4");
    auto identifierExpr412 = std::make_unique<ExprIdentifierIR>(std::move(identifier412));
    auto printfArgs41 = std::vector<std::unique_ptr<ExprIR>>{};
    printfArgs41.push_back(std::move(identifierExpr412));
    auto exprIr411 = std::make_unique<ExprPrintfIR>("%d\n", std::move(printfArgs41));
    mainExpr.push_back(std::move(exprIr411));

    auto programIr = ProgramIR{std::move(classDefinitions), std::move(funcDefinitions), std::move(mainExpr)};
    codegen->codegenProgram(programIr);
    codegen->configureTarget();
    codegen->dumpLLVMIR();
    return 0;
}

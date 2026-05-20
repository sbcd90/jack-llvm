#pragma once

#include "AST.h"
#include "IR_Codegen_Visitor.h"

ProgramIR convertToIR(const ClassAST &klass) {
    std::vector<std::unique_ptr<ClassIR>> classDefinitions;
    std::vector<std::unique_ptr<FunctionIR>> functionDefinitions;
    std::vector<std::unique_ptr<ExprIR>> mainExpr;

    for (const auto &fn: klass.functions) {
        if (fn.kind == FunctionKind::Method) {
            std::vector<std::unique_ptr<ParameterIR>> params;
            auto thisType = std::make_unique<TypeClassIR>(klass.name);
            params.push_back(std::make_unique<ParameterIR>(std::move(thisType), "this"));

            for (const auto &param: fn.parameters) {
                auto type = std::make_unique<TypeIntIR>();
                params.push_back(std::make_unique<ParameterIR>(std::move(type), param.name));
            }

            auto returnType = std::make_unique<TypeIntIR>();
            std::vector<std::unique_ptr<ExprIR>> body;

            for (const auto &letStmt: fn.lets) {
                auto bin = dynamic_cast<BinaryExprAST*>(letStmt.expr.get());
                auto lhsVar = dynamic_cast<VariableExprAST*>(bin->lhs.get());
                auto rhsVar = dynamic_cast<VariableExprAST*>(bin->rhs.get());

                auto lhsId = std::make_unique<IdentifierVarIR>(lhsVar->name);
                auto rhsId = std::make_unique<IdentifierVarIR>(rhsVar->name);

                auto lhsExpr = std::make_unique<ExprIdentifierIR>(std::move(lhsId));
                auto rhsExpr = std::make_unique<ExprIdentifierIR>(std::move(rhsId));

                auto addExpr = std::make_unique<ExprBinOpIR>(BinOpPlus,
                                                             std::move(lhsExpr), std::move(rhsExpr));
                body.push_back(std::make_unique<ExprLetIR>(letStmt.variable, std::move(addExpr)));
            }

            for (const auto &d: fn.dos) {
                auto var = dynamic_cast<VariableExprAST*>(d.call->args[0].get());
                auto ident = std::make_unique<IdentifierVarIR>(var->name);
                auto expr = std::make_unique<ExprIdentifierIR>(std::move(ident));

                std::vector<std::unique_ptr<ExprIR>> printfArgs;
                printfArgs.push_back(std::move(expr));

                body.push_back(std::make_unique<ExprPrintfIR>("%d\n", std::move(printfArgs)));
            }

            auto function = std::make_unique<FunctionIR>(fn.name, std::move(returnType), std::move(params),
                                                         std::move(body));
            functionDefinitions.push_back(std::move(function));
        } else {
            auto constructorExpr = std::make_unique<ExprConstructorIR>(std::string {"Main"}, std::vector<std::unique_ptr<ConstructorArgIR>>{});
            auto letExpr2 = std::make_unique<ExprLetIR>("main", std::move(constructorExpr));
            mainExpr.push_back(std::move(letExpr2));
            for (const auto &d: fn.dos) {
                if (d.call->methodName == "add") {
                    std::vector<std::unique_ptr<ExprIR>> args;

                    for (const auto &arg: d.call->args) {
                        auto intExpr = dynamic_cast<IntegerExprAST*>(arg.get());
                        args.push_back(std::make_unique<ExprIntegerIR>(intExpr->value));
                    }

                    auto methodCall = std::make_unique<ExprMethodAppIR>(d.call->caller, d.call->methodName,
                                                                        0, std::move(args));
                    mainExpr.push_back(std::move(methodCall));
                }
            }
        }
    }
    std::vector<std::unique_ptr<TypeIR>> fieldTypes;
    auto classDefinition = std::make_unique<ClassIR>(klass.name, std::move(fieldTypes),
                                                     std::vector<std::string>{"add"});
    classDefinitions.push_back(std::move(classDefinition));
    return ProgramIR{
            std::move(classDefinitions),
            std::move(functionDefinitions),
            std::move(mainExpr)
    };
}
#pragma once
#ifndef FUNCTION_IR_H_
#define FUNCTION_IR_H_

#include <memory>
#include <utility>
#include "TypeIR.h"

class IRVisitor;

struct ParameterIR {
    std::unique_ptr<TypeIR> paramType;
    std::string paramName;
    ParameterIR(std::unique_ptr<TypeIR> paramType, std::string paramName):
            paramType(std::move(paramType)), paramName(std::move(paramName)) {}
};

struct FunctionIR {
    std::string functionName;
    std::unique_ptr<TypeIR> returnType;
    std::vector<std::unique_ptr<ParameterIR>> params;
    std::vector<std::unique_ptr<ExprIR>> bodyExpr;

    FunctionIR(std::string functionName, std::unique_ptr<TypeIR> returnType,
               std::vector<std::unique_ptr<ParameterIR>> params, std::vector<std::unique_ptr<ExprIR>> bodyExpr):
               functionName(std::move(functionName)), returnType(std::move(returnType)), params(std::move(params)), bodyExpr(std::move(bodyExpr)) {}
};

#endif // FUNCTION_IR_H_
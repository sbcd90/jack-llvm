#pragma once
#ifndef AST_H_
#define AST_H_

#include <memory>
#include <string>
#include <vector>

enum class FunctionKind {
    Method,
    Function
};

struct ExprAST {
    virtual ~ExprAST() = default;
};

struct IntegerExprAST: ExprAST {
    int value;
    IntegerExprAST(int v): value(v) {}
};

struct VariableExprAST: ExprAST {
    std::string name;

    VariableExprAST(std::string n): name(std::move(n)) {}
};

struct BinaryExprAST: ExprAST {
    std::string op;
    std::unique_ptr<ExprAST> lhs;
    std::unique_ptr<ExprAST> rhs;

    BinaryExprAST(std::string op, std::unique_ptr<ExprAST> lhs, std::unique_ptr<ExprAST> rhs):
    op(std::move(op)), lhs(std::move(lhs)), rhs(std::move(rhs)) {}
};

struct MethodCallExprAST: ExprAST {
    std::string caller;
    std::string methodName;

    std::vector<std::unique_ptr<ExprAST>> args;

    MethodCallExprAST(std::string caller, std::string methodName, std::vector<std::unique_ptr<ExprAST>> args):
    caller(std::move(caller)), methodName(std::move(methodName)), args(std::move(args)) {}
};

struct LetStatementAST {
    std::string variable;
    std::unique_ptr<ExprAST> expr;

    LetStatementAST(std::string variable, std::unique_ptr<ExprAST> expr):
    variable(std::move(variable)), expr(std::move(expr)) {}
};

struct DoStatementAST {
    std::unique_ptr<MethodCallExprAST> call;
    DoStatementAST(std::unique_ptr<MethodCallExprAST> call): call(std::move(call)) {}
};

struct ReturnStatementAST {
    std::unique_ptr<ExprAST> expr;
};

struct VariableDeclAST {
    std::string typeOf;
    std::string name;

    VariableDeclAST(std::string typeOf, std::string name): typeOf(std::move(typeOf)),
    name(std::move(name)) {}
};

struct ParameterAST {
    std::string typeOf;
    std::string name;

    ParameterAST(std::string typeOf, std::string name): typeOf(std::move(typeOf)),
    name(std::move(name)) {}
};

struct FunctionAST {
    FunctionKind kind;

    std::string returnType;
    std::string name;

    std::vector<ParameterAST> parameters;
    std::vector<VariableDeclAST> locals;

    std::vector<LetStatementAST> lets;
    std::vector<DoStatementAST> dos;

    std::unique_ptr<ReturnStatementAST> ret;
};

struct ClassAST {
    std::string name;
    std::vector<FunctionAST> functions;
};

#endif // AST_H_
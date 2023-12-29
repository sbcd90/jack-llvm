#pragma once
#ifndef EXPR_IR_H_
#define EXPR_IR_H_

#include <utility>

#include "llvm/IR/Value.h"

class IRVisitor;

enum BinOp {
    BinOpPlus
};

struct IdentifierIR {
    std::string varName;
    virtual ~IdentifierIR() = default;
    virtual llvm::Value* codegen(IRVisitor &visitor) = 0;
};

struct IdentifierVarIR: public IdentifierIR {
    IdentifierVarIR(const std::string &name) {
        this->varName = name;
    }
    virtual llvm::Value* codegen(IRVisitor &visitor) override;
};

struct IdentifierObjectVarIR: public IdentifierIR {
    std::string objName;
    std::string objClassName;
    int fieldIndex;
    IdentifierObjectVarIR(std::string objName, std::string objClassName, int fieldIndex):
        objName(std::move(objName)), objClassName(std::move(objClassName)), fieldIndex(fieldIndex) {}
    virtual llvm::Value* codegen(IRVisitor &visitor) override;
};

struct ExprIR {
    virtual ~ExprIR() = default;
    virtual llvm::Value* codegen(IRVisitor &irVisitor)  = 0;
};

struct ExprIntegerIR: public ExprIR {
    int val;
    ExprIntegerIR(const int &i): val(i) {}
    virtual llvm::Value* codegen(IRVisitor &visitor) override;
};

struct ExprBinOpIR: public ExprIR {
    enum BinOp op;
    std::unique_ptr<ExprIR> expr1;
    std::unique_ptr<ExprIR> expr2;
    ExprBinOpIR(enum BinOp op, std::unique_ptr<ExprIR> expr1, std::unique_ptr<ExprIR> expr2):
            op(op), expr1(std::move(expr1)), expr2(std::move(expr2)) {}
    virtual llvm::Value* codegen(IRVisitor &visitor) override;
};

struct ExprPrintfIR: public ExprIR {
    std::string formatStr;
    std::vector<std::unique_ptr<ExprIR>> arguments;
    ExprPrintfIR(std::string formatStr, std::vector<std::unique_ptr<ExprIR>> arguments):
        formatStr(std::move(formatStr)), arguments(std::move(arguments)) {}
    virtual llvm::Value* codegen(IRVisitor &visitor) override;
};


struct ExprAssignIR: public ExprIR {
    std::unique_ptr<IdentifierIR> identifier;
    std::unique_ptr<ExprIR> assignedExpr;
    ExprAssignIR(std::unique_ptr<IdentifierIR> identifier, std::unique_ptr<ExprIR> assignedExpr):
        identifier(std::move(identifier)), assignedExpr(std::move(assignedExpr)) {}
    virtual llvm::Value* codegen(IRVisitor &irVisitor) override;
};

#endif
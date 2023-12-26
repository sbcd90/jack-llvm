#pragma once
#ifndef EXPR_IR_H_
#define EXPR_IR_H_

#include "llvm/IR/Value.h"

class IRVisitor;

enum BinOp {
    BinOpPlus
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

#endif
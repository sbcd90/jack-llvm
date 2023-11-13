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

#endif
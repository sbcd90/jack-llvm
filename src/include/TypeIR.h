#pragma once
#ifndef TYPE_IR_H_
#define TYPE_IR_H_

#include <utility>
#include "llvm/ir/Type.h"

class IRVisitor;

struct TypeIR {
    virtual ~TypeIR() = default;
    virtual llvm::Type* codegen(IRVisitor &irVisitor) = 0;
};

struct TypeIntIR: public TypeIR {
    virtual llvm::Type* codegen(IRVisitor &irVisitor) override;
};

struct TypeClassIR: public TypeIR {
    std::string className;
    TypeClassIR(std::string name): className(std::move(name)) {}
    virtual llvm::Type* codegen(IRVisitor &irVisitor) override;
};

struct TypeVoidIR: public TypeIR {
    virtual llvm::Type* codegen(IRVisitor &irVisitor) override;
};

struct TypeBoolIR: public TypeIR {
    virtual llvm::Type* codegen(IRVisitor &irVisitor) override;
};

#endif // TYPE_IR_H_
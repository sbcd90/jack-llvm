#pragma once
#ifndef IR_VISITOR_H_
#define IR_VISITOR_H_

#include "llvm/ir/Type.h"
#include "TypeIR.h"

class IRVisitor {
public:
    virtual llvm::Type* codegen(const TypeIntIR &typeIr) = 0;
    virtual llvm::Type* codegen(const TypeClassIR &typeIr) = 0;
    virtual llvm::Type* codegen(const TypeVoidIR &typeIr) = 0;
    virtual llvm::Type* codegen(const TypeBoolIR &typeIr) = 0;
};
#endif // IR_VISITOR_H_
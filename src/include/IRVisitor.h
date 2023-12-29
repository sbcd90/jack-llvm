#pragma once
#ifndef IR_VISITOR_H_
#define IR_VISITOR_H_

#include "llvm/IR/Type.h"
#include "TypeIR.h"
#include "ExprIR.h"

class IRVisitor {
public:
    virtual llvm::Type* codegen(const TypeIntIR &typeIr) = 0;
    virtual llvm::Type* codegen(const TypeClassIR &typeIr) = 0;
    virtual llvm::Type* codegen(const TypeVoidIR &typeIr) = 0;
    virtual llvm::Type* codegen(const TypeBoolIR &typeIr) = 0;
    virtual llvm::Value* codegen(const ExprIntegerIR &exprIr) = 0;
    virtual llvm::Value* codegen(const ExprBinOpIR &exprIr) = 0;
    virtual llvm::Value* codegen(const ExprPrintfIR &exprIr) = 0;
    virtual llvm::Value* codegen(const IdentifierVarIR &exprIr) = 0;
    virtual llvm::Value* codegen(const IdentifierObjectVarIR &exprIr) = 0;
    virtual llvm::Value* codegen(const ExprAssignIR &exprIr) = 0;
};
#endif // IR_VISITOR_H_
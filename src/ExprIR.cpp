#include "ExprIR.h"
#include "IRVisitor.h"

llvm::Value* ExprIntegerIR::codegen(IRVisitor &visitor) {
    return visitor.codegen(*this);
}

llvm::Value* ExprBinOpIR::codegen(IRVisitor &visitor) {
    return visitor.codegen(*this);
}

llvm::Value* ExprPrintfIR::codegen(IRVisitor &visitor) {
    return visitor.codegen(*this);
}

llvm::Value* IdentifierVarIR::codegen(IRVisitor &visitor) {
    return visitor.codegen(*this);
}

llvm::Value* IdentifierObjectVarIR::codegen(IRVisitor &visitor) {
    return visitor.codegen(*this);
}

llvm::Value* ExprAssignIR::codegen(IRVisitor &visitor) {
    return visitor.codegen(*this);
}

llvm::Value* ExprIdentifierIR::codegen(IRVisitor &visitor) {
    return visitor.codegen(*this);
}

llvm::Value* ExprLetIR::codegen(IRVisitor &visitor) {
    return visitor.codegen(*this);
}
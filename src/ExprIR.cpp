#include "ExprIR.h"
#include "IRVisitor.h"

llvm::Value* ExprIntegerIR::codegen(IRVisitor &visitor) {
    return visitor.codegen(*this);
}

llvm::Value* ExprBinOpIR::codegen(IRVisitor &visitor) {
    return visitor.codegen(*this);
}
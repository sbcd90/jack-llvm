#include "TypeIR.h"
#include "IRVisitor.h"

llvm::Type* TypeIntIR::codegen(IRVisitor &irVisitor) {
    return irVisitor.codegen(*this);
}

llvm::Type* TypeClassIR::codegen(IRVisitor &irVisitor) {
    return irVisitor.codegen(*this);
}

llvm::Type* TypeVoidIR::codegen(IRVisitor &irVisitor) {
    return irVisitor.codegen(*this);
}

llvm::Type* TypeBoolIR::codegen(IRVisitor &irVisitor) {
    return irVisitor.codegen(*this);
}
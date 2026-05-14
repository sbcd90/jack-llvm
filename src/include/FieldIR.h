#pragma once
#ifndef FIELD_IR_H_
#define FIELD_IR_H_

#include <memory>
#include "TypeIR.h"

struct FieldIR {
    std::unique_ptr<TypeIR> fieldType;
    std::string fieldName;

    FieldIR(std::unique_ptr<TypeIR> fieldType, const std::string &fieldName):
    fieldType(std::move(fieldType)), fieldName(fieldName) {}
};

#endif // FIELD_IR_H_
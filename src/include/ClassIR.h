#pragma once
#ifndef CLASS_IR_H_
#define CLASS_IR_H_

#include <string>
#include "TypeIR.h"

struct ClassIR {
    std::string className;
    std::vector<std::unique_ptr<TypeIR>> fields;
    std::vector<std::string> vTable;

    ClassIR(const std::string &className, std::vector<std::unique_ptr<TypeIR>> fields, std::vector<std::string> vTable):
        className(className), fields(std::move(fields)), vTable(std::move(vTable)) {}
};

#endif // CLASS_IR_H_
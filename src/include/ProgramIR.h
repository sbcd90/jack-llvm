#pragma once
#ifndef PROGRAM_IR_H_
#define PROGRAM_IR_H_

#include <utility>
#include <vector>
#include <memory>
#include "ClassIR.h"
#include "FunctionIR.h"

struct ProgramIR {
    std::vector<std::unique_ptr<ClassIR>> classDefinitions;
    std::vector<std::unique_ptr<FunctionIR>> funcDefinitions;
    std::vector<std::unique_ptr<ExprIR>> mainExpr;

    ProgramIR(std::vector<std::unique_ptr<ClassIR>> classDefinitions,
              std::vector<std::unique_ptr<FunctionIR>> functionDefinitions,
              std::vector<std::unique_ptr<ExprIR>> mainExpr):
              classDefinitions(std::move(classDefinitions)), funcDefinitions(std::move(functionDefinitions)), mainExpr(std::move(mainExpr)) {}
};

#endif // PROGRAM_IR_H_
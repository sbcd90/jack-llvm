#pragma once
#ifndef PROGRAM_IR_H_
#define PROGRAM_IR_H_

#include <vector>
#include <memory>
#include "ClassIR.h"
#include "FunctionIR.h"

struct ProgramIR {
    std::vector<std::unique_ptr<ClassIR>> classDefinitions;
    std::vector<std::unique_ptr<FunctionIR>> funcDefinitions;
    std::vector<std::unique_ptr<ExprIR>> mainExpr;

    ProgramIR(const std::vector<std::unique_ptr<ClassIR>> &classDefinitions,
              const std::vector<std::unique_ptr<FunctionIR>> &functionDefinitions,
              const std::vector<std::unique_ptr<ExprIR>> &mainExpr):
              classDefinitions(classDefinitions), funcDefinitions(functionDefinitions), mainExpr(mainExpr) {}
};

#endif // PROGRAM_IR_H_
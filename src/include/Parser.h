#pragma once
#ifndef PARSER_H_
#define PARSER_H_

#include "Tokenizer.h"
#include "AST.h"

class Parser {
private:
    std::vector<Token> tokens;
    size_t pos = 0;

    Token& current() {
        return tokens[pos];
    }

    bool match(const std::string &value) {
        if (current().value == value) {
            pos++;
            return true;
        }
        return false;
    }

    Token consume() {
        return tokens[pos++];
    }
    std::unique_ptr<ExprAST> parsePrimary();
    std::unique_ptr<ExprAST> parseExpression();
    std::unique_ptr<MethodCallExprAST> parseMethodCall();

public:
    Parser(std::vector<Token> tokens): tokens(std::move(tokens)) {}
    ClassAST parseClass();
    FunctionAST parseFunction();
};

#endif // PARSER_H_


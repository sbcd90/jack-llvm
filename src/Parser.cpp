#include "Parser.h"

std::unique_ptr<ExprAST> Parser::parsePrimary() {
    Token tok = this->consume();
    if (tok.type == TokenType::Integer) {
        return std::make_unique<IntegerExprAST>(std::stoi(tok.value));
    }
    return std::make_unique<VariableExprAST>(tok.value);
}

std::unique_ptr<ExprAST> Parser::parseExpression() {
    auto lhs = this->parsePrimary();
    if (match("+")) {
        auto rhs = this->parsePrimary();
        return std::make_unique<BinaryExprAST>(
                "+",
                std::move(lhs),
                std::move(rhs)
        );
    }
    return lhs;
}

std::unique_ptr<MethodCallExprAST> Parser::parseMethodCall() {
    auto caller = this->consume().value;
    auto methodName = this->consume().value;

    auto args = std::vector<std::unique_ptr<ExprAST>>{};
    match("(");
    if (!match(")")) {
        do {
            args.push_back(this->parseExpression());
        } while (match(","));
        match(")");
    }
    auto call = std::make_unique<MethodCallExprAST>(caller, methodName, std::move(args));
    return call;
}

ClassAST Parser::parseClass() {
    match("class");

    auto className = this->consume().value;
    match("{");

    ClassAST klass;
    klass.name = className;

    while (!match("}")) {
        klass.functions.push_back(this->parseFunction());
    }
    return klass;
}

FunctionAST Parser::parseFunction() {
    FunctionAST fn;

    if (match("method")) {
        fn.kind = FunctionKind::Method;
    } else {
        match("function");
        fn.kind = FunctionKind::Function;
    }

    fn.returnType = this->consume().value;
    fn.name = this->consume().value;

    match("(");
    if (!match(")")) {
        do {
            auto typeOf = this->consume().value;
            auto name = this->consume().value;
            fn.parameters.push_back(ParameterAST{typeOf, name});
        } while (match(","));
        match(")");
    }

    match("{");
    while (match("var")) {
        auto typeOf = this->consume().value;
        auto name = this->consume().value;
        auto localVar = VariableDeclAST{typeOf, name};
        fn.locals.push_back(localVar);
        match(";");
    }

    while (!match("}")) {
        if (match("let")) {
            auto variable = this->consume().value;
            match("=");
            auto expression = this->parseExpression();
            match(";");
            auto letStmt = LetStatementAST{variable, std::move(expression)};
            fn.lets.push_back(std::move(letStmt));
        } else if (match("do")) {
            auto call = this->parseMethodCall();
            match(";");
            auto doStmt = DoStatementAST(std::move(call));
            fn.dos.push_back(std::move(doStmt));
        } else if (match("return")) {
            auto ret = std::make_unique<ReturnStatementAST>();
            if (!match(";")) {
                ret.expr = this->parseExpression();
                match(";");
            }
            fn.ret = std::move(ret);
        }
    }
    return fn;
}
#pragma once

#include <string>
#include <vector>
#include <unordered_set>
#include <cctype>

enum class TokenType {
    Keyword,
    Identifier,
    Integer,
    Symbol,
    End
};

struct Token {
    TokenType type;
    std::string value;
};

class Tokenizer {
private:
    std::string source;
    size_t pos = 0;

    std::unordered_set<std::string> keywords = {
            "class",
            "method",
            "function",
            "int",
            "void",
            "var",
            "let",
            "do",
            "return"
    };

    char peek() const {
        if (pos >= source.size()) {
            return '\0';
        }
        return source[pos];
    }

    char advance() {
        return source[pos++];
    }

    void skipWhitespace() {
        while (std::isspace(peek())) {
            advance();
        }
    }

public:
    Tokenizer(const std::string &src): source(src) {}

    std::vector<Token> tokenize() {
        std::vector<Token> tokens;

        while (pos < source.size()) {
            skipWhitespace();

            char c = peek();
            if (std::isalpha(c) || c == '_') {
                std::string ident;
                while (std::isalnum(peek()) || peek() == '_') {
                    ident += advance();
                }

                if (keywords.count(ident)) {
                    tokens.push_back({TokenType::Keyword, ident});
                } else {
                    tokens.push_back({TokenType::Identifier, ident});
                }
            } else if (std::isdigit(c)) {
                std::string number;
                while (std::isdigit(peek())) {
                    number += advance();
                }
                tokens.push_back({
                    TokenType::Integer,
                    number
                });
            } else {
                std::string sym(1, advance());
                tokens.push_back({
                    TokenType::Symbol,
                    sym
                });
            }
        }
        tokens.push_back({
            TokenType::End,
            ""
        });
        return tokens;
    }
};
#pragma once
#include <string>

enum class TokenType {
    Number,
    Operator,    // ~ для унарного минуса
    LeftParen,
    RightParen,
    End
};

struct Token {
    TokenType type{ TokenType::End };
    std::string content;
    double numericValue{ 0.0 };

    Token() = default;

    static Token createNumber(double num, std::string original = {}) {
        Token result;
        result.type = TokenType::Number;
        result.numericValue = num;
        result.content = std::move(original);
        return result;
    }

    static Token createOperator(char op) {
        Token result;
        result.type = TokenType::Operator;
        result.content.assign(1, op);
        return result;
    }

    static Token createLeftParen() {
        Token result;
        result.type = TokenType::LeftParen;
        result.content = "(";
        return result;
    }

    static Token createRightParen() {
        Token result;
        result.type = TokenType::RightParen;
        result.content = ")";
        return result;
    }

    static Token createEnd() {
        return Token{};
    }

    char getOperatorChar() const { return content.empty() ? '\0' : content[0]; }
};
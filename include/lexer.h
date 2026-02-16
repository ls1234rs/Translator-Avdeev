#pragma once
#include <string>
#include <stdexcept>
#include <cctype>
#include <cstdlib>
#include "token.h"

// Лексический анализатор: разбивает строку на токены
class Lexer {
    std::string inputText;
    size_t currentPosition{ 0 };
    Token lastToken{ Token::createEnd() };  // Для определения унарного минуса
    
    static bool isWhitespace(char ch) {
        return ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r';
    }

    static bool isValidOperator(char ch) {
        return ch == '+' || ch == '-' || ch == '*' || ch == '/';
    }

    void advancePastWhitespace() {
        while (currentPosition < inputText.size() && isWhitespace(inputText[currentPosition])) {
            currentPosition++;
        }
    }

    // Унарный минус возможен после начала выражения, оператора или открывающей скобки
    bool canBeUnaryMinus() const {
        if (lastToken.type == TokenType::End) return true;
        if (lastToken.type == TokenType::Operator) return true;
        if (lastToken.type == TokenType::LeftParen) return true;
        return false;
    }

public:
    explicit Lexer(std::string input = {}) : inputText(std::move(input)) {}

    void setInput(std::string input) {
        inputText = std::move(input);
        currentPosition = 0;
        lastToken = Token::createEnd();
    }

    Token getNextToken() {
        // Пропускаем пробелы
        advancePastWhitespace();
        if (currentPosition >= inputText.size()) {
            lastToken = Token::createEnd();
            return Token::createEnd();
        }

        char currentChar = inputText[currentPosition];

        // Обрабатываем скобки
        if (currentChar == '(') {
            currentPosition++;
            lastToken = Token::createLeftParen();
            return lastToken;
        }
        if (currentChar == ')') {
            currentPosition++;
            lastToken = Token::createRightParen();
            return lastToken;
        }

        // Обрабатываем операторы
        if (isValidOperator(currentChar)) {
            currentPosition++;
            // Определяем унарный или бинарный минус
            if (currentChar == '-' && canBeUnaryMinus()) {
                lastToken = Token::createOperator('~'); 
                return lastToken;
            }
            lastToken = Token::createOperator(currentChar);
            return lastToken;
        }
        // Обрабатываем числа
        if (std::isdigit(static_cast<unsigned char>(currentChar)) || currentChar == '.') {
            const char* startPtr = inputText.c_str() + currentPosition;
            char* endPtr = nullptr;

            // Преобразуем строку в число
            double numValue = std::strtod(startPtr, &endPtr);
            if (endPtr == startPtr) {
                throw std::runtime_error("Lexer error: invalid number");
            }

            // Сохраняем исходное представление числа
            size_t charsRead = static_cast<size_t>(endPtr - startPtr);
            std::string originalText = inputText.substr(currentPosition, charsRead);
            currentPosition += charsRead;

            lastToken = Token::createNumber(numValue, originalText);
            return lastToken;
        }

        throw std::runtime_error(std::string("Lexer error: unexpected character '") + currentChar + "'");
    }
};
#pragma once
#include <string>
#include <vector>
#include <stdexcept>
#include "lexer.h"
#include "parser.h"
#include "token.h"
#include "stack.h"

// Вычисление выражений в RPN
class Eval {
public:
    double evaluateRpn(const std::vector<Token>& rpnTokens) const {
        ds::Stack<double> valueStack;

        for (const Token& token : rpnTokens) {
            // Если число - кладем в стек
            if (token.type == TokenType::Number) {
                valueStack.push(token.numericValue);
                continue;
            }

            if (token.type != TokenType::Operator) {
                throw std::runtime_error("Eval error: unexpected token in RPN");
            }

            char operatorChar = token.getOperatorChar();

            // Обрабатываем унарный минус
            if (operatorChar == '~') {
                if (valueStack.size() < 1) throw std::runtime_error("Eval error: unary minus needs 1 operand");
                double operand = valueStack.top(); 
                valueStack.pop();
                valueStack.push(-operand);
                continue;
            }

            // Обрабатываем бинарные операторы
            if (valueStack.size() < 2) throw std::runtime_error("Eval error: binary operator needs 2 operands");

            // Извлекаем операнды (сначала правый, потом левый)
            double rightOperand = valueStack.top(); 
            valueStack.pop();
            double leftOperand = valueStack.top(); 
            valueStack.pop();

            // Выполняем операцию и кладем результат в стек
            switch (operatorChar) {
            case '+': valueStack.push(leftOperand + rightOperand); break;
            case '-': valueStack.push(leftOperand - rightOperand); break;
            case '*': valueStack.push(leftOperand * rightOperand); break;
            case '/':
                if (rightOperand == 0.0) throw std::runtime_error("Eval error: division by zero");
                valueStack.push(leftOperand / rightOperand);
                break;
            default:
                throw std::runtime_error("Eval error: unknown operator");
            }
        }

        // В стеке должно остаться одно значение - результат
        if (valueStack.size() != 1) throw std::runtime_error("Eval error: invalid expression");
        return valueStack.top();
    }
};

class Translator {
    Lexer tokenizer;
    Parcer converter;
    Eval evaluator;

public:
    double calculate(const std::string& expression) {
        // Шаг 1: Лексический анализ - разбиваем строку на токены
        tokenizer.setInput(expression);
        // Шаг 2: Преобразуем в обратную польскую нотацию
        std::vector<Token> rpnSequence = converter.toRpn(tokenizer);
        // Шаг 3: Вычисляем значение RPN выражения
        return evaluator.evaluateRpn(rpnSequence);
    }
};
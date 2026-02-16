#pragma once
#include <vector>
#include <stdexcept>
#include "token.h"
#include "stack.h"
#include "lexer.h"

// Преобразование инфиксной нотации в RPN (алгоритм Shunting Yard)
class Parcer {
    static int getPrecedence(const Token& token) {
        if (token.type != TokenType::Operator) return -1;
        switch (token.getOperatorChar()) {
        case '+': case '-': return 1;
        case '*': case '/': return 2;
        case '~': return 3;
        default: return -1;
        }
    }

    static bool isRightAssociative(const Token& token) {
        return (token.type == TokenType::Operator && token.getOperatorChar() == '~');
    }

public:
    std::vector<Token> toRpn(Lexer& lex) {
        std::vector<Token> outputQueue;
        ds::Stack<Token> operatorStack;

        enum ParseState { ExpectingOperand, ExpectingOperator };
        ParseState currentState = ExpectingOperand;

        for (;;) {
            Token currentToken = lex.getNextToken();

            // Ожидаем операнд: число, скобку или унарный оператор
            if (currentState == ExpectingOperand) {
                if (currentToken.type == TokenType::Number) {
                    // Число сразу в выходную очередь
                    outputQueue.push_back(currentToken);
                    currentState = ExpectingOperator;
                    continue;
                }
                if (currentToken.type == TokenType::LeftParen) {
                    // Открывающая скобка в стек
                    operatorStack.push(currentToken);
                    currentState = ExpectingOperand;
                    continue;
                }
                if (currentToken.type == TokenType::Operator && currentToken.getOperatorChar() == '~') {
                    // Унарный минус в стек
                    operatorStack.push(currentToken);
                    currentState = ExpectingOperand;
                    continue;
                }
                throw std::runtime_error("Parser error: operand expected");
            }
            // Обрабатываем бинарный оператор
            if (currentToken.type == TokenType::Operator) {
                // Выталкиваем операторы с большим или равным приоритетом
                while (!operatorStack.empty() && operatorStack.top().type == TokenType::Operator) {
                    const Token& stackTop = operatorStack.top();

                    int stackPrec = getPrecedence(stackTop);
                    int currentPrec = getPrecedence(currentToken);

                    // Выталкиваем, если приоритет выше или равен (для левоассоциативных)
                    if (stackPrec > currentPrec || (stackPrec == currentPrec && !isRightAssociative(currentToken))) {
                        outputQueue.push_back(stackTop);
                        operatorStack.pop();
                    }
                    else {
                        break;
                    }
                }
                // Кладем текущий оператор в стек
                operatorStack.push(currentToken);
                currentState = ExpectingOperand;
                continue;
            }

            // Обрабатываем закрывающую скобку
            if (currentToken.type == TokenType::RightParen) {
                bool matchingLeftFound = false;
                // Выталкиваем операторы до открывающей скобки
                while (!operatorStack.empty()) {
                    Token stackTop = operatorStack.top();
                    operatorStack.pop();

                    if (stackTop.type == TokenType::LeftParen) {
                        matchingLeftFound = true;
                        break;
                    }
                    outputQueue.push_back(stackTop);
                }
                if (!matchingLeftFound) throw std::runtime_error("Parser error: ')' without matching '('");
                currentState = ExpectingOperator;
                continue;
            }

            // Конец выражения
            if (currentToken.type == TokenType::End) {
                if (currentState == ExpectingOperand) {
                    throw std::runtime_error("Parser error: incomplete expression");
                }
                // Выталкиваем все оставшиеся операторы
                while (!operatorStack.empty()) {
                    Token stackTop = operatorStack.top();
                    operatorStack.pop();
                    if (stackTop.type == TokenType::LeftParen) {
                        throw std::runtime_error("Parser error: '(' without matching ')'");
                    }
                    outputQueue.push_back(stackTop);
                }
                return outputQueue;
            }

            throw std::runtime_error("Parser error: operator expected");
        }
    }
};
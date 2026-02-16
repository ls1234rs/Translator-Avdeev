#include <iostream>
#include <string>
#include <stdexcept>

#include "translator.h"

int main() {
    Translator calculator;

    std::cout << "Enter expression per line. Empty line or EOF to exit.\n";

    std::string inputLine;
    // Основной цикл обработки выражений
    while (true) {
        std::cout << "> ";
        // Читаем строку из стандартного ввода
        if (!std::getline(std::cin, inputLine)) {  
            break;
        }
        if (inputLine.empty()) {             
            break;
        }

        // Вычисляем выражение и обрабатываем ошибки
        try {
            double computationResult = calculator.calculate(inputLine);
            std::cout << computationResult << "\n";
        }
        catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }

    return 0;
}
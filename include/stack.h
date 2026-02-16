#pragma once

#include <vector>
#include <stdexcept>

namespace ds {

    template <typename ElementType, template<typename...> class ContainerType = std::vector>
    class Stack {
    public:
        Stack() = default;
        Stack(const Stack& other) : elements(other.elements) {
        }
        Stack(Stack&& other) noexcept : elements(std::move(other.elements)) {
        }
        Stack& operator=(const Stack& other) {
            if (this != &other) {
                elements = other.elements;
            }
            return *this;
        }
        Stack& operator=(Stack&& other) noexcept {
            if (this != &other) {
                elements = std::move(other.elements);
            }
            return *this;
        }

        void push(const ElementType& item) {
            elements.push_back(item);
        }

        void push(ElementType&& item) {
            elements.push_back(std::move(item));
        }

        void pop() {
            if (elements.empty()) {
                throw std::out_of_range("Stack::pop on empty stack");
            }
            elements.pop_back();
        }

        ElementType& top() {
            if (elements.empty()) {
                throw std::out_of_range("Stack::top on empty stack");
            }
            return elements.back();
        }

        const ElementType& top() const {
            if (elements.empty()) {
                throw std::out_of_range("Stack::top on empty stack");
            }
            return elements.back();
        }

        bool empty() const noexcept {
            return elements.empty();
        }

        std::size_t size() const noexcept {
            return elements.size();
        }

    private:
        ContainerType<ElementType> elements;
    };

}


//
// Created by Nicholas on 14/12/2020.
//

#include <cstddef>
#include <stdexcept>
#include "Vector.h"

namespace wtl {

    /**
     * A first-in last-out (FILO) stack implemented using an dynamically sized array.
     * @tparam T
     */
    template<typename T>
    class Stack {
    private:

        // Underlying vector data structure
        wtl::Vector<T> m_Vector;

    public:

        /**
         * Default constructor
         */
        Stack() = default;

        /**
         * Copy constructor
         * @param other
         */
        Stack(const Stack<T>& other) : m_Vector(other.m_Vector) {
        }

        /**
         * Copy assignment
         * @param other
         * @return
         */
        Stack<T>& operator=(const Stack<T>& other) noexcept {
            if (this != &other) {
                m_Vector = other.m_Vector;
            }
        }

        Stack(Stack<T>&& other) = delete;

        Stack<T>& operator=(Stack<T>&& other) = delete;

        /**
         * Get the size of the stack
         * @return
         */
        [[nodiscard]] std::size_t getSize() const noexcept {
            return m_Vector.getSize();
        }

        /**
         * Check if the stack is empty.
         * @return True if the stack is empty, false if not.
         */
        [[nodiscard]] bool isEmpty() const noexcept {
            return m_Vector.isEmpty();
        }

        /**
         * Push element onto the top of the stack
         * @param value
         */
        void push(const T& value) noexcept {
            m_Vector.pushBack(value);
        }

        /**
         * Push element onto the top of the stack
         * @param value
         */
        void push(T&& value) noexcept {
            m_Vector.pushBack(std::move(value));
        }

        /**
         * Construct element on the top of the stack.
         * @tparam Ts
         * @param args
         * @return
         */
        template<typename ... Ts>
        T& emplace(Ts&& ... args) {
            return m_Vector.emplaceBack(std::forward<Ts>(args)...);
        }

        /**
         * Peek element on the top of the stack
         * @return
         */
        T& peek() {
            if (isEmpty()) {
                throw std::runtime_error("Stack is empty");
            }
            return m_Vector.at(m_Vector.getSize() - 1);
        }

        /**
         * Peek element on the top of the stack
         * @return
         */
        const T& peek() const {
            if (isEmpty()) {
                throw std::runtime_error("Stack is empty");
            }
            return m_Vector.at(m_Vector.getSize() - 1);
        }

        /**
         * Remove the element at the top of the stack.
         */
        void pop() {
            if (isEmpty()) {
                throw std::runtime_error("Stack is empty");
            }
            m_Vector.popBack();
        }

        /**
         * Remove all the elements at the top of the stack.
         * Size is reset to 0.
         */
        void clear() noexcept {
            m_Vector.clear();
        }

        /**
         * Check if an element exists in the stack.
         * @param element
         * @return True if it exists, false if it does not.
         */
        [[nodiscard]] bool exists(const T& element) const noexcept {
            return count(element) != 0;
        }

        /**
         * Check the number of times an element is present in the stack.
         * @param element
         * @return
         */
        [[nodiscard]] std::size_t count(const T& element) const noexcept {
            std::size_t count = 0;
            for (std::size_t i = 0; i < m_Vector.getSize(); i++) {
                if (m_Vector.at(i) == element) {
                    ++count;
                }
            }
            return count;
        }

        /**
         * Check if 2 stacks are equal.
         * @param other
         * @return
         */
        bool operator==(const Stack<T>& other) const noexcept {
            return m_Vector == other.m_Vector;
        }

        /**
         * Check if 2 stacks are not equal.
         * @param other
         * @return
         */
        bool operator!=(const Stack<T>& other) const noexcept {
            return m_Vector != other.m_Vector;
        }

        ~Stack() = default;

    };

}

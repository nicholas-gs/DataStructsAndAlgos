//
// Created by Nicholas on 14/12/2020.
//

#pragma once

#include "SinglyLinkedList.h"
#include <cstddef>
#include <stdexcept>

namespace wtl {

    /**
     * A first-in last-out (FILO) stack implemented using a Linked List. Supports constant time push and pop.
     * @tparam T
     */
    template<typename T>
    class Stack_LL {
    private:

        // Underlying singly linked list data structure
        wtl::SinglyLinkedList<T> m_SLL;

    public:

        /**
         * Default constructor
         */
        Stack_LL() = default;

        /**
        * Copy constructor
        * @param other
        */
        Stack_LL(const Stack_LL<T>& other) : m_SLL(other.m_SLL) {}

        /**
         * Copy assignment
         * @param other
         * @return
         */
        Stack_LL<T>& operator=(const Stack_LL<T>& other) noexcept {
            if (this != &other) {
                m_SLL = other.m_SLL;
            }
        }

        // Move constructor
        Stack_LL(Stack_LL<T>&& other) = delete;

        // Move assignment
        Stack_LL<T>& operator=(Stack_LL<T>&& other) = delete;

        /**
         * Get the size of the stack
         * @return
         */
        [[nodiscard]] std::size_t getSize() const noexcept {
            return m_SLL.getSize();
        }

        /**
         * Check if the stack is empty.
         * @return True if the stack is empty, false if not.
         */
        [[nodiscard]] bool isEmpty() const noexcept {
            return m_SLL.isEmpty();
        }

        /**
         * Push element onto the top of the stack
         * @param value
         */
        void push(const T& value) noexcept {
            m_SLL.appendFront(value);
        }

        /**
         * Push element onto the top of the stack
         * @param value
         */
        void push(T&& value) noexcept {
            m_SLL.appendFront(std::move(value));
        }

        /**
         * Construct element on the top of the stack.
         * @tparam Ts
         * @param args
         * @return
         */
        template<typename ... Ts>
        T& emplace(Ts&& ... args) {
            return m_SLL.emplaceFront(std::forward<Ts>(args)...);
        }

        /**
         * Peek element on the top of the stack
         * @return
         */
        T& peek() {
            if (isEmpty()) {
                throw std::runtime_error("Stack is empty");
            }
            return m_SLL.front();
        }

        /**
         * Peek element on the top of the stack
         * @return
         */
        const T& peek() const {
            if (isEmpty()) {
                throw std::runtime_error("Stack is empty");
            }
            return m_SLL.front();
        }

        /**
         * Remove the element at the top of the stack.
         */
        void pop() {
            if (isEmpty()) {
                throw std::runtime_error("Stack is empty");
            }
            m_SLL.detachFront();
        }

        /**
         * Remove all the elements at the top of the stack.
         * Size is reset to 0.
         */
        void clear() noexcept {
            m_SLL.clear();
        }

        /**
         * Check if an element exists in the stack.
         * @param element
         * @return True if it exists, false if it does not.
         */
        [[nodiscard]] bool exists(const T& element) const noexcept {
            return count(element) == 0 ? false : true;
        }

        /**
         * Check the number of times an element is present in the stack
         * @param element
         * @return
         */
        [[nodiscard]] std::size_t count(const T& element) const noexcept {
            std::size_t count = 0;
            m_SLL.foreach([&](const T& value) { if (value == element) { ++count; }});
            return count;
        }

        /**
         * Check if 2 stacks are equal.
         * @param other
         * @return
         */
        bool operator==(const Stack_LL<T>& other) const noexcept {
            return m_SLL == other.m_SLL;
        }

        /**
         * Check if 2 stacks are not equal.
         * @param other
         * @return
         */
        bool operator!=(const Stack_LL<T>& other) const noexcept {
            return m_SLL != other.m_SLL;
        }

        ~Stack_LL() = default;
    };

}

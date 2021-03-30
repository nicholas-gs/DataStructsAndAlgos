//
// Created by Nicholas on 7/12/2020.
//

#pragma once

#include <exception>
#include <cstddef>
#include <functional>
#include "DoublyLinkedList.h"

namespace wtl {

    /**
     * A first-in first-out (FIFO) queue implemented using a DoublyLinkedList.
     * @tparam T Type of the element stored in the queue
     */
    template<typename T>
    class Queue {
    private:

        /// Underlying doubly linked list data structure
        wtl::DoublyLinkedList<T> m_dll;

    public:

        /**
         * Constructor
         */
        Queue() = default;

        /**
         * Copy constructor
         * @param other
         */
        Queue(const Queue<T>& other) noexcept {
            m_dll = other.m_dll;
        }

        /**
         * Copy assignment
         * @param other
         * @return
         */
        Queue<T>& operator=(const Queue<T>& other) noexcept {
            if (this != &other) {
                clear();
                m_dll = other.m_dll;
            }
            return *this;
        }

        /**
         * Move constructor
         * @param other
         */
        Queue(Queue<T>&& other) noexcept {
            m_dll = std::move(other.m_dll);
        }

        /**
         * Move assignment
         * @param other
         * @return
         */
        Queue<T>& operator=(Queue<T>&& other) noexcept {
            if (this != &other) {
                clear();
                m_dll = std::move(other.m_dll);
            }
            return *this;
        }

        /**
         * Get the length of the queue
         * @return Length of the queue
         */
        [[nodiscard]] std::size_t getSize() const noexcept {
            return m_dll.getSize();
        }

        /**
         * Check if queue is empty
         * @return True if queue is empty, false if not
         */
        [[nodiscard]] bool isEmpty() const noexcept {
            return m_dll.isEmpty();
        }

        /**
         * Peek the front of the queue.
         * @return Reference to the front of the queue
         */
        [[nodiscard]] T& peek() noexcept {
            return m_dll.front();
        }

        /**
         * Peek the front of the queue
         * @return Const reference to the front of the queue
         */
        [[nodiscard]] const T& peek() const noexcept {
            return m_dll.front();
        }

        /**
         * Check if an element exists in the queue
         * @param element
         * @return True if element is in the queue, false if not
         */
        [[nodiscard]] bool contains(const T& element) const noexcept {
            return m_dll.contains(element);
        }

        /**
         * Check the number of times elements are present in the queue.
         * @param element
         * @return
         */
        [[nodiscard]] std::size_t count(const T& element) const noexcept {
            return m_dll.count(element);
        }

        /**
         * Performs an user defined operation on each element in the array.
         * @param func Lambda that returns void and takes in a T reference.
         */
        void foreach(const std::function<void(T&)>& func) noexcept {
            m_dll.foreach(func);
        }

        /**
         * Performs an user defined operation on each element in the array.
         * @param func Lambda that returns void and takes in a const T reference
         */
        void foreach(const std::function<void(const T&)>& func) const noexcept {
            m_dll.foreach(func);
        }

        /**
         * Check if two queues are equal.
         * @param other
         * @return
         */
        bool equals(const Queue<T>& other) const noexcept {
            return (*this) == other;
        }

        /**
         * Check if two queues are not equal
         * @param other
         * @return
         */
        bool notEquals(const Queue<T>& other) const noexcept {
            return !((*this) == other);
        }

        /**
         * Check if two queues are equal.
         * @param other
         * @return
         */
        bool operator==(const Queue<T>& other) const noexcept {
            return m_dll == other.m_dll;
        }

        /**
         * Check if two queues are not equal
         * @param other
         * @return
         */
        bool operator!=(const Queue<T>& other) const noexcept {
            return !(m_dll == other.m_dll);
        }

        /**
         * Enqueue an element to the back of the queue
         * @param element
         */
        void enqueue(const T& element) noexcept {
            m_dll.appendBack(element);
        }

        /**
         * Enqueue an element to the back of the queue
         * @param element
         */
        void enqueue(T&& element) noexcept {
            m_dll.appendBack(std::move(element));
        }

        /**
         * Enqueue an element to the back of the queue in-place.
         * @tparam Ts
         * @param args
         * @return
         */
        template<typename ... Ts>
        T& emplace(Ts&& ... args) noexcept {
            return m_dll.emplaceBack(std::forward<Ts>(args)...);
        }

        /**
         * Remove the element at the front of the queue.
         * @throw std::runtime_error if queue is empty
         */
        void dequeue() {
            if (isEmpty()) {
                throw std::runtime_error("Cannot dequeue empty queue");
            }
            m_dll.detachFront();
        }

        /**
         * Clear the queue. Size is reset to 0.
         */
        void clear() noexcept {
            m_dll.clear();
        }

        /**
         * Destructor
         */
        ~Queue() = default;

    };

}
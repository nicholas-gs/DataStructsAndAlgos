//
// Created by Nicholas on 8/12/2020.
//

#pragma once

#include "Heap.h"
#include "impl/DataStructuresImpl.h"
#include <stdexcept>
#include <cstddef>

namespace wtl {

    /**
     * Priority queue implemented using a heap.
     * By default, the comparison is done using operator<, hence the largest value has the highest priority.
     * A custom binary predicate can be provided for custom ordering.
     * @tparam T
     * @tparam Comparator
     */
    template<typename T, typename Comparator = impl::ds::StandardComparator<T>>
    class PriorityQueue {
    private:

        using Heap = wtl::Heap<T, Comparator>;

        /// Underlying heap data structure
        Heap m_Heap;

    public:

        /**
         * Constructor
         * @param order
         */
        PriorityQueue() = default;

        /**
         * Constructor
         * Construct heap using the elements in a stl library container.
         * @tparam Container
         * @param container
         */
        template<typename Container>
        explicit PriorityQueue(const Container& container) : m_Heap(container) {}

        /**
         * Copy constructor
         * @param other
         */
        PriorityQueue(const PriorityQueue<T, Comparator>& other) noexcept: m_Heap(other.m_Heap) {}

        /**
         * Copy assignment
         * @param other
         * @return
         */
        PriorityQueue<T, Comparator>& operator=(const PriorityQueue<T, Comparator>& other) noexcept {
            if (this != &other) {
                m_Heap.clear();
                m_Heap = other.m_Heap;
            }
            return *this;
        }

        /**
         * Move constructor
         * @param other
         */
        PriorityQueue(PriorityQueue<T, Comparator>&& other) noexcept: m_Heap(std::move(other.m_Heap)) {}

        /**
         * Move assignment
         * @param other
         * @return
         */
        PriorityQueue<T, Comparator>& operator=(PriorityQueue<T, Comparator>&& other) noexcept {
            if (this != &other) {
                m_Heap.clear();
                m_Heap = std::move(other.m_Heap);
            }
            return *this;
        }

        /**
         * Check if priority queue is empty
         * @return True if priority queue is empty, false if not
         */
        [[nodiscard]] bool isEmpty() const noexcept {
            return m_Heap.isEmpty();
        }

        /**
         * Get the number of elements in the priority queue
         * @return
         */
        [[nodiscard]] std::size_t getSize() const noexcept {
            return m_Heap.getSize();
        }

        /**
         * Get the element at the root of the heap
         * @return reference to the element at the root of the heap
         */
        T& peek() {
            if (isEmpty()) {
                throw std::runtime_error("Priority Queue is empty");
            }
            return m_Heap.peek();
        }

        /**
         * Get the element at the root of the heap
         * @return const reference to the element at the root of the heap
         */
        const T& peek() const {
            if (isEmpty()) {
                throw std::runtime_error("Priority Queue is empty");
            }
            return m_Heap.peek();
        }

        /**
         * Check if 2 priority queues are equal.
         * @param other
         * @return
         */
        bool equals(const PriorityQueue<T>& other) const noexcept {
            return (*this) == other;
        }

        /**
         * Check if 2 priority queues are not equal
         * @param other
         * @return
         */
        bool notEquals(const PriorityQueue<T>& other) const noexcept {
            return (*this) != other;
        }

        /**
         * Operator== overload
         * @param other
         * @return
         */
        bool operator==(const PriorityQueue<T>& other) const noexcept {
            return m_Heap == (other.m_Heap);
        }

        /**
         * Operator!= overload
         * @param other
         * @return
         */
        bool operator!=(const PriorityQueue<T>& other) const noexcept {
            return !(m_Heap == other.m_Heap);
        }

        /**
         * Remove the root element of the heap.
         */
        void poll() {
            if (isEmpty()) {
                throw std::runtime_error("Priority Queue is empty");
            }
            m_Heap.poll();
        }

        /**
         * Insert element into priority queue
         * @param element
         */
        void insert(const T& element) noexcept {
            m_Heap.insert(element);
        }

        /**
         * Insert element into priority queue
         * @param element
         */
        void insert(T&& element) noexcept {
            m_Heap.insert(std::move(element));
        }

        /**
         * Construct element in-place in priority queue
         * @tparam Ts
         * @param args
         */
        template<typename ... Ts>
        void emplace(Ts&& ... args) {
            m_Heap.emplace(std::forward<Ts>(args)...);
        }

        /**
         * Destructor
         */
        ~PriorityQueue() = default;

    };

}

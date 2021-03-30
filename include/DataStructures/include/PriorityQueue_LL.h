//
// Created by Nicholas on 14/12/2020.
//

#pragma once

#include "SinglyLinkedList.h"
#include "impl/DataStructuresImpl.h"
#include <cstddef>

namespace wtl {

    /**
     * Priority queue implemented using a SinglyLinkedList.
     * By default, the comparison is done using operator<.
     * A custom binary predicate can be provided for custom ordering.
     * @tparam T
     * @tparam Comparator
     */
    template<typename T, typename Comparator = impl::ds::StandardComparator<T>>
    class PriorityQueue_LL {
    private:

        /// Underlying singly linked list
        /// The front of the linked list is the element with the highest priority.
        wtl::SinglyLinkedList<T> m_SLL;

        /// Comparator object used for ordering
        impl::ds::ComparatorHelper<T, Comparator> m_Comparator;

    public:

        /**
         * Default constructor
         */
        PriorityQueue_LL() = default;

        /**
         * Check if priority queue is empty
         * @return True if priority queue is empty, false if not
         */
        [[nodiscard]] bool isEmpty() const noexcept {
            return m_SLL.isEmpty();
        }

        /**
         * Get the number of elements in the priority queue
         * @return
         */
        [[nodiscard]] std::size_t getSize() const noexcept {
            return m_SLL.getSize();
        }

        /**
         * Get the element at the root of the heap
         * @return reference to the element at the root of the heap
         */
        T& peek() {
            if (isEmpty()) {
                throw std::runtime_error("Priority Queue is empty");
            }
            return m_SLL.front();
        }

        /**
         * Get the element at the root of the heap
         * @return const reference to the element at the root of the heap
         */
        const T& peek() const {
            if (isEmpty()) {
                throw std::runtime_error("Priority Queue is empty");
            }
            return m_SLL.front();
        }

        /**
         * Check if 2 priority queues are equal.
         * @param other
         * @return
         */
        bool equals(const PriorityQueue_LL<T, Comparator>& other) const noexcept {
            return m_SLL == other.m_SLL;
        }

        /**
         * Check if 2 priority queues are not equal
         * @param other
         * @return
         */
        bool notEquals(const PriorityQueue_LL<T, Comparator>& other) const noexcept {
            return m_SLL != other.m_SLL;
        }

        /**
         * Operator== overload
         * @param other
         * @return
         */
        bool operator==(const PriorityQueue_LL<T, Comparator>& other) const noexcept {
            return m_SLL == other.m_SLL;
        }

        /**
         * Operator!= overload
         * @param other
         * @return
         */
        bool operator!=(const PriorityQueue_LL<T, Comparator>& other) const noexcept {
            return m_SLL != other.m_SLL;
        }

        /**
         * Remove the element at the front of the priority queue.
         */
        void poll() {
            if (isEmpty()) {
                throw std::runtime_error("Priority Queue is empty");
            }
            m_SLL.detachFront();
        }

        /**
         * Insert element into priority queue
         * @param element
         */
        void insert(const T& element) noexcept {
            std::size_t i = 0;
            m_SLL.foreach([&](const T& value) {
                // element > value
                if (m_Comparator(value, element)) { return; }
                else { ++i; }
            });
            m_SLL.insertAt(i, element);
        }

        /**
         * Insert element into priority queue
         * @param element
         */
        void insert(T&& element) noexcept {
            std::size_t i = 0;
            m_SLL.foreach([&](const T& value) {
                // element > value
                if (m_Comparator(value, element)) { return; }
                else { ++i; }
            });
            m_SLL.insertAt(i, std::move(element));
        }

        /**
         * Destructor
         */
        ~PriorityQueue_LL() = default;

    };

}

//
// Created by Nicholas on 7/12/2020.
//

#pragma once

#include "impl/DataStructuresImpl.h"
#include <cstddef>
#include <utility>
#include <stdexcept>
#include <vector>
#include <functional>

namespace wtl {

    /**
     * Complete binary heap implemented using a dynamic array as the underlying data structure.
     * By default, the comparison is done using operator<.
     * A custom binary predicate can be provided for custom ordering.
     * @tparam T
     * @tparam Comparator
     */
    template<typename T, typename Comparator = impl::ds::StandardComparator<T>>
    class Heap {
    private:

        /// Comparator object used for ordering
        impl::ds::ComparatorHelper<T, Comparator> comparator;

        /// Underlying vector data structure
        std::vector<T> m_Arr;

        void constructHeap() noexcept {
            heapify(1);
        }

        void heapify(std::size_t root) noexcept {
            std::size_t size = m_Arr.size();
            if (root * 2 > size) {
                return;
            }
            std::size_t leftNode = 2 * root;
            std::size_t rightNode = leftNode + 1;
            heapify(leftNode);
            bool hasRightChild = rightNode <= size;
            if (hasRightChild) {
                heapify(rightNode);
            }
            fixHeap(root);
        }

        void fixHeap(std::size_t root) noexcept {
            std::size_t size = m_Arr.size();
            std::vector<T>& vector = m_Arr;
            if (root * 2 > size) {
                return;
            }
            // Get the index of the left and right child nodes
            std::size_t leftNode = 2 * root;
            std::size_t rightNode = leftNode + 1;
            bool hasRightChild = rightNode <= size;
            // Max Heap
            std::size_t largerChildIndex = -1;

            if (hasRightChild) {
                // vector[leftNode - 1] >= vector[rightNode - 1]
                largerChildIndex = comparator.dualCompare(vector[rightNode - 1], vector[leftNode - 1]) ? leftNode : rightNode;
            } else {
                largerChildIndex = leftNode;
            }
            //vector[root - 1] < vector[largerChildIndex - 1]
            if (comparator(vector[root - 1], vector[largerChildIndex - 1])) {
                std::swap(vector[root - 1], vector[largerChildIndex - 1]);
                fixHeap(largerChildIndex);
            }

        }

        void bubbleUp(std::size_t node) noexcept {
            std::vector<T>& vector = m_Arr;
            std::size_t parentNode = node / 2;
            while (parentNode >= 1) {
                // vector[node - 1] > vector[parentNode - 1]
                if (comparator(vector[parentNode - 1], vector[node - 1])) {
                    std::swap(vector[parentNode - 1], vector[node - 1]);
                } else {
                    break;
                }
                node = parentNode;
                parentNode = node / 2;
            }
        }

    public:

        /**
         * Constructor
         */
        Heap() = default;

        /**
         * Constructor
         * Construct heap using the elements in a stl library container.
         * @tparam Container
         * @param container
         */
        template<typename Container>
        Heap(const Container& container) : m_Arr(container.begin(), container.end()) {
            constructHeap();
        }

        /**
         * Copy constructor
         * @param other
         */
        Heap(const Heap<T, Comparator>& other) noexcept: m_Arr(other.m_Arr.begin(), other.m_Arr.end()) {}

        /**
         * Copy assignment
         * @param other
         * @return
         */
        Heap<T, Comparator>& operator=(const Heap<T, Comparator>& other) noexcept {
            if (this != &other) {
                m_Arr.clear();
                m_Arr = other.m_Arr;
            }
            return *this;
        }

        /**
         * Move constructor
         * @param other
         */
        Heap(Heap<T, Comparator>&& other) noexcept: m_Arr(std::move(other.m_Arr)) {}

        /**
         * Move assignment
         * @param other
         * @return
         */
        Heap<T, Comparator>& operator=(Heap<T, Comparator>&& other) noexcept {
            if (this != &other) {
                m_Arr.clear();
                m_Arr = std::move(other.m_Arr);
            }
            return *this;
        }

        /**
         * Get the number of elements in the heap
         * @return
         */
        [[nodiscard]] std::size_t getSize() const noexcept {
            return m_Arr.size();
        }

        /**
         * Get the current capacity of the heap
         * @return
         */
        [[nodiscard]] std::size_t getCapacity() const noexcept {
            return m_Arr.capacity();
        }

        /**
         * Check if the heap is empty
         * @return
         */
        [[nodiscard]] bool isEmpty() const noexcept {
            return m_Arr.empty();
        }

        /**
         * Check if the two heaps are equal. They are equal if they have the same number of elements and same order
         * of elements.
         * @param other
         * @return
         */
        bool equals(const Heap<T, Comparator>& other) const noexcept {
            return (*this) == other;
        }

        /**
         * Check if the two heaps are not equal. They are not equal if they have different number of elements
         * or different order of elements.
         * @param other
         * @return
         */
        bool notEquals(const Heap<T, Comparator>& other) const noexcept {
            return (*this) != other;
        }

        bool operator==(const Heap<T, Comparator>& other) const noexcept {
            return m_Arr == other.m_Arr;
        }

        bool operator!=(const Heap<T, Comparator>& other) const noexcept {
            return !(m_Arr == other.m_Arr);
        }

        /**
         * Get the element at the root of the heap
         * @return Reference to the root element
         */
        T& peek() {
            if (isEmpty()) {
                throw std::runtime_error("Heap is empty");
            }
            return m_Arr[0];
        }

        /**
         * Get the element at the root of the heap
         * @return Const reference to the root element
         */
        const T& peek() const {
            if (isEmpty()) {
                throw std::runtime_error("Heap is empty");
            }
            return m_Arr[0];
        }

        /**
         * Iterate through all the elements in the heap
         * @param func Function that takes in a T& as parameter
         */
        void foreach(const std::function<void(T&)>& func) {
            for (T& element : m_Arr) {
                func(element);
            }
        }

        /**
         * Iterate through all the elements in the heap
         * @param func Function that takes in a const T& as parameter
         */
        void foreach(const std::function<void(const T&)>& func) const {
            for (const T& element : m_Arr) {
                func(element);
            }
        }

        /**
         * Remove the root element of the heap. Heap property is maintained.
         */
        void poll() {
            if (isEmpty()) {
                throw std::runtime_error("Heap is empty");
            }
            std::swap(m_Arr.front(), m_Arr.back());
            m_Arr.pop_back();
            fixHeap(1);
        }

        /**
         * Insert an element into the heap.
         * @param element
         */
        void insert(const T& element) noexcept {
            m_Arr.push_back(element);
            bubbleUp(getSize());
        }

        /**
         * Insert an element into the heap.
         * @param element
         */
        void insert(T&& element) noexcept {
            m_Arr.push_back(std::move(element));
            bubbleUp(getSize());
        }

        /**
         * Construct an element in-place in the heap.
         * @tparam Ts
         * @param args
         */
        template<typename ... Ts>
        void emplace(Ts ... args) noexcept {
            m_Arr.emplace_back(std::forward<Ts>(args)...);
            bubbleUp(getSize());
        }

        /**
         * Remove all elements in the heap.
         */
        void clear() noexcept {
            m_Arr.clear();
        }

        /**
         * Destructor
         */
        ~Heap() = default;

    };
}
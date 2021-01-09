//
// Created by Nicholas on 30 Dec 2020.
//

#pragma once

#include <stdexcept>
#include <cstddef>
#include <vector>
#include <utility>
#include <algorithm>
#include <functional>

#include "impl/DataStructuresImpl.h"
#include "BiMap.h"

namespace wtl {

    /**
     * By default, the IPQ uses the operator< to sort elements of type V. A custom binary predicate of type Comparator
     * can be used to sort the elements of type V.
     * Keys of type K acts as a unique identifier for objects of type V. Therefore, the keys must be unique in the IPQ.
     * @tparam K Type of key that acts as a unique identifier for values
     * @tparam V Type of value to be sorted in the priority queue
     * @tparam Comparator Type of binary predicate used to compare objects of type V to determine their priorities.
     */
    template<typename K, typename V, typename Comparator = impl::ds::StandardComparator<V>>
    class IndexedPriorityQueue {
    private:

        using BiMap = BiMap<K, std::size_t>;
        using ComparatorHelper = impl::ds::ComparatorHelper<V, Comparator>;
        using Result = std::pair<std::reference_wrapper<const K>, std::reference_wrapper<V>>;
        using Const_Result = std::pair<std::reference_wrapper<const K>, std::reference_wrapper<const V>>;

        ComparatorHelper comparator;

        /// BiMap between keys of type K and Key-Index of type std::size_t
        BiMap m_BiMap;

        /// Underlying data structure of the heap
        /// This heap is zero-indexed!
        std::vector<V> m_Heap;

        /// Position Map - Maps from key-index to position in the heap
        std::vector<std::size_t> m_PositionMap;

        /// Inverse Map - Maps from position in the heap to the key-index
        std::vector<std::size_t> m_InverseMap;

        /// Keep track of which key-index to give to the next object inserted.
        std::size_t m_Counter = 0;

        /**
         * Check if a node is a leaf node - it has no child nodes
         * @param node Index of the node in the heap
         * @return true if it is a leaf node, false if not
         */
        [[nodiscard]] inline bool isLeafNode(std::size_t node) const noexcept {
            return (node + 1) * 2 > m_Heap.size();
        }

        void bubbleUp(std::size_t node) {
            if (node == 0) {
                return;
            }
            std::size_t parentNode = (node - 1) / 2;
            while (parentNode >= 0) {
                // vector[node] > vector[parentNode]
                if (comparator(m_Heap[parentNode], m_Heap[node])) {
                    swap(node, parentNode);
                } else {
                    break;
                }
                node = parentNode;
                if (node > 0) {
                    parentNode = (node - 1) / 2;
                }
            }
        }

        /**
         * Swap the position of two elements in the heap, as well as update the position maps and inverse maps.
         * @param v Index of first element in the heap
         * @param w Index of second element in the heap
         */
        void swap(std::size_t v, std::size_t w) {
            // Get the KI of the parent and child node.
            std::size_t v_KI = m_InverseMap[v];
            std::size_t w_KI = m_InverseMap[w];
            // Swap their positionMap values.
            m_PositionMap[v_KI] = w;
            m_PositionMap[w_KI] = v;
            // Swap their inverseMap values.
            m_InverseMap[v] = w_KI;
            m_InverseMap[w] = v_KI;
            // Swap the elements in the heap
            std::swap(m_Heap[v], m_Heap[w]);
        }

        void sink(std::size_t root) {
            if (isLeafNode(root)) {
                return;
            }
            std::size_t leftNode = (root * 2) + 1;
            std::size_t rightNode = leftNode + 1;
            bool hasRightChild = rightNode < m_Heap.size();
            std::size_t largerChildIndex = leftNode;
            if (hasRightChild) {
                largerChildIndex = comparator.dualCompare(m_Heap[rightNode], m_Heap[leftNode]) ? leftNode : rightNode;
            }
            if (comparator(m_Heap[root], m_Heap[largerChildIndex])) {
                swap(root, largerChildIndex);
                sink(largerChildIndex);
            }
        }


    public:

        IndexedPriorityQueue() = default;

        /**
         * Return number of elements in the IPQ.
         * @return
         */
        [[nodiscard]] std::size_t getSize() const noexcept {
            return m_Heap.size();
        }

        /**
         * Check if the IPQ is empty.
         * @return True if empty, false if not.
         */
        [[nodiscard]] bool isEmpty() const noexcept {
            return m_Heap.size() == 0;
        }

        /**
         * Get the element with the highest priority.
         * @return Reference to the element with the highest priority
         */
        [[nodiscard]] Result peek() {
            if (isEmpty()) {
                throw std::runtime_error("Indexed Priority Queue is empty");
            }
            return {m_BiMap.find(m_InverseMap[0], BiMap::bimap_right_tag).value(), m_Heap[0]};
        }

        /**
         * Get the element with the highest priority.
         * @return Const reference to the element with the highest priority
         */
        [[nodiscard]] Const_Result peek() const {
            if (isEmpty()) {
                throw std::runtime_error("Indexed Priority Queue is empty");
            }
            return {m_BiMap.find(m_InverseMap[0], BiMap::bimap_right_tag).value(), m_Heap[0]};
        }

        /**
         * Checks if a key exists in the IPQ.
         * @param key
         * @return True if it exists, false if not
         */
        [[nodiscard]] bool exists(const K& key) const {
            return m_BiMap.contains(key, BiMap::bimap_left_tag);
        }

        /**
        * Find an element in the IPQ using its key.
        * @param key
        * @return const reference to the element
        */
        [[nodiscard]] const V& find(const K& key) const {
            auto result = m_BiMap.find(key, BiMap::bimap_left_tag);
            if (!result.has_value()) {
                throw std::invalid_argument("Key not found");
            }
            std::size_t pos = m_PositionMap[result.value()];
            return m_Heap[pos];
        }

        /**
         * Get the number of elements ahead of the specified element.
         * If the value is 0, then it is head of the queue.
         * @param key
         * @return
         */
        [[nodiscard]] std::size_t position(const K& key) const {
            auto result = m_BiMap.find(key, BiMap::bimap_left_tag);
            if (!result.has_value()) {
                throw std::invalid_argument("Key not found");
            }
            return m_PositionMap[result.value()];
        }

        /**
         * Insert key-value pair into the IPQ. If duplicate key is found, then no change is made.
         * @param key
         * @param value
         * @return True if key-value was inserted, false if duplicate key was found
         */
        bool insert(const K& key, const V& value) {
            if (exists(key)) {
                return false;
            }
            std::size_t ki = m_Counter++;
            m_BiMap.insert(key, ki);
            m_Heap.push_back(value);
            m_InverseMap.push_back(ki);
            m_PositionMap.push_back(getSize() - 1);
            bubbleUp(getSize() - 1);
            return true;
        }

        /**
         * Insert key-value pair into the IPQ. If duplicate key is found, then no change is made.
         * @param key
         * @param value
         * @return True if key-value was inserted, false if duplicate key was found
         */
        bool insert(const K& key, V&& value) {
            if (exists(key)) {
                return false;
            }
            std::size_t ki = m_Counter++;
            m_BiMap.insert(key, ki);
            m_Heap.push_back(std::move(value));
            m_InverseMap.push_back(ki);
            m_PositionMap.push_back(getSize() - 1);
            bubbleUp(getSize() - 1);
            return true;
        }

        /**
         * Insert key-value pair into the IPQ, with constructing the value in-place. If duplicate key is found,
         * then no change is made.
         * @tparam Ts
         * @param key
         * @param args
         * @return  True if key-value was inserted, false if duplicate key was found
         */
        template<typename ... Ts>
        bool emplace(const K& key, Ts&& ... args) {
            if (exists(key)) {
                return false;
            }
            std::size_t ki = m_Counter++;
            m_BiMap.insert(key, ki);
            m_Heap.emplace_back(std::forward<Ts>(args)...);
            m_InverseMap.push_back(ki);
            m_PositionMap.push_back(getSize() - 1);
            bubbleUp(getSize() - 1);
            return true;
        }

        /**
         * Remove the element with the highest priority from the IPQ.
         */
        void poll() {
            if (isEmpty()) {
                throw std::runtime_error("IPQ is empty");
            }
            m_BiMap.erase(m_InverseMap[0], BiMap::bimap_right_tag);
            swap(0, m_Heap.size() - 1);
            m_Heap.pop_back();
            m_InverseMap.pop_back();
            sink(0);
        }

        /**
         * Update an element in the priority queue using its key. The priority queue ordering is maintained.
         * @param key
         * @param value
         */
        void update(const K& key, const V& value) {
            auto result = m_BiMap.find(key, BiMap::bimap_left_tag);
            if (!result.has_value()) {
                throw std::invalid_argument("Key not found");
            }
            std::size_t pos = m_PositionMap[result.value()];
            m_Heap[pos] = value;
            swap(pos, m_Heap.size() - 1);
            bubbleUp(m_Heap.size() - 1);
        }

        /**
         * Update an element in the priority queue using its key. The priority queue ordering is maintained.
         * @param key
         * @param value
         */
        void update(const K& key, V&& value) {
            auto result = m_BiMap.find(key, BiMap::bimap_left_tag);
            if (!result.has_value()) {
                throw std::invalid_argument("Key not found");
            }
            std::size_t pos = m_PositionMap[result.value()];
            m_Heap[pos] = std::move(value);
            sink(pos);
            bubbleUp(pos);
        }

        /**
         * Remove an element from the IPQ using its key
         * @param key
         */
        void remove(const K& key) {
            auto result = m_BiMap.find(key, BiMap::bimap_left_tag);
            if (!result.has_value()) {
                throw std::invalid_argument("Key not found");
            }
            std::size_t pos = m_PositionMap[result.value()];
            swap(pos, m_Heap.size() - 1);
            m_Heap.pop_back();
            m_InverseMap.pop_back();
            sink(pos);
            bubbleUp(pos);
            m_BiMap.erase(key, BiMap::bimap_left_tag);
        }

        /**
         * Clear the IPQ. Size is reset to 0.
         */
        void clear() {
            m_BiMap.clear();
            m_Heap.clear();
            m_PositionMap.clear();
            m_InverseMap.clear();
            m_Counter = 0;
        }

    };

}

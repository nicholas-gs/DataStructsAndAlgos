//
// Created by Nicholas on 15 Dec 2020.
//

#pragma once

#include <stdexcept>
#include <cstddef>
#include <vector>
#include <iterator>
#include <type_traits>

namespace wtl {

    /**
     * A fenwick tree that has an vector as the underlying data structure.
	 * A fenwick tree, also known as a binary indexed tree, is a data structure that supports efficient elements update and range queries
	 * in O(logn) time complexity.
	 * Fenwick tree is indexed [0, size - 1], like an original array.
     * @tparam T Numeric data type
     */
    template<typename T>
    class FenwickTree {
    private:

        /// Underlying vector data structure
        std::vector<T> m_Vector;

        [[nodiscard]] bool outOfBounds(std::size_t index) const noexcept {
            if (index < 0 || index >= m_Vector.size()) {
                return true;
            }
            return false;
        }

        /** Get the value of first set bit.
         *  @param val Value to find the first set bit
         */
        [[nodiscard]] std::size_t getFirstBit(std::size_t val) const {
            return val ^ (val & (val - 1));
        }

        /**
         * Construct the fenwick tree from the original array in O(n) time.
         */
        void construct() {
            for (std::size_t i = 0; i < m_Vector.size(); i++) {
                std::size_t index = i + getFirstBit(i + 1);
                m_Vector[index] += m_Vector[i];
            }
        }

    public:

        /**
         * Constructor
         * Takes in a stl container and populates the Fenwick Tree with the elements, in the same order
         * as they appear in the container as per the iterators.
         * @tparam Container
         * @param container
         */
        template<typename Container>
        explicit FenwickTree(const Container& container) : m_Vector(container.begin(), container.end()) {
            static_assert(std::is_arithmetic_v<T>, "Fenwick Tree only supports arithmetic types");
            construct();
        }

        /**
         * Number of elements in the Fenwick Tree.
         * @return
         */
        [[nodiscard]] std::size_t getSize() const noexcept {
            return m_Vector.size();
        }

        /**
         * Retrieve the value of a single index position of the array.
         * @param index
         * @return
         */
        [[nodiscard]] T retrieve(std::size_t index) const {
            if (outOfBounds(index)) {
                throw std::invalid_argument("Index out of range");
            }
            return query(index, index);
        }

        /// Query the fenwick tree from index startIndex (inclusive) to index zero (inclusive).
        ///
        /// @throws IllegalArgumentException if startIndex is outside range of array.
        /// @param startIndex starting index of the query
        /// @return sum of all indexes from specified starting index to zero.
        [[nodiscard]] T query(std::size_t startIndex) const {
            if (outOfBounds(startIndex)) {
                throw std::invalid_argument("Index out of range");
            }
            startIndex++;
            T sum = 0;
            while (startIndex > 0) {
                sum += m_Vector[startIndex - 1];
                startIndex -= getFirstBit(startIndex);
            }
            return sum;
        }

        /**
         * Query the fenwick tree from index startIndex (inclusive) to endIndex (inclusive).
         * StartIndex cannot be larger than endIndex.
         * @param startIndex
         * @param endIndex
         * @return
         */
        [[nodiscard]] T query(std::size_t startIndex, std::size_t endIndex) const {
            if (outOfBounds(startIndex) || outOfBounds(endIndex) || startIndex > endIndex) {
                throw std::invalid_argument("Index out of range");
            }
            if (startIndex == 0) {
                return query(endIndex);
            }
            return query(endIndex) - query(startIndex - 1);
        }

        /**
         * Change the value of an index position of the array by an amount.
         * @param index
         * @param value
         */
        void update(std::size_t index, T value) {
            if (outOfBounds(index)) {
                throw std::invalid_argument("Index out of range");
            }
            index++;
            while (index <= m_Vector.size()) {
                m_Vector[index - 1] += value;
                index += getFirstBit(index);
            };
        }

        /**
         * Replace the value of an index position of the array.
         * @param index
         * @param value
         */
        void replace(std::size_t index, T value) {
            if (outOfBounds(index)) {
                throw std::invalid_argument("Index out of range");
            }
            T originalVal = retrieve(index);
            index++;
            while (index <= m_Vector.size()) {
                m_Vector[index - 1] -= originalVal;
                m_Vector[index - 1] += value;
                index += getFirstBit(index);
            }
        }

        /**
         * Destructor
         */
        ~FenwickTree() = default;

    };

}

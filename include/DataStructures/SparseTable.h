//
// Created by Nicholas on 17/12/2020.
//

#pragma once

#include <cstddef>
#include <iterator>
#include <stdexcept>
#include <cmath>
#include <type_traits>
#include <cstring>

namespace wtl {

    /**
     * A sparse table allows for O(1) range query of a static array for associative and idempotent function.
     * For non-idempotent functions, the time complexity is O(nlogn).
     * The operations that are being supported are in SparseTableOperations enum.
     * @tparam T
     */
    template<typename T>
    class SparseTable {
    public:

        /**
         * Operations supported by the sparse table
         */
        enum class Operation {
            MIN, MAX
        };

    private:

        /// Number of elements in the sparse table
        std::size_t m_Size;

        /// Pointer to the underlying array.
        /// Even though sparse table is usually represented as a 2D array, a 1D array is used for faster access.
        /// The (row, column) used to access a 2D array is converted into a single index value.
        T* m_Table = nullptr;

        /// Keep track of what operation is being done on the sparse table
        Operation m_Operation;

        /// Function pointer to the operation done by the sparse table
        T (wtl::SparseTable<T>::*m_OpFunc)(std::size_t, std::size_t, std::size_t, std::size_t) const;

        /**
         * Convert row and column index for a 2D array into the index of a 1D array
         * @param row
         * @param column
         * @return
         */
        [[nodiscard]] std::size_t indexConvert(std::size_t row, std::size_t column) const noexcept {
            return (row * m_Size) + column;
        }

        template<typename Iterator>
        void constructSparseTable(Iterator iteratorBegin, Iterator iteratorEnd) {
            std::size_t p = std::floor(std::log2(m_Size));
            m_Table = new T[m_Size * (p + 1)];

            // Copy over the first row
            std::size_t index = 0;
            while (iteratorBegin != iteratorEnd) {
                m_Table[index++] = *iteratorBegin;
                ++iteratorBegin;
            }
            // Construct the rest of the sparse table
            for (std::size_t i = 1; i <= p; i++) {
                std::size_t jump = pow(2, i - 1);
                for (std::size_t j = 0; j < m_Size; j++) {
                    std::size_t secondIndex = j + jump;
                    if (secondIndex < m_Size) {
                        T val = (this->*m_OpFunc)(i - 1, j, i - 1, secondIndex);
                        m_Table[(i * m_Size) + j] = val;
                    }
                }
            }
        }

        T min(std::size_t r1, std::size_t c1, std::size_t r2, std::size_t c2) const noexcept {
            std::size_t index1 = indexConvert(r1, c1);
            std::size_t index2 = indexConvert(r2, c2);
            if (m_Table[index1] < m_Table[index2]) {
                return m_Table[index1];
            }
            return m_Table[index2];
        }

        T max(std::size_t r1, std::size_t c1, std::size_t r2, std::size_t c2) const noexcept {
            std::size_t index1 = indexConvert(r1, c1);
            std::size_t index2 = indexConvert(r2, c2);
            if (m_Table[index1] > m_Table[index2]) {
                return m_Table[index1];
            }
            return m_Table[index2];
        }

        [[nodiscard]] bool outOfBounds(std::size_t index) const noexcept {
            return index < 0 || index >= m_Size;
        }

        void bindOperation(Operation operation) noexcept {
            switch (operation) {
                case Operation::MIN:
                    this->m_OpFunc = &SparseTable<T>::min;
                    break;
                case Operation::MAX:
                    this->m_OpFunc = &SparseTable<T>::max;
                    break;
            }
        }

    public:

        /**
         * Constructor.
         * Construct a sparse table using the elements in a stl container.
         * @tparam Iterator
         * @param iteratorBegin Iterator pointing to the first element in the stl container
         * @param iteratorEnd Iterator pointing to the past-the-end element of the stl container
         */
        template<typename Iterator>
        explicit
        SparseTable(Iterator iteratorBegin, Iterator iteratorEnd, Operation operation) : m_Operation(operation) {
            static_assert(std::is_arithmetic_v<T>, "Sparse table only supports arithmetic types");
            Iterator itFront = iteratorBegin;
            Iterator itBack = iteratorEnd;
            m_Size = std::distance(itFront, itBack);
            if (m_Size == 0) {
                throw std::invalid_argument("Cannot construct empty sparse table");
            }
            bindOperation(m_Operation);
            constructSparseTable(iteratorBegin, iteratorEnd);
        }

        /**
         * Copy constructor
         * @param other
         */
        SparseTable(const SparseTable<T>& other) {
            m_Operation = other.m_Operation;
            bindOperation(m_Operation);
            m_Size = other.m_Size;
            std::size_t p = std::floor(std::log2(m_Size));
            m_Table = new T[m_Size * (p + 1)];
            memcpy(m_Table, other.m_Table, m_Size * (p + 1) * sizeof(T));
        }

        /**
         * Copy assignment
         */
        SparseTable<T>& operator=(const SparseTable<T>& other) noexcept {
            if (this != &other) {
                delete[] m_Table;
                m_Operation = other.m_Operation;
                bindOperation(m_Operation);
                m_Size = other.m_Size;
                std::size_t p = std::floor(std::log2(m_Size));
                m_Table = new T[m_Size * (p + 1)];
                memcpy(m_Table, other.m_Table, m_Size * (p + 1) * sizeof(T));
            }
            return *this;
        }

        /**
         * Move constructor
         * @param other
         */
        SparseTable(SparseTable<T>&& other) noexcept {
            m_Size = other.m_Size;
            other.m_Size = 0;
            m_Operation = other.m_Operation;
            bindOperation(m_Operation);
            m_Table = other.m_Table;
            other.m_Table = nullptr;
        }

        /**
         * Move assignment
         */
        SparseTable<T>& operator=(SparseTable<T>&& other) noexcept {
            if (this != &other) {
                delete[] m_Table;
                m_Size = other.m_Size;
                other.m_Size = 0;
                m_Operation = other.m_Operation;
                bindOperation(m_Operation);
                m_Table = other.m_Table;
                other.m_Table = nullptr;
            }
            return *this;
        }

        /**
         * Get the number of elements in the sparse table
         * @return
         */
        [[nodiscard]] std::size_t getSize() const noexcept {
            return m_Size;
        }

        /**
         * Get the operation of the sparse table.
         * @return
         */
        [[nodiscard]] Operation getOperation() const noexcept {
            return m_Operation;
        }

        /**
         * Query between two inclusive indexes.
         * @param startIndex
         * @param endIndex
         * @return
         */
        [[nodiscard]] T query(std::size_t startIndex, std::size_t endIndex) const {
            if (outOfBounds(startIndex) || outOfBounds(endIndex)) {
                throw std::invalid_argument("Index out of range");
            }
            std::size_t p = std::floor(std::log2(endIndex - startIndex + 1));
            std::size_t k = std::pow(2, p);
            return (this->*m_OpFunc)(p, startIndex, p, endIndex - k + 1);
        }

        /**
         * Destructor
         */
        ~SparseTable() {
            delete[] m_Table;
        }
    };

}

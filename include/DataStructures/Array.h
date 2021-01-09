//
// Created by Nicholas on 22 Nov 2020.
//

#pragma once

#include <cstddef>
#include <stdexcept>
#include <cstring>
#include <functional>

namespace wtl {

    /**
     * Static array of fixed size.
     * @tparam T Type of element stored in array
     * @tparam m_Size Size of the array
     */
    template<typename T, std::size_t m_Size>
    class Array {
    private:

        /// Underlying array storage
        T m_Arr[m_Size];

        /**
         * Check if an index to the underlying storage is out of bounds
         * @param index Position of underlying array storage to access
         * @return true if invalid index, false if not
         */
        [[nodiscard]] bool outOfBounds(std::size_t index) const noexcept {
            if (index < 0 || index >= m_Size) {
                return true;
            }
            return false;
        }

    public:

        /**
         * Constructor
         */
        Array() {
            static_assert(m_Size > 0, "Size of array cannot be less than 1");
        }

        /**
         * Constructor
         * @param val Initial value to fill array
         */
        explicit Array(T val) {
            static_assert(m_Size > 0, "Size of array cannot be less than 1");
            fill(val);
        }

        /**
         * Copy constructor
         * @param other
         */
        Array(const Array& other) noexcept {
            memcpy(m_Arr, other.m_Arr, m_Size * sizeof(T));
        }

        /**
         * Copy assignment
         * @param other
         * @return
         */
        Array& operator=(const Array& other) noexcept {
            if (this != &other) {
                memcpy(m_Arr, other.m_Arr, m_Size * sizeof(T));
            }
            return *this;
        }

        /**
         * Move constructor is deleted
         * @param other
         */
        Array(Array&& other) = delete;

        /**
         * Move assignment
         * @param other
         * @return
         */
        Array& operator=(Array&& other) = delete;

        /**
         * Return size of the array
         * @return Size of array
         */
        [[nodiscard]] constexpr std::size_t getSize() const noexcept {
            return m_Size;
        }

        /**
         * Return pointer to underlying array serving as storage
         * @return Pointer to the underlying array
         */
        [[nodiscard]] T* data() noexcept {
            return m_Arr;
        }

        /**
         * Return pointer to const array serving as storage
         * @return Pointer to const array serving as storage
         */
        [[nodiscard]] const T* data() const noexcept {
            return m_Arr;
        }

        /**
         * Return reference to element at the front
         * @return Reference to element at the front
         */
        [[nodiscard]] T& front() noexcept {
            return at(0);
        }

        /**
         * Return const reference to element at the front
         * @return Const reference to element at the front
         */
        [[nodiscard]] const T& front() const noexcept {
            return at(0);
        }

        /**
        * Return reference to element at the back
        * @return Reference to element at the back
        */
        [[nodiscard]] T& back() noexcept {
            return at(m_Size - 1);
        }

        /**
        * Return const reference to element at the back
        * @return Const reference to element at the back
        */
        [[nodiscard]] const T& back() const noexcept {
            return at(m_Size - 1);
        }

        /**
         * Access element at specified position
         * @throws std::out_of_range if position is invalid
         * @param index Position to access
         * @return reference to specified element
         */
        [[nodiscard]] T& at(std::size_t index) {
            if (outOfBounds(index)) {
                throw std::out_of_range("Index argument out of range");
            }
            return m_Arr[index];
        }

        /**
         * Access element at specified position
         * @throws std::out_of_range if position is invalid
         * @param index Position to access
         * @return const reference to specified element
         */
        [[nodiscard]] const T& at(std::size_t index) const {
            if (outOfBounds(index)) {
                throw std::out_of_range("Index argument out of range");
            }
            return m_Arr[index];
        }

        /**
          * Insert element at specified index
          * @throws std::out_of_range if position is invalid
          * @param element Element to insert by const reference
          * @param index Position to insert element
          */
        void insertAt(const T& element, std::size_t index) {
            if (outOfBounds(index)) {
                throw std::out_of_range("Index argument out of range");
            }
            m_Arr[index] = element;
        }

        /**
         * Insert element at specified index
         * @throws std::out_of_range if position is invalid
         * @param element Element to insert by rvalue
         * @param index Position to insert element
         */
        void insertAt(T&& element, std::size_t index) {
            if (outOfBounds(index)) {
                throw std::out_of_range("Index argument out of range");
            }
            m_Arr[index] = std::move(element);
        }

        /**
         * Fill the array with specified element
         * @param element Element to fill the array with
         */
        void fill(const T& element) noexcept {
            for (size_t i = 0; i < m_Size; i++) {
                m_Arr[i] = element;
            }
        }

        /**
         * Performs an user defined operation on each element in the array.
         * @param func Lambda that returns void and takes in a T reference.
         */
        void foreach(const std::function<void(T&)>& func) noexcept {
            for (std::size_t i = 0; i < m_Size; i++) {
                func(m_Arr[i]);
            }
        }

        /**
         * Performs an user defined operation on each element in the array.
         * @param func Lambda that returns void and takes in a const T reference
         */
        void foreach(const std::function<void(const T&)>& func) const noexcept {
            for (std::size_t i = 0; i < m_Size; i++) {
                func(m_Arr[i]);
            }
        }

        /**
          * Access specified element
          * @throws std::out_of_range if invalid index access
          * @param index
          * @return reference to element at specified index
          */
        [[nodiscard]] T& operator[](std::size_t index) {
            if (outOfBounds(index)) {
                throw std::out_of_range("Accessing invalid index");
            }
            return m_Arr[index];
        }

        /**
         * Access specified element
         * @throws std::out_of_range if invalid index access
         * @param index
         * @return const reference to element at specified index
         */
        [[nodiscard]] const T& operator[](std::size_t index) const {
            if (outOfBounds(index)) {
                throw std::out_of_range("Accessing invalid index");
            }
            return m_Arr[index];
        }

        /**
         * Check if 2 arrays are equals
         * @param other const reference to other array
         * @return true if equals, false if not equals
         */
        [[nodiscard]] bool equals(const Array& other) const noexcept {
            return *this == other;
        }

        /**
         * Check if 2 arrays are not equals
         * @param other const reference to other array
         * @return true if not equals, false if equals
         */
        [[nodiscard]] bool notEquals(const Array& other) const noexcept {
            return *this != other;
        }

        /**
         * Check if 2 arrays are equal
         * @param other
         * @return
         */
        bool operator==(const Array& other) const noexcept {
            if (m_Size != other.getSize()) {
                return false;
            }
            for (std::size_t i = 0; i < m_Size; i++) {
                if (m_Arr[i] != other.at(i)) {
                    return false;
                }
            }
            return true;
        }

        /**
         * Check if 2 arrays are not equal.
         * @param other
         * @return
         */
        bool operator!=(const Array& other) const noexcept {
            return !(*this == other);
        }

    };

}

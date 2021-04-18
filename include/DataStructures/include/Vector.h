//
// Created by Nicholas on 14/12/2020.
//

#pragma once

#include <cstddef>
#include <utility>
#include <stdexcept>
#include <initializer_list>

namespace wtl {

    template<typename T>
    class Vector {
    private:

        // Pointer to underlying array storage
        T* m_Arr = nullptr;

        // Number of elements currently stored
        std::size_t m_Size;

        // Size of underlying storage element allocated
        std::size_t m_Capacity;

        /**
         * Check if an index is out of bounds.
         * @param index
         * @return True if out of bounds, false if not.
         */
        [[nodiscard]] bool outOfBounds(std::size_t index) const {
            if (index < 0 || index >= m_Size) {
                return true;
            }
            return false;
        }

        /**
         * Reallocate the contents of a filled storage into a new storage.
         * @param newCapacity
         */
        void reAllocate(std::size_t newCapacity) noexcept {
            T* newArr = (T*) (::operator new(newCapacity * sizeof(T)));
            for (std::size_t i = 0; i < m_Size; i++) {
                new(&newArr[i]) T(std::move(m_Arr[i]));
                m_Arr[i].~T();
            }
            ::operator delete(m_Arr, m_Capacity * sizeof(T));
            m_Arr = newArr;
            m_Capacity = newCapacity;
        }

    public:

        /**
         * Default constructor
         */
        Vector() : m_Size(0), m_Capacity(2) {
            m_Arr = (T*) (::operator new(m_Capacity * sizeof(T)));
        }

        /**
         * Constructor
         * @param capacity Initial capacity reserved for vector
         */
        explicit Vector(std::size_t capacity) : m_Size(0), m_Capacity(capacity) {
            m_Arr = (T*) (::operator new(m_Capacity * sizeof(T)));
        }

        Vector(std::initializer_list<T> list)
            : m_Size(list.size()), m_Capacity(m_Size * 2) {
                m_Arr = (T*)(::operator new(m_Capacity * sizeof(T)));
                typename std::initializer_list<T>::const_iterator it = list.begin();
                for(std::size_t i = 0; i < list.size(); i++) {
                    new(&m_Arr[i]) T(*it);
                    ++it;
                }
            }

        /**
         * Copy constructor
         * @param other
         */
        Vector(const Vector& other) noexcept {
            m_Size = other.m_Size;
            m_Capacity = other.m_Capacity;
            m_Arr = (T*) (::operator new(m_Capacity * sizeof(T)));
            for (std::size_t i = 0; i < m_Size; i++) {
                new(&m_Arr[i]) T(other.m_Arr[i]);
            }
        }

        /**
         * Copy assignment
         * @param other
         * @return
         */
        Vector<T>& operator=(const Vector& other) noexcept {
            if (this != &other) {
                for (std::size_t i = 0; i < m_Size; i++) {
                    m_Arr[i].~T();
                }
                ::operator delete(m_Arr, m_Capacity * sizeof(m_Capacity));

                m_Size = other.m_Size;
                m_Capacity = other.m_Capacity;
                m_Arr = (T*) (::operator new(m_Capacity * sizeof(T)));
                for (std::size_t i = 0; i < m_Size; i++) {
                    new(&m_Arr[i]) T(other.m_Arr[i]);
                }
            }
            return *this;
        }

        /**
         * Move constructor
         * @param other
         */
        Vector(Vector&& other) = delete;

        /**
         * Move assignment
         * @param other
         * @return
         */
        Vector<T>& operator=(Vector&& other) = delete;

        /**
         * Get the number of elements in the vector
         * @return Number of elements
         */
        [[nodiscard]] std::size_t getSize() const noexcept {
            return m_Size;
        }

        /**
         * Get the number of elements that can be held in currently allocated storage
         * @return
         */
        [[nodiscard]] std::size_t getCapacity() const noexcept {
            return m_Capacity;
        }

        /**
         * Check if the vector is empty
         * @return True if vector is empty, false if not
         */
        [[nodiscard]] bool isEmpty() const noexcept {
            return m_Size == 0;
        }

        /**
         * Access specified element with bounds checking
         * @param index
         * @return
         */
        [[nodiscard]] T& at(std::size_t index) {
            if (outOfBounds(index)) {
                throw std::out_of_range("Accessing index out of range");
            }
            return m_Arr[index];
        }

        /**
         * Access specified element with bounds checking
         * @param index
         * @return
         */
        [[nodiscard]] const T& at(std::size_t index) const {
            if (outOfBounds(index)) {
                throw std::out_of_range("Accessing index out of range");
            }
            return m_Arr[index];
        }

        /**
         * Returns a reference to the element at specified index with bounds checking.
         * @param index
         * @return
         */
        [[nodiscard]] const T& operator[](std::size_t index) const {
            if (outOfBounds(index)) {
                throw std::out_of_range("Accessing index out of range");
            }
            return m_Arr[index];
        }

        /**
         * Returns a reference to the element at specified index with bounds checking.
         * @param index
         * @return
         */
        [[nodiscard]] T& operator[](std::size_t index) {
            if (outOfBounds(index)) {
                throw std::out_of_range("Accessing index out of range");
            }
            return m_Arr[index];
        }

        /**
         * Append an element at the back of the vector
         * @param value
         */
        void pushBack(const T& value) noexcept {
            if (m_Size == m_Capacity) {
                reAllocate(m_Capacity * 1.5);
            }
            new(&m_Arr[m_Size]) T(value);
            m_Size++;
        }

        /**
         * Append an element at the back of the vector
         * @param value
         */
        void pushBack(T&& value) noexcept {
            if (m_Size == m_Capacity) {
                reAllocate(m_Capacity * 1.5);
            }
            new(&m_Arr[m_Size]) T(std::move(value));
            m_Size++;
        }

        /**
         * Construct an element in-place at the back of the vector
         * @tparam Ts
         * @param args
         * @return Reference to the constructed element
         */
        template<typename ... Ts>
        T& emplaceBack(Ts&& ... args) {
            if (m_Size == m_Capacity) {
                reAllocate(m_Capacity * 1.5);
            }
            new(&m_Arr[m_Size]) T(std::forward<Ts>(args)...);
            return m_Arr[m_Size++];
        }

        /**
         * Remove the last element.
         */
        void popBack() noexcept {
            if (m_Size > 0) {
                m_Arr[--m_Size].~T();
            }
            std::size_t temp = m_Capacity / 2;
            if ((m_Size < temp) && (temp >= 2)) {
                reAllocate(temp);
            }
        }

        /**
         * Shrink the vector to release unused memory, such that the capacity is equal to the size.
         */
        void shrinkToFit() noexcept {
            if (m_Size >= 2 && m_Size < m_Capacity) {
                T* temp = (T*) (::operator new(m_Size * sizeof(T)));
                for (std::size_t i = 0; i < m_Size; i++) {
                    new(&temp[i]) T(std::move(m_Arr[i]));
                    m_Arr[i].~T();
                }
                ::operator delete(m_Arr, m_Capacity * sizeof(m_Capacity));
                m_Capacity = m_Size;
                m_Arr = temp;
            }
        }

        /**
         * Clear all elements in the vector. Size is reset to 0.
         */
        void clear() noexcept {
            for (std::size_t i = 0; i < m_Size; i++) {
                m_Arr[i].~T();
            }
            ::operator delete(m_Arr, m_Capacity * sizeof(m_Capacity));
            m_Size = 0;
            m_Capacity = 2;
            m_Arr = (T*) (::operator new(m_Capacity * sizeof(T)));
        }

        /**
         * Check if 2 vectors are equal. They are equal if the number of elements are equal,
         * and the elements at each index are equal.
         * @param other
         * @return
         */
        bool operator==(const Vector<T>& other) const noexcept {
            if (m_Size != other.m_Size) {
                return false;
            }
            for (std::size_t i = 0; i < m_Size; i++) {
                if (m_Arr[i] != other.m_Arr[i]) {
                    return false;
                }
            }
            return true;
        }

        /**
         * Check if 2 vectors are not equal
         * @param other
         * @return
         */
        bool operator!=(const Vector<T>& other) const noexcept {
            return !(*this == other);
        }

        /**
         * Destructor
         */
        ~Vector() {
            for (std::size_t i = 0; i < m_Size; i++) {
                m_Arr[i].~T();
            }
            ::operator delete(m_Arr, m_Capacity * sizeof(m_Capacity));
        }

    };

}

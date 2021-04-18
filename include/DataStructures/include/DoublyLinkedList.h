//
// Created by Nicholas on 02 Dec 2020.
//

#pragma once

#include <cstddef>
#include <stdexcept>
#include <functional>
#include <utility>
#include <initializer_list>

namespace wtl {

    namespace impl {

        template<typename T>
        struct DLLNode {
            /// Element stored in the node
            T m_Data;

            /// Pointer to the previous node
            DLLNode<T>* m_Prev;

            /// Pointer to the next node
            DLLNode<T>* m_Next;

            /**
             * Constructor
             * @param data
             * @param next
             */
            explicit DLLNode(const T& data, DLLNode<T>* prev = nullptr, DLLNode<T>* next = nullptr)
                    : m_Data(data), m_Prev(prev), m_Next(next) {}

            /**
             * Constructor
             * @param data Element stored in the node
             * @param next Pointer to next node
             */
            explicit DLLNode(T&& data, DLLNode<T>* prev = nullptr, DLLNode<T>* next = nullptr)
                    : m_Data(std::move(data)), m_Prev(prev), m_Next(next) {}

            /**
             * Constructor
             * @tparam Ts
             * @param prev
             * @param next
             * @param args
             */
            template<typename ... Ts>
            explicit DLLNode(DLLNode<T>* prev, DLLNode<T>* next, Ts&& ... args) : m_Data(
                    std::forward<Ts>(args)...), m_Prev(prev), m_Next(next) {}

        };
    }

    /**
     * Doubly linked list
     * @tparam T Type of element stored
     */
    template<typename T>
    class DoublyLinkedList {
    private:

        /// Typedef wtl::impl::DLLNode<T>
        typedef wtl::impl::DLLNode<T> DLLNode;

        /// Number of elements stored. Also the number of nodes
        std::size_t m_Size = 0;

        /// Pointer to the front of the linked list
        DLLNode* m_Head = nullptr;

        /// Pointer to the back of the linked list
        DLLNode* m_Tail = nullptr;

        /**
         * Check if index is out of bounds
         * @param index
         * @return true if out of bounds, false if now
         */
        [[nodiscard]] bool outOfBounds(std::size_t index) const noexcept {
            if (index < 0 || index >= m_Size) {
                return true;
            }
            return false;
        }

        /**
         * Return pointer to the node at the specified index. No bounds checking conducted!
         * Traverses from either from the front or back of the linked list depending on which was closer.
         * @param index
         * @return Pointer to node at specified index
         */
        [[nodiscard]] DLLNode* ptrTo(std::size_t index) const noexcept {
            DLLNode* ptr = nullptr;
            if (index <= (m_Size - 1) / 2) {
                ptr = m_Head;
                for (int i = 0; i < index; i++) {
                    ptr = ptr->m_Next;
                }
            } else {
                ptr = m_Tail;
                for (int i = m_Size - 1; i != index; i--) {
                    ptr = ptr->m_Prev;
                }
            }
            return ptr;
        }

        /**
         * Insert a new node into a currently empty linked list
         * @param data Element to be inserted
         */
        void insertToEmptyList(const T& data) noexcept {
            auto* node = new DLLNode(data);
            m_Head = node;
            m_Tail = node;
            m_Size++;
        }

        /**
         * Insert a new node into a currently empty linked list
         * @param data Element to be inserted
         */
        void insertToEmptyList(T&& data) noexcept {
            auto* node = new DLLNode(std::move(data));
            m_Head = node;
            m_Tail = node;
            m_Size++;
        }

        template<typename ... Ts>
        void insertToEmptyList(Ts&& ... args) noexcept {
            auto* node = new DLLNode(nullptr, nullptr, std::forward<Ts>(args)...);
            m_Head = node;
            m_Tail = node;
            m_Size++;
        }

        /**
         * Remove a node from the linked list which is not either the first or last node
         * @param nodeToRemove
         */
        void removeMiddle(DLLNode* nodeToRemove) noexcept {
            (nodeToRemove->m_Prev)->m_Next = nodeToRemove->m_Next;
            (nodeToRemove->m_Next)->m_Prev = nodeToRemove->m_Prev;
            delete nodeToRemove;
            m_Size--;
        }

    public:

        /**
         * Constructor
         */
        DoublyLinkedList() = default;

        DoublyLinkedList(std::initializer_list<T> list) {
            for(typename std::initializer_list<T>::const_iterator it = list.begin(); it != list.end(); ++it) {
                appendBack(*it);
            }
        }

        /**
         * Copy constructor
         * @param other
         */
        DoublyLinkedList(const DoublyLinkedList<T>& other) noexcept {
            DLLNode* ptr = other.m_Head;
            while (ptr != nullptr) {
                appendBack(ptr->m_Data);
                ptr = ptr->m_Next;
            }
        }

        /**
         * Copy assignment
         * @param other
         * @return
         */
        DoublyLinkedList<T>& operator=(const DoublyLinkedList<T>& other) noexcept {
            if (this != &other) {
                clear();
                DLLNode* ptr = other.m_Head;
                while (ptr != nullptr) {
                    appendBack(ptr->m_Data);
                    ptr = ptr->m_Next;
                }
            }
            return *this;
        }

        /**
         * Move constructor
         * @param other
         */
        DoublyLinkedList(DoublyLinkedList<T>&& other) noexcept {
            m_Head = other.m_Head;
            other.m_Head = nullptr;
            m_Tail = other.m_Tail;
            other.m_Tail = nullptr;
            m_Size = other.m_Size;
            other.m_Size = 0;
        }

        /**
         * Move assignment
         * @param other
         * @return
         */
        DoublyLinkedList<T>& operator=(DoublyLinkedList<T>&& other) noexcept {
            if (this != &other) {
                clear();
                m_Head = other.m_Head;
                other.m_Head = nullptr;
                m_Tail = other.m_Tail;
                other.m_Tail = nullptr;
                m_Size = other.m_Size;
                other.m_Size = 0;
            }
            return *this;
        }

        /**
         * Return number of elements
         * @return Number of elements
         */
        [[nodiscard]] std::size_t getSize() const noexcept {
            return m_Size;
        }

        /**
         * Check if linked list is empty
         * @return true if empty, false if not
         */
        [[nodiscard]] bool isEmpty() const noexcept {
            return m_Size == 0;
        }

        /**
         * Retrieve element at the front
         * @throws std::out_of_range if linked list is empty
         * @return Reference to element at the front
         */
        [[nodiscard]] T& front() {
            if (isEmpty()) {
                throw std::out_of_range("Doubly linked list is empty");
            }
            return m_Head->m_Data;
        }

        /**
        * Retrieve element at the front
        * @throws std::out_of_range if linked list is empty
        * @return Const reference to element at the front
        */
        [[nodiscard]] const T& front() const {
            if (isEmpty()) {
                throw std::out_of_range("Doubly linked list is empty");
            }
            return m_Head->m_Data;
        }

        /**
        * Retrieve element at the back
        * @throws std::out_of_range if linked list is empty
        * @return Reference to element at the front
        */
        [[nodiscard]] T& back() {
            if (isEmpty()) {
                throw std::out_of_range("Doubly linked list is empty");
            }
            return m_Tail->m_Data;
        }

        /**
        * Retrieve element at the front
        * @throws std::out_of_range if linked list is empty
        * @return Const reference to element at the front
        */
        [[nodiscard]] const T& back() const {
            if (isEmpty()) {
                throw std::out_of_range("Doubly linked list is empty");
            }
            return m_Tail->m_Data;
        }

        /**
         * Retrieve element at specified index
         * @throws std::out_of_range if linked list is empty
         * @param index
         * @return reference to element
         */
        [[nodiscard]] T& at(std::size_t index) {
            if (outOfBounds(index)) {
                throw std::out_of_range("Index out of range");
            }
            return ptrTo(index)->m_Data;
        }

        /**
         * Retrieve element at specified index
         * @throws std::out_of_range if linked list is empty
         * @param index
         * @return Const reference to element
         */
        [[nodiscard]] const T& at(std::size_t index) const {
            if (outOfBounds(index)) {
                throw std::out_of_range("Index out of range");
            }
            return ptrTo(index)->m_Data;
        }

        /**
        * Check if an element is present in the linked list
        * @param element
        * @return true if element is present, false if not present
        */
        [[nodiscard]] bool contains(const T& element) const noexcept {
            return count(element) != 0;
        }


        /**
         * Count the number of matching elements in the linked list.
         * @param element Element to compare to
         * @return Number of matching elements
         */
        [[nodiscard]] std::size_t count(const T& element) const noexcept {
            std::size_t count = 0;
            DLLNode* ptr = m_Head;
            while (ptr != nullptr) {
                if (ptr->m_Data == element) {
                    count++;
                }
                ptr = ptr->m_Next;
            }
            return count;
        }

        /**
         * Check if 2 doubly linked lists are equal piecewise.
         * @param other
         * @return
         */
        bool equals(const DoublyLinkedList<T>& other) const noexcept {
            return (*this) == other;
        }

        /**
         * Check if 2 doubly linked lists are not equal piecewise.
         * @param other
         * @return
         */
        bool notEquals(const DoublyLinkedList<T>& other) const noexcept {
            return !((*this) == other);
        }

        /**
         * Check if 2 doubly linked lists are equal piecewise.
         * @param other
         * @return
         */
        bool operator==(const DoublyLinkedList<T>& other) const noexcept {
            if (m_Size != other.m_Size) {
                return false;
            }
            DLLNode* ptr = m_Head;
            DLLNode* otherPtr = other.m_Head;
            while (ptr != nullptr) {
                if (!(ptr->m_Data == otherPtr->m_Data)) {
                    return false;
                }
                ptr = ptr->m_Next;
                otherPtr = otherPtr->m_Next;
            }
            return true;
        }

        /**
         * Check if 2 doubly linked lists are not equal piecewise.
         * @param other
         * @return
         */
        bool operator!=(const DoublyLinkedList<T>& other) const noexcept {
            return !(*this == other);
        }

        /**
         * Performs an user defined operation on each element in the array.
         * @param func Lambda that returns void and takes in a T reference.
         */
        void foreach(const std::function<void(T&)>& func) noexcept {
            DLLNode* ptr = m_Head;
            for (std::size_t i = 0; i < m_Size; i++) {
                func(ptr->m_Data);
                ptr = ptr->m_Next;
            }
        }

        /**
         * Performs an user defined operation on each element in the array.
         * @param func Lambda that returns void and takes in a const T reference
         */
        void foreach(const std::function<void(const T&)>& func) const noexcept {
            DLLNode* ptr = m_Head;
            for (std::size_t i = 0; i < m_Size; i++) {
                func(ptr->m_Data);
                ptr = ptr->m_Next;
            }
        }

        /**
         * Insert element to the front of the linked list
         * @param data Element to insert
         */
        void appendFront(const T& data) noexcept {
            if (isEmpty()) {
                insertToEmptyList(data);
            } else {
                auto* newHead = new DLLNode(data, nullptr, m_Head);
                m_Head->m_Prev = newHead;
                m_Head = newHead;
                m_Size++;
            }
        }

        /**
        * Insert element to the front of the linked list
        * @param data Element to insert
        */
        void appendFront(T&& data) noexcept {
            if (isEmpty()) {
                insertToEmptyList(std::move(data));
            } else {
                auto* newHead = new DLLNode(std::move(data), nullptr, m_Head);
                m_Head->m_Prev = newHead;
                m_Head = newHead;
                m_Size++;
            }
        }

        /**
        * Insert an element at the front of the linked list in-place.
        * @tparam Ts
        * @param args
        * @return Reference to the inserted element
        */
        template<typename ... Ts>
        T& emplaceFront(Ts&& ... args) noexcept {
            if (isEmpty()) {
                insertToEmptyList(std::forward<Ts>(args)...);
            } else {
                auto* newHead = new DLLNode(nullptr, m_Head, std::forward<Ts>(args)...);
                m_Head->m_Prev = newHead;
                m_Head = newHead;
                m_Size++;
            }
            return m_Head->m_Data;
        }

        /**
         * Insert element to the back of the linked list
         * @param data Element to insert
         */
        void appendBack(const T& data) noexcept {
            if (isEmpty()) {
                insertToEmptyList(data);
            } else {
                auto* newTail = new DLLNode(data, m_Tail, nullptr);
                m_Tail->m_Next = newTail;
                m_Tail = newTail;
                m_Size++;
            }
        }

        /**
         * Insert element to the back of the linked list
         * @param data Element to insert
         */
        void appendBack(T&& data) noexcept {
            if (isEmpty()) {
                insertToEmptyList(std::move(data));
            } else {
                auto* newTail = new DLLNode(std::move(data), m_Tail, nullptr);
                m_Tail->m_Next = newTail;
                m_Tail = newTail;
                m_Size++;
            }
        }

        /**
         * Insert an element at the back of the linked list in-place.
         * @tparam Ts
         * @param args
         * @return Reference to the inserted element
         */
        template<typename ... Ts>
        T& emplaceBack(Ts&& ... args) noexcept {
            if (isEmpty()) {
                insertToEmptyList(std::forward<Ts>(args)...);
            } else {
                auto* newTail = new DLLNode(m_Tail, nullptr, std::forward<Ts>(args)...);
                m_Tail->m_Next = newTail;
                m_Tail = newTail;
                m_Size++;
            }
            return m_Tail->m_Data;
        }

        /**
         * Insert an element into the linked list at the specified index.
         * If index supplied is 0, the element is inserted at the front.
         * If index supplied is the size of the linked ist, the element is inserted at the back.
         * If index supplied is within the bounds, the new element will push the element at the current index
         * one index backwards.
         * @param data
         * @param index
         */
        void insertAt(std::size_t index, const T& data) {
            if (index > m_Size || index < 0) {
                throw std::out_of_range("Index out of range");
            }
            if (isEmpty() && index == 0) {
                insertToEmptyList(data);
            } else if (index == 0) {
                appendFront(data);
            } else if (index == m_Size) {
                appendBack(data);
            } else {
                auto* newNode = new DLLNode(data);
                DLLNode* currNode = ptrTo(index);
                (currNode->m_Prev)->m_Next = newNode;
                newNode->m_Prev = currNode->m_Prev;
                currNode->m_Prev = newNode;
                newNode->m_Next = currNode;
                m_Size++;
            }
        }

        /**
         * Insert an element into the linked list at the specified index.
         * If index supplied is 0, the element is inserted at the front.
         * If index supplied is the size of the linked ist, the element is inserted at the back.
         * If index supplied is within the bounds, the new element will push the element at the current index
         * one index backwards.
         * @param data
         * @param index
         */
        void insertAt(std::size_t index, T&& data) {
            if (index > m_Size || index < 0) {
                throw std::out_of_range("Index out of range");
            }
            if (isEmpty() && index == 0) {
                insertToEmptyList(std::move(data));
            } else if (index == 0) {
                appendFront(std::move(data));
            } else if (index == m_Size) {
                appendBack(std::move(data));
            } else {
                auto* newNode = new DLLNode(std::move(data));
                DLLNode* currNode = ptrTo(index);
                (currNode->m_Prev)->m_Next = newNode;
                newNode->m_Prev = currNode->m_Prev;
                currNode->m_Prev = newNode;
                newNode->m_Next = currNode;
                m_Size++;
            }
        }

        /**
         * Insert an element in-place at the specified index.
         * If index supplied is 0, the element is inserted at the front.
         * If index supplied is the size of the linked ist, the element is inserted at the back.
         * If index supplied is within the bounds, the new element will push the element at the current index
         * @tparam Ts
         * @param args
         * @return Reference to the inserted element
         */
        template<typename ... Ts>
        T& emplaceAt(std::size_t index, Ts&& ... args) noexcept {
            if (index > m_Size || index < 0) {
                throw std::out_of_range("Index out of range");
            }
            if (isEmpty() && index == 0) {
                insertToEmptyList(std::forward<Ts>(args)...);
                return m_Head->m_Data;
            } else if (index == 0) {
                emplaceFront(std::forward<Ts>(args)...);
                return m_Head->m_Data;
            } else if (index == m_Size) {
                emplaceBack(std::forward<Ts>(args)...);
                return m_Tail->m_Data;
            } else {
                auto* newNode = new DLLNode(nullptr, nullptr, std::forward<Ts>(args)...);
                DLLNode* currNode = ptrTo(index);
                (currNode->m_Prev)->m_Next = newNode;
                newNode->m_Prev = currNode->m_Prev;
                currNode->m_Prev = newNode;
                newNode->m_Next = currNode;
                m_Size++;
                return newNode->m_Data;
            }
        }

        /**
         * Removes first element in O(1). Nothing happens if linked list is already empty
         */
        void detachFront() noexcept {
            if (!isEmpty()) {
                DLLNode* tempPtr = m_Head;
                m_Head = m_Head->m_Next;
                m_Size--;
                if (isEmpty()) {
                    m_Tail = nullptr;
                } else {
                    m_Head->m_Prev = nullptr;
                }
                delete tempPtr;
            }
        }

        /**
         * Removes last element in O(1). Nothing happens is linked list is already empty
         */
        void detachBack() noexcept {
            if (!isEmpty()) {
                DLLNode* tempPtr = m_Tail;
                m_Tail = m_Tail->m_Prev;
                m_Size--;
                if (isEmpty()) {
                    m_Head = nullptr;
                } else {
                    m_Tail->m_Next = nullptr;
                }
                delete tempPtr;
            }
        }

        /**
         * Remove element at specified index
         * @param index
         */
        void removeAt(std::size_t index) {
            if (outOfBounds(index)) {
                throw std::out_of_range("Index out of range");
            }
            if (index == 0) {
                return detachFront();
            } else if (index == m_Size - 1) {
                return detachBack();
            } else {
                DLLNode* tempPtr = m_Head;
                for (int i = 0; i < index - 1; i++) {
                    tempPtr = tempPtr->m_Next;
                }
                removeMiddle(ptrTo(index));
            }
        }


        /**
         * Remove all elements in the linked list that matches the supplied element
         * @param element
         * @return Number of removed elements
         */
        std::size_t remove(const T& element) noexcept {
            // Keep track of how many nodes have been removed
            std::size_t count = 0;
            DLLNode* ptr = m_Head;
            while (ptr != nullptr) {
                DLLNode* nextNode = ptr->m_Next;
                if (ptr->m_Data == element) {
                    count++;
                    // Node to remove is the head
                    if (ptr->m_Prev == nullptr) {
                        detachFront();
                    }
                        // Node to remove is the tail
                    else if (ptr->m_Next == nullptr) {
                        detachBack();
                    } else {
                        removeMiddle(ptr);
                    }
                }
                ptr = nextNode;
            }
            return count;
        }

        /**
         * Removes all elements in O(n). Size returns to 0.
         */
        void clear() noexcept {
            DLLNode* tempPtr = m_Head;
            DLLNode* nextPtr = nullptr;
            while (tempPtr != nullptr) {
                nextPtr = tempPtr->m_Next;
                delete tempPtr;
                tempPtr = nextPtr;
            }
            m_Size = 0;
            m_Head = nullptr;
            m_Tail = nullptr;
        }

        /**
         * Destructor
         */
        ~DoublyLinkedList() {
            clear();
        }
    };

}

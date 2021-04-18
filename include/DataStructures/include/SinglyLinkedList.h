//
// Created by Nicholas on 24 Nov 2020.
//

#pragma once

#include <cstddef>
#include <stdexcept>
#include <functional>
#include <utility>
#include <initializer_list>

namespace wtl {

    namespace impl {

        /**
         * Node in singly linked list
         * @tparam T type of element stored in each node
         */
        template<typename T>
        struct SLLNode {

            /// Element stored in the node
            T m_Data;

            /// Pointer to the next node
            SLLNode<T>* m_Next;

            /**
             * Constructor
             * @param data
             * @param next
             */
            explicit SLLNode(const T& data, SLLNode<T>* next = nullptr) : m_Data(data), m_Next(next) {}

            /**
             * Constructor
             * @param data Element stored in the node
             * @param next Pointer to next node
             */
            explicit SLLNode(T&& data, SLLNode<T>* next = nullptr) : m_Data(std::move(data)), m_Next(next) {}

            /**
             * Constructor
             * @tparam Ts
             * @param next
             * @param args
             */
            template<typename ... Ts>
            explicit
            SLLNode(SLLNode<T>* next, Ts&&... args) : m_Data(std::forward<Ts>(args)...), m_Next(next) {}

        };

    }

    /**
     * Singly linked list with O(1) access, insertion and removal of elements at the front.
     * @tparam T Type of element stored
     */
    template<typename T>
    class SinglyLinkedList {
    private:

        /// Typedef wtl::impl::SLLNode<T>
        typedef wtl::impl::SLLNode<T> SLLNode;

        /// Number of elements stored. Also the number of nodes
        std::size_t m_Size = 0;

        /// Pointer to the front of the linked list
        SLLNode* m_Head = nullptr;

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
         * @param index
         * @return Pointer to node at specified index
         */
        [[nodiscard]] SLLNode* ptrTo(std::size_t index) const noexcept {
            SLLNode* ptr = m_Head;
            for (std::size_t i = 0; i < index; i++) {
                ptr = ptr->m_Next;
            }
            return ptr;
        }

    public:

        /**
         * Constructor
         */
        SinglyLinkedList() = default;

        SinglyLinkedList(std::initializer_list<T> list) {
            for(auto it = std::rbegin(list); it != std::rend(list); ++it) {
                appendFront(*it);
            }
        }

        /**
         * Copy constructor
         * @param other
         */
        SinglyLinkedList(const SinglyLinkedList<T>& other) {
            SLLNode* ptr = other.m_Head;
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
        SinglyLinkedList<T>& operator=(const SinglyLinkedList<T>& other) {
            if (this != &other) {
                clear();
                SLLNode* ptr = other.m_Head;
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
        SinglyLinkedList(SinglyLinkedList<T>&& other) noexcept {
            m_Size = other.m_Size;
            other.m_Size = 0;
            m_Head = other.m_Head;
            other.m_Head = nullptr;
        }

        /**
         * Move assignment
         * @param other
         * @return
         */
        SinglyLinkedList<T>& operator=(SinglyLinkedList<T>&& other) noexcept {
            if (this != &other) {
                clear();
                m_Size = other.m_Size;
                other.m_Size = 0;
                m_Head = other.m_Head;
                other.m_Head = nullptr;
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
                throw std::out_of_range("Singly linked list is empty");
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
                throw std::out_of_range("Singly linked list is empty");
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
                throw std::out_of_range("Singly linked list is empty");
            }
            return ptrTo(m_Size - 1)->m_Data;
        }

        /**
        * Retrieve element at the front
        * @throws std::out_of_range if linked list is empty
        * @return Const reference to element at the front
        */
        [[nodiscard]] const T& back() const {
            if (isEmpty()) {
                throw std::out_of_range("Singly linked list is empty");
            }
            return ptrTo(m_Size - 1)->m_Data;
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
            SLLNode* temp = m_Head;
            while (temp != nullptr) {
                if (temp->m_Data == element) {
                    count++;
                }
                temp = temp->m_Next;
            }
            return count;
        }

        /**
         * Check if 2 singly linked lists are equal piecewise.
         * @param other
         * @return
         */
        bool equals(const SinglyLinkedList<T>& other) const noexcept {
            return (*this) == other;
        }

        /**
         * Check if 2 singly linked lists are not equal piecewise.
         * @param other
         * @return
         */
        bool notEquals(const SinglyLinkedList<T>& other) const noexcept {
            return !((*this) == other);
        }

        /**
         * Check if 2 singly linked lists are equal piecewise.
         * @param other
         * @return
         */
        bool operator==(const SinglyLinkedList<T>& other) const noexcept {
            if (m_Size != other.m_Size) {
                return false;
            }
            SLLNode* ptr = m_Head;
            SLLNode* otherPtr = other.m_Head;
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
        bool operator!=(const SinglyLinkedList<T>& other) const noexcept {
            return !(*this == other);
        }


        /**
         * Performs an user defined operation on each element in the array.
         * @param func Lambda that returns void and takes in a T reference.
         */
        void foreach(const std::function<void(T&)>& func) noexcept {
            SLLNode* ptr = m_Head;
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
            SLLNode* ptr = m_Head;
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
                m_Head = new SLLNode(data);
            } else {
                auto* newHead = new SLLNode(data, m_Head);
                m_Head = newHead;
            }
            m_Size++;
        }

        /**
        * Insert element to the front of the linked list
        * @param data Element to insert
        */
        void appendFront(T&& data) noexcept {
            if (isEmpty()) {
                m_Head = new SLLNode(std::move(data));
            } else {
                auto* newHead = new SLLNode(std::move(data), m_Head);
                m_Head = newHead;
            }
            m_Size++;
        }

        /**
         * Construct an element in-place at the front of the linked list
         * @tparam Ts
         * @param args
         * @return
         */
        template<typename... Ts>
        T& emplaceFront(Ts&& ... args) noexcept {
            if (isEmpty()) {
                m_Head = new SLLNode(nullptr, std::forward<Ts>(args)...);
            } else {
                auto* newHead = new SLLNode(m_Head, std::forward<Ts>(args)...);
                m_Head = newHead;
            }
            m_Size++;
            return m_Head->m_Data;
        }

        /**
         * Insert element to the back of the linked list
         * @param data Element to insert
         */
        void appendBack(const T& data) noexcept {
            if (isEmpty()) {
                m_Head = new SLLNode(data);
            } else {
                SLLNode* lastPtr = ptrTo(m_Size - 1);
                lastPtr->m_Next = new SLLNode(data);
            }
            m_Size++;
        }

        /**
         * Insert element to the back of the linked list
         * @param data Element to insert
         */
        void appendBack(T&& data) noexcept {
            if (isEmpty()) {
                m_Head = new SLLNode(std::move(data));
            } else {
                SLLNode* lastPtr = ptrTo(m_Size - 1);
                lastPtr->m_Next = new SLLNode(std::move(data));
            }
            m_Size++;
        }

        /**
         * Construct an element in-place at the back of the linked list
         * @tparam Ts
         * @param args
         * @return
         */
        template<typename ... Ts>
        T& emplaceBack(Ts&& ... args) noexcept {
            if (isEmpty()) {
                m_Head = new SLLNode(nullptr, std::forward<Ts>(args)...);
                m_Size++;
                return m_Head->m_Data;
            } else {
                SLLNode* lastPtr = ptrTo(m_Size - 1);
                lastPtr->m_Next = new SLLNode(nullptr, std::forward<Ts>(args)...);
                m_Size++;
                return lastPtr->m_Next->m_Data;
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
        void insertAt(std::size_t index, const T& data) {
            if (index > m_Size || m_Size < 0) {
                throw std::out_of_range("Index out of range");
            }
            if (index == m_Size) {
                appendBack(data);
            } else if (index == 0) {
                appendFront(data);
            } else {
                SLLNode* newNode = new SLLNode(data);
                SLLNode* tempPtr = m_Head;
                for (int i = 0; i < index - 1; i++) {
                    tempPtr = tempPtr->m_Next;
                }
                newNode->m_Next = tempPtr->m_Next;
                tempPtr->m_Next = newNode;
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
            if (index > m_Size || m_Size < 0) {
                throw std::out_of_range("Index out of range");
            }
            if (index == m_Size) {
                appendBack(std::move(data));
            } else if (index == 0) {
                appendFront(std::move(data));
            } else {
                SLLNode* newNode = new SLLNode(std::move(data));
                SLLNode* tempPtr = m_Head;
                for (int i = 0; i < index - 1; i++) {
                    tempPtr = tempPtr->m_Next;
                }
                newNode->m_Next = tempPtr->m_Next;
                tempPtr->m_Next = newNode;
                m_Size++;
            }
        }

        /**
         * Construct an element in-place at the specified index of the linked list.
         * If index supplied is 0, the element is inserted at the front.
         * If index supplied is the size of the linked ist, the element is inserted at the back.
         * If index supplied is within the bounds, the new element will push the element at the current index
         * one index backwards.
         * @tparam Ts
         * @param index
         * @param args
         * @return
         */
        template<typename ... Ts>
        T& emplaceAt(std::size_t index, Ts&& ... args) {
            if (index > m_Size || m_Size < 0) {
                throw std::out_of_range("Index out of range");
            }
            if (index == m_Size) {
                emplaceBack(std::forward<Ts>(args)...);
            } else if (index == 0) {
                emplaceFront(std::forward<Ts>(args)...);
            } else {
                auto* newNode = new SLLNode(nullptr, std::forward<Ts>(args)...);
                SLLNode* tempPtr = m_Head;
                for (int i = 0; i < index - 1; i++) {
                    tempPtr = tempPtr->m_Next;
                }
                newNode->m_Next = tempPtr->m_Next;
                tempPtr->m_Next = newNode;
                m_Size++;
            }
        }

        /**
         * Removes first element in O(1). Nothing happens if linked list is already empty
         */
        void detachFront() noexcept {
            if (!isEmpty()) {
                SLLNode* ptr = m_Head->m_Next;
                delete m_Head;
                m_Head = ptr;
                m_Size--;
            }
        }

        /**
         * Removes last element in O(1). Nothing happens is linked list is already empty
         */
        void detachBack() noexcept {
            if (isEmpty()) {
                return;
            }
            if (m_Size == 1) {
                delete m_Head;
                m_Head = nullptr;
            } else {
                SLLNode* ptr = ptrTo(m_Size - 2);
                delete ptr->m_Next;
                ptr->m_Next = nullptr;
            }
            m_Size--;
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
                detachFront();
            } else if (index == m_Size - 1) {
                detachBack();
            } else {
                SLLNode* tempPtr = m_Head;
                for (int i = 0; i < index - 1; i++) {
                    tempPtr = tempPtr->m_Next;
                }
                SLLNode* nodeToRemove = tempPtr->m_Next;
                tempPtr->m_Next = nodeToRemove->m_Next;
                delete nodeToRemove;
                m_Size--;
            }
        }

        /**
         * Remove all elements in the linked list that matches the supplied element
         * @param element
         * @return Number of removed elements
         */
        std::size_t remove(const T& element) noexcept {
            if (isEmpty()) {
                return 0;
            }
            std::size_t count = 0;
            while (!isEmpty()) {
                if (front() == element) {
                    detachFront();
                    m_Size--;
                    count++;
                } else {
                    break;
                }
            }
            if (!isEmpty()) {
                SLLNode* prevNode = m_Head;
                SLLNode* currNode = m_Head->nextNode;
                while (currNode != nullptr) {
                    if (currNode->data == element) {
                        prevNode->nextNode = currNode->nextNode;
                        delete currNode;
                        currNode = prevNode->nextNode;
                        m_Size--;
                        count++;
                    } else {
                        prevNode = currNode;
                        currNode = currNode->nextNode;
                    }
                }
            }
            return count;
        }

        /**
         * Removes all elements in O(n). Size returns to 0.
         */
        void clear() noexcept {
            if (!isEmpty()) {
                SLLNode* next = m_Head->m_Next;
                delete m_Head;
                while (next != nullptr) {
                    m_Head = next;
                    next = next->m_Next;
                    delete m_Head;
                }
                m_Size = 0;
                m_Head = nullptr;
            }
        }

        /**
         * Destructor
         */
        ~SinglyLinkedList() {
            clear();
        }

    };

}

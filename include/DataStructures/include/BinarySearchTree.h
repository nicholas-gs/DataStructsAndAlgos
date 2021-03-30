//
// Created by Nicholas on 3/12/2020.
//

#pragma once

#include <utility>
#include <cstddef>
#include <optional>
#include <functional>
#include <list>
#include <exception>

namespace wtl {

    namespace impl {

        /**
         * Node for binary search tree
         * @tparam K Type of comparator value
         * @tparam V Type of data stored in each node
         */
        template<typename K, typename V>
        struct BSTNode {

            /// Key that is used as the comparator value in the BST
            K m_Key;

            /// Item stored in each node
            V m_Value;

            /// Pointer to the left child node
            BSTNode<K, V>* m_leftChild = nullptr;

            /// Pointer to the right child node
            BSTNode<K, V>* m_rightChild = nullptr;

            /**
             * Constructor
             * @param key
             * @param value
             * @param leftChild
             * @param rightChild
             */
            BSTNode(const K& key, const V& value, BSTNode<K, V>* leftChild = nullptr,
                    BSTNode<K, V>* rightChild = nullptr)
                    : m_Key(key), m_Value(value), m_leftChild(leftChild), m_rightChild(rightChild) {}


            /**
             *
             * @param key
             * @param value
             * @param leftChild
             * @param rightChild
             */
            BSTNode(const K& key, V&& value, BSTNode<K, V>* leftChild = nullptr, BSTNode<K, V>* rightChild = nullptr)
                    : m_Key(key), m_Value(std::move(value)), m_leftChild(leftChild),
                      m_rightChild(rightChild) {}

            /**
             * Constructor for constructing value in-place
             * @tparam T
             * @tparam Ts
             * @param leftChild
             * @param rightChild
             * @param key
             * @param args
             */
            template<typename ... Ts>
            BSTNode(const K& key, BSTNode<K, V>* leftChild, BSTNode<K, V>* rightChild, Ts&& ... args)
                    : m_Key(key), m_Value(std::forward<Ts>(args)...), m_leftChild(leftChild),
                      m_rightChild(rightChild) {}

        };
    }

    /**
     * Binary Search Tree (BST).
     * To insert an element into the binary search tree, pass in the element of type V
     * as well as an associated key value of type K.
     * Key values have to be unique, while elements of type V do not have to be.
     * @tparam K
     * @tparam V
     */
    template<typename K, typename V>
    class BinarySearchTree {
    private:

        typedef wtl::impl::BSTNode<K, V> Node;

        /// Number of elements in the BST
        std::size_t m_Size = 0;

        /// Pointer to the root node of the BST
        Node* m_Root = nullptr;

        /**
         * Recursive post-order traversal of BST and delete each node
         * @param root Root node of sub tree
         */
        void clearNodes(Node* const root) noexcept {
            if (root == nullptr) {
                return;
            }
            clearNodes(root->m_leftChild);
            clearNodes(root->m_rightChild);
            delete root;
            m_Size--;
        }

        /**
         * Remove a node from the BST and repair the tree.
         * There are 3 possible situations: 1. Node to be removed is a leaf node.
         * 2. Node to be removed has only 1 child. 3. Node to be removed has 2 child nodes
         * @param currNode Node to be removed from the BST
         * @param prevNode Parent node of the node to be removed
         */
        void removeNode(Node* const currNode, Node* const prevNode) noexcept {
            // Does the node to remove have a left child?
            bool hasLeftChild = currNode->m_leftChild != nullptr;
            // Does the node to remove have a right child?
            bool hasRightChild = currNode->m_leftChild != nullptr;
            // Is the node to remove the left child of its parent?
            bool isLeftChild = currNode == (prevNode->m_leftChild);

            // Node to remove is a leaf node
            if (!hasLeftChild && !hasRightChild) {
                if (isLeftChild) {
                    prevNode->m_leftChild = nullptr;
                } else {
                    prevNode->m_rightChild = nullptr;
                }
            }
                // Node to remove only has a left child
            else if (hasLeftChild & !hasRightChild) {
                if (isLeftChild) {
                    prevNode->m_leftChild = currNode->m_leftChild;
                } else {
                    prevNode->m_rightChild = currNode->m_leftChild;
                }
            }
                // Node to remove only has a right child
            else if (!hasLeftChild && hasRightChild) {
                if (isLeftChild) {
                    prevNode->m_leftChild = currNode->m_rightChild;
                } else {
                    prevNode->m_rightChild = currNode->m_rightChild;
                }
            }
                // Node to remove has both left and right children.
                // Take either the biggest key of the left sub tree, or smallest key of the right sub tree.
            else {
                Node* prevSub = currNode;
                Node* currSub = currNode->m_rightChild;
                while (currSub->m_leftChild != nullptr) {
                    prevSub = currSub;
                    currSub = currSub->m_leftChild;
                }
                currNode->m_Key = currSub->m_Key;
                currNode->m_Value = currSub->m_Value;
                if (prevSub != currNode) {
                    prevSub->m_leftChild = currSub->m_rightChild;
                } else {
                    currNode->m_rightChild = nullptr;
                }
                delete currSub;
                return;
            }
            delete currNode;
        }

        /**
         * Recursive in-order traversal of BST. Call provided function in-order.
         * @param root
         * @param func Function that takes in a V&
         */
        void inOrderTraversal(Node* const root, const std::function<void(V&)>& func) noexcept {
            if (root == nullptr) {
                return;
            }
            inOrderTraversal(root->m_leftChild, func);
            func(root->m_Value);
            inOrderTraversal(root->m_rightChild, func);
        }

        /**
         * Recursive in-order traversal of BST. Call provided function in-order.
         * @param root
         * @param func Function that takes in a const V&
         */
        void inOrderTraversal(const Node* const root, const std::function<void(const V&)>& func) const noexcept {
            if (root == nullptr) {
                return;
            }
            inOrderTraversal(root->m_leftChild, func);
            func(root->m_Value);
            inOrderTraversal(root->m_rightChild, func);
        }

        /**
         * Recursive in-order traversal of BST. Call provided function in-order.
         * @param root
         * @param func Function that takes in a const K&
         */
        void inOrderTraversal(const Node* const root, const std::function<void(const K&)>& func) const noexcept {
            if (root == nullptr) {
                return;
            }
            inOrderTraversal(root->m_leftChild, func);
            func(root->m_Key);
            inOrderTraversal(root->m_rightChild, func);
        }

        /**
         * Insert key value pair into BST. If key already exists, then pointer to the node is returned. But value
         * is not updated
         * @param key
         * @param value
         * @return
         */
        Node* insertPrivate(const K& key, const V& value) noexcept {
            if (isEmpty()) {
                m_Root = new Node(key, value);
                m_Size++;
                return nullptr;
            } else {
                Node* currNode = m_Root;

                while (true) {
                    // New node needs to go to the left sub tree of current node
                    if (key < currNode->m_Key) {
                        // Current node has no left child node
                        if (currNode->m_leftChild == nullptr) {
                            currNode->m_leftChild = new Node(key, value);
                            m_Size++;
                            return nullptr;
                        } else {
                            currNode = currNode->m_leftChild;
                        }
                    }
                        // New node needs to go to the right sub tree of current node
                    else if (key > currNode->m_Key) {
                        // Current node has no right child node
                        if (currNode->m_rightChild == nullptr) {
                            currNode->m_rightChild = new Node(key, value);
                            m_Size++;
                            return nullptr;
                        } else {
                            currNode = currNode->m_rightChild;
                        }
                    }
                        // Found a node with duplicate key, so return pointer
                    else {
                        return currNode;
                    }
                }
            }
        }

        /**
         * Insert key value pair into BST. If key already exists, then pointer to the node is returned. But value
         * is not updated
         * @param key
         * @param value
         * @return
         */
        Node* insertPrivate(const K& key, V&& value) noexcept {
            if (isEmpty()) {
                m_Root = new Node(key, std::move(value));
                m_Size++;
                return nullptr;
            } else {
                Node* currNode = m_Root;

                while (true) {
                    // New node needs to go to the left sub tree of current node
                    if (key < currNode->m_Key) {
                        // Current node has no left child node
                        if (currNode->m_leftChild == nullptr) {
                            currNode->m_leftChild = new Node(key, std::move(value));
                            m_Size++;
                            return nullptr;
                        } else {
                            currNode = currNode->m_leftChild;
                        }
                    }
                        // New node needs to go to the right sub tree of current node
                    else if (key > currNode->m_Key) {
                        // Current node has no right child node
                        if (currNode->m_rightChild == nullptr) {
                            currNode->m_rightChild = new Node(key, std::move(value));
                            m_Size++;
                            return nullptr;
                        } else {
                            currNode = currNode->m_rightChild;
                        }
                    }
                        // Found a node with duplicate key, so return pointer
                    else {
                        return currNode;
                    }
                }
            }
        }

        /**
          * Insert key value pair into BST with value created in-place.
          * If key already exists, then pointer to the node is returned. But value is not updated
          * @tparam Ts
          * @param key
          * @param args
          * @return
          */
        template<typename ... Ts>
        std::pair<bool, Node*> emplacePrivate(const K& key, Ts&& ... args) noexcept {
            if (isEmpty()) {
                m_Root = new Node(key, nullptr, nullptr, std::forward<Ts>(args)...);
                m_Size++;
                return {true, m_Root};
            } else {
                Node* currNode = m_Root;

                while (true) {
                    // New node needs to go to the left sub tree of current node
                    if (key < currNode->m_Key) {
                        // Current node has no left child node
                        if (currNode->m_leftChild == nullptr) {
                            currNode->m_leftChild = new Node(key, nullptr, nullptr,
                                                             std::forward<Ts>(args)...);
                            m_Size++;
                            return {true, currNode->m_leftChild};
                        } else {
                            currNode = currNode->m_leftChild;
                        }
                    }
                        // New node needs to go to the right sub tree of current node
                    else if (key > currNode->m_Key) {
                        // Current node has no right child node
                        if (currNode->m_rightChild == nullptr) {
                            currNode->m_rightChild = new Node(key, nullptr, nullptr,
                                                              std::forward<Ts>(args)...);
                            m_Size++;
                            return {true, currNode->m_rightChild};
                        } else {
                            currNode = currNode->m_rightChild;
                        }
                    }
                        // Found a node with duplicate key, so return pointer
                    else {
                        return {false, currNode};
                    }
                }
            }
        }

    public:

        typedef std::optional<std::reference_wrapper<V>> BSTValue;
        typedef std::optional<std::reference_wrapper<const V>> BSTValueConst;

        /**
         * Constructor
         */
        BinarySearchTree() = default;

        /**
         * Copy constructor
         * @param other
         */
        BinarySearchTree(const BinarySearchTree<K, V>& other) noexcept {
            // TODO
        }

        /**
        * Copy assignment
        * @param other
        * @return
        */
        BinarySearchTree<K, V>& operator=(const BinarySearchTree<K, V>& other) noexcept {
            if (this != &other) {
                clear();
                // TODO
            }
            return *this;
        }

        /**
         * Move constructor
         * @param other
         */
        BinarySearchTree(BinarySearchTree<K, V>&& other) noexcept {
            m_Root = other.m_Root;
            other.m_Root = nullptr;
            m_Size = other.m_Size;
            other.m_Size = 0;
        }

        /**
         * Move assignment
         * @param other
         * @return
         */
        BinarySearchTree<K, V>& operator=(BinarySearchTree<K, V>&& other) noexcept {
            if (this != &other) {
                clear();
                m_Root = other.m_Root;
                other.m_Root = nullptr;
                m_Size = other.m_Size;
                other.m_Size = 0;
            }
            return *this;
        }

        /**
         * Get the number of elements in the BST
         * @return
         */
        [[nodiscard]] std::size_t getSize() const noexcept {
            return m_Size;
        }

        /**
         * Check if BST is empty
         * @return true if empty, false if not empty
         */
        [[nodiscard]] bool isEmpty() const noexcept {
            return m_Size == 0;
        }

        /**
         * Retrieve all the keys in the binary search tree in in-order traversal order.
         * The keys will be in ascending order.
         * @param listPtr Pointer to a std::list<K>
         */
        void getKeys(std::list<K>& list) const noexcept {
            list.clear();
            inOrderTraversal(m_Root, [&](const K& key) { list.push_back(key); });
        }

        /**
         * Performs an user defined operation on each element in the BST in in-order traversal.
         * @param func Function that takes a V&
         */
        void foreach(const std::function<void(V&)>& func) noexcept {
            inOrderTraversal(m_Root, func);
        }

        /**
         * Performs an user defined operation on each element in the BST in in-order traversal.
         * @param func Function that takes a const V&
         */
        void foreach(const std::function<void(const V&)>& func) const noexcept {
            inOrderTraversal(m_Root, func);
        }

        /**
         * Insert an element into the BST. If the key value already exists, nothing happens.
         * @note Use insertOrReplace to replace value if key already exists
         * @param key
         * @param value
         * @return true if key-value pair was inserted. false if nothing was updated
         */
        bool insert(const K& key, const V& value) noexcept {
            if (insertPrivate(key, value) == nullptr) {
                return true;
            }
            return false;
        }

        /**
         * Insert an element into the BST. If the key value already exists, nothing happens.
         * @note Use insertOrReplace to replace value if key already exists
         * @param key
         * @param value
         * @return true if key-value pair was inserted. false if nothing was updated
         */
        bool insert(const K& key, V&& value) noexcept {
            if (insertPrivate(key, std::move(value)) == nullptr) {
                return true;
            }
            return false;
        }

        /**
         * Insert an element into the BST, or update the value if the key already exists.
         * @param key
         * @param value
         * @return true if key-value pair was inserted, false if key already existed and value updated
         */
        bool insertOrReplace(const K& key, const V& value) noexcept {
            Node* ptr = insertPrivate(key, value);
            if (ptr == nullptr) {
                return true;
            } else {
                ptr->m_Value = value;
                return false;
            }
        }

        /**
         * Insert an element into the BST, or update the value if the key already exists.
         * @param key
         * @param value
         * @return true if key-value pair was inserted, false if key already existed and value updated
         */
        bool insertOrReplace(const K& key, V&& value) noexcept {
            Node* ptr = insertPrivate(std::move(key), std::move(value));
            if (ptr == nullptr) {
                return true;
            } else {
                ptr->m_Value = std::move(value);
                return false;
            }
        }

        /**
          * Construct new value in-place. If key value already exists, then nothing is updated.
          * @tparam Ts
          * @param key
          * @param args
          * @return A pair that containing a bool and reference. bool is true if insertion occurred and reference to the
          * newly created object of type V. bool is false if insertion did not occur and reference to the already
          * existing object.
          */
        template<typename ... Ts>
        std::pair<bool, std::reference_wrapper<V>> emplace(K&& key, Ts&& ... args) noexcept {
            std::pair<bool, Node*> result = emplacePrivate(key, std::forward<Ts>(args)...);
            Node* ptr = result.second;
            return {result.first, ptr->m_Value};
        }

        /**
         * Retrieve an element from the BST
         * @note BSTValue is a typedef for a std::optional of type V& template specialization.
         * @param key
         * @return
         */
        [[nodiscard]] BSTValue get(const K& key) noexcept {
            Node* currNode = m_Root;
            while (currNode != nullptr) {
                if (key == currNode->m_Key) {
                    return currNode->m_Value;
                } else if (key < currNode->m_Key) {
                    currNode = currNode->m_leftChild;
                } else {
                    currNode = currNode->m_rightChild;
                }
            }
            return std::nullopt;
        }

        /**
         * Retrieve an element from the BST
         * @note BSTValue is a typedef for a std::optional of type const V& template specialization.
         * @param key
         * @return
         */
        [[nodiscard]] BSTValueConst get(const K& key) const noexcept {
            Node* currNode = m_Root;
            while (currNode != nullptr) {
                if (key == currNode->m_Key) {
                    return currNode->m_Value;
                } else if (key < currNode->m_Key) {
                    currNode = currNode->m_leftChild;
                } else {
                    currNode = currNode->m_rightChild;
                }
            }
            return std::nullopt;
        }

        /**
         * Remove an element from the BST specified using its key
         * @param key
         * @return true if specified key was found and element remove, false if not.
         */
        bool remove(const K& key) noexcept {
            // Pointer to the parent node of the node to remove.
            Node* prevNode = nullptr;
            // Pointer to the node to be removed.
            Node* currNode = m_Root;
            bool removed = false;

            while (currNode != nullptr) {
                if (key == currNode->m_Key) {
                    removed = true;
                    removeNode(currNode, prevNode);
                    break;
                } else if (key < currNode->m_Key) {
                    prevNode = currNode;
                    currNode = currNode->m_leftChild;
                } else {
                    prevNode = currNode;
                    currNode = currNode->m_rightChild;
                }
            }
            if (removed) {
                m_Size--;
            }
            return removed;
        }

        /**
         * Clear the BST by deleting each element. Size reset to 0.
         */
        void clear() noexcept {
            clearNodes(m_Root);
        }

        /**
         * Destructor
         */
        ~BinarySearchTree() {
            clearNodes(m_Root);
        }

    };

}

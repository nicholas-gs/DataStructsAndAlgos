//
// Created by Nicholas on 10/12/2020.
//

#pragma once

#include "impl/SortingAlgorithmsImpl.h"
#include <vector>
#include <cstddef>

namespace wtl {

    namespace impl {

        /**
         * Node for the binary search tree used in the tree sort algorithm.
         * The count instance variable keeps track the number of times
         * a certain value is present in the BST.
         * @tparam T
         */
        template<typename T>
        class TreeSortNode {
        public:

            // Data stored in the node
            T m_Value;

            // Pointer to the left child
            TreeSortNode<T>* m_LeftChild;

            // Pointer to the right child
            TreeSortNode<T>* m_RightChild;

            // Number of instance of the value m_Value
            std::size_t m_Count = 1;

            // explicit TreeSortNode(const T& value, TreeSortNode<T>* leftChild = nullptr, TreeSortNode<T>* rightChild= nullptr)
            //        : m_Value(value), m_LeftChild(leftChild), m_RightChild(rightChild) {}


            explicit TreeSortNode(T&& value, TreeSortNode<T>* leftChild = nullptr,
                                  TreeSortNode<T>* rightChild = nullptr)
                    : m_Value(std::move(value)), m_LeftChild(leftChild), m_RightChild(rightChild) {}

            TreeSortNode(const TreeSortNode<T>& other) = delete;

            TreeSortNode<T>& operator=(const TreeSortNode<T>& other) = delete;

            TreeSortNode(TreeSortNode<T>&& other) = delete;

            TreeSortNode<T>& operator=(TreeSortNode<T>&& other) = delete;
        };
    }

    /**
     * Sort elements using a binary search tree.
     * @tparam T
     * @tparam Comparator
     */
    template<typename T, typename Comparator = impl::sort::StandardComparator<T>>
    class TreeSort {
    private:

        typedef wtl::impl::TreeSortNode<T> Node;

        inline static Node* m_Root = nullptr;

        inline static std::size_t index = 0;

        static void insert(T& element, const Comparator& comparator) {
            if (m_Root == nullptr) {
                Node* newNode = new Node(std::move(element));
                m_Root = newNode;
            } else {
                Node* currNode = m_Root;
                bool inserted = false;
                while (!inserted) {
                    // element == currNode->m_Value
                    if (!comparator(currNode->m_Value, element) && !comparator(element, currNode->m_Value)) {
                        currNode->m_Count++;
                        inserted = true;
                        // element < currNode->m_Value
                    } else if (comparator(element, currNode->m_Value)) {
                        if (currNode->m_LeftChild == nullptr) {
                            currNode->m_LeftChild = new Node(std::move(element));
                            inserted = true;
                        } else {
                            currNode = currNode->m_LeftChild;
                        }
                    } else {
                        if (currNode->m_RightChild == nullptr) {
                            currNode->m_RightChild = new Node(std::move(element));
                            inserted = true;
                        } else {
                            currNode = currNode->m_RightChild;
                        }
                    }
                }
            }
        }

        static void inOrderTraversal(std::vector<T>& vector, Node* root) {
            if (root == nullptr) {
                return;
            }
            inOrderTraversal(vector, root->m_LeftChild);
            for (std::size_t i = 0; i < root->m_Count; i++) {
                if (i + 1 == root->m_Count) {
                    vector[index] = std::move(root->m_Value);
                } else {
                    vector[index] = root->m_Value;
                }
                index++;
            }
            inOrderTraversal(vector, root->m_RightChild);
        }

        static void clear(Node* root) {
            if (root == nullptr) {
                return;
            }
            clear(root->m_LeftChild);
            clear(root->m_RightChild);
            delete root;
        }

    public:

        /**
         * Sort elements in a vector in-place.
         * Elements are compared using the supplied comparison function compare.
         * @tparam Comparator
         * @param vector
         * @param comparator
         */
        static void sort(std::vector<T>& vector) {
            Comparator comparator;
            if (vector.size() < 2) {
                return;
            }
            m_Root = nullptr;
            index = 0;
            for (T& element : vector) {
                insert(element, comparator);
            }
            inOrderTraversal(vector, m_Root);
            clear(m_Root);
            m_Root = nullptr;
        }
    };

}

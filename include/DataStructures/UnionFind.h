//
// Created by Nicholas on 14/12/2020.
//

#pragma once

#include <cstddef>
#include <stdexcept>

namespace wtl {

    /**
     * Union find (dis-joint set) data structure.
     * Path compression is carried out to help ensure amortised O(1) for join, find and check if connected operations.
	 * Join operation is carried out by rank.
     */
    class UnionFind {
    private:

        // Graph representation as an int array.
        // The value inside each array index is the parent node of that index.
        std::size_t* m_Graph = nullptr;

        // Keep track of the rank of each parent node. If a node is isolated, it has a rank of 0.
        std::size_t* m_Rank = nullptr;

        // Number of nodes
        std::size_t m_Size = 0;

        // Number of disjoint sets.
        // Initially, the number of disjoint sets is equal to the number of elements since each element is isolated.
        // The minimum number of disjoint set is 1, which is when every node is connected.
        std::size_t m_NumberOfSets = 0;

        /**
         * Do path compression. Starting at an initial node, we traverse to its root node while pointing all nodes
         * along the path to the root node.
         * @param initialNode
         * @param rootNode
         */
        void pathCompression(std::size_t initialNode, std::size_t rootNode) noexcept {
            while (m_Graph[initialNode] != rootNode) {
                std::size_t next = m_Graph[initialNode];
                m_Graph[initialNode] = rootNode;
                initialNode = next;
            }
        }

        /**
         * Initialise the Union Find data structure.
         * @param size
         */
        void init(std::size_t size) noexcept {
            m_Size = size;
            m_NumberOfSets = size;
            m_Graph = new std::size_t[size];
            for (std::size_t i = 0; i < size; i++) {
                m_Graph[i] = i;
            }
            m_Rank = new std::size_t[size]{0};
        }

        /**
         * Check if an index is out of bounds.
         * @param index
         * @return
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
         * @param size Number of nodes in the UnionFind
         */
        explicit UnionFind(std::size_t size) {
            if (size <= 0) {
                throw std::invalid_argument("Size of union find cannot be less than 1");
            }
            init(size);
        }

        /**
         * Copy constructor
         * @param other
         */
        UnionFind(const UnionFind& other) noexcept {
            m_Size = other.m_Size;
            m_NumberOfSets = other.m_NumberOfSets;
            m_Graph = new std::size_t[m_Size];
            m_Rank = new std::size_t[m_Size];
            for (std::size_t i = 0; i < m_Size; i++) {
                m_Graph[i] = other.m_Graph[i];
                m_Rank[i] = other.m_Rank[i];
            }
        }

        /**
         * Copy assignment
         * @param other
         * @return
         */
        UnionFind& operator=(const UnionFind& other) noexcept {
            if (this != &other) {
                delete[] m_Graph;
                delete[] m_Rank;
                m_Graph = new std::size_t[m_Size];
                m_Rank = new std::size_t[m_Size];
                for (std::size_t i = 0; i < m_Size; i++) {
                    m_Graph[i] = other.m_Graph[i];
                    m_Rank[i] = other.m_Rank[i];
                }
            }
            return *this;
        }

        UnionFind(UnionFind&& other) = delete;

        UnionFind& operator=(UnionFind&& other) = delete;

        /**
         * Get the number of nodes.
         * @return
         */
        [[nodiscard]] std::size_t getSize() const noexcept {
            return m_Size;
        }

        /**
         * Get the number of disjoint sets. The range of values are [1, size].
         * @return
         */
        [[nodiscard]] std::size_t getSets() const noexcept {
            return m_NumberOfSets;
        }

        /**
         * Find which set a node belongs to.
         * @param node
         * @return Parent node of the specified node
         */
        [[nodiscard]] std::size_t belongs(std::size_t node) {
            if (outOfBounds(node)) {
                throw std::invalid_argument("Invalid node");
            }
            std::size_t o = node;
            while (node != m_Graph[node]) {
                node = m_Graph[node];
            }
            pathCompression(o, node);
            return node;
        }

        /**
         * Union two nodes' group together.
         * Union does not occur if both nodes have the same parent node. If union does occur, then the union occurs by RANK.
		 * That means the shorter tree's parent node is attached to the taller tree's parent node and the taller tree's parent
		 * node is the final tree's parent node. Thus, the resulting tree is no taller than the originals unless they were of
		 * equal height, in which case the resulting tree is taller by one node.
		 * If both are of equal height, then node1's parent becomes the overall parent node.
         * @param node1
         * @param node2
         * @return
         */
        bool join(std::size_t node1, std::size_t node2) {
            if (outOfBounds(node1) || outOfBounds(node2)) {
                throw std::invalid_argument("Invalid node");
            }
            std::size_t node1parent = node1;
            std::size_t node2parent = node2;
            // Find the parent node of node 1
            while (node1parent != m_Graph[node1parent]) {
                node1parent = m_Graph[node1parent];
            }
            // Find the parent node of node 2
            while (node2parent != m_Graph[node2parent]) {
                node2parent = m_Graph[node2parent];
            }
            // Both nodes belong to the same group, so don't carry out union
            if (node1parent == node2parent) {
                return false;
            }
            // Carry out union
            std::size_t node1Rank = m_Rank[node1parent];
            std::size_t node2Rank = m_Rank[node2parent];

            if (node1Rank == node2Rank) {
                m_Graph[node2parent] = node1parent;
                m_Rank[node1parent]++;
            } else if (node1Rank < node2Rank) {
                m_Graph[node1parent] = node2parent;
            } else {
                m_Graph[node2parent] = node1parent;
            }
            --m_NumberOfSets;
            return true;
        }

        /**
         * Check if 2 nodes are connected
         * @param node1
         * @param node2
         * @return True if they are connected
         */
        [[nodiscard]] bool connected(std::size_t node1, std::size_t node2) {
            if (outOfBounds(node1) || outOfBounds(node2)) {
                throw std::invalid_argument("Invalid node");
            }
            return belongs(node1) == belongs(node2);
        }

        /**
         * Destructor
         */
        ~UnionFind() {
            delete[] m_Graph;
            delete[] m_Rank;
        }

    };

}

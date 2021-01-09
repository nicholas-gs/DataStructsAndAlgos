//
// Created by Nicholas on 4 Jan 2021.
//

#pragma once

#include <cstddef>
#include <cstddef>
#include <memory>
#include <vector>

#include "../DataStructures/Stack.h"
#include "SimpleGraph_Unweighted.h"

namespace wtl {

    /**
     * Find the strongly connected components (SCC) of a directed graph represented using an adjacency list
     * in O(V+E) time complexity using Tarjan's algorithm.
     *
	 * This algorithm allows for O(1) look-up to check if two vertices belong to the same strong component.
     *
	 * Two vertices are strongly connected if it is possible to traverse between the vertices in both direction.
	 * A strongly connected component is a maximal subset of vertices of a graph for which
     * any two vertices are strongly connected.
     */
    class Tarjan {
    private:

        using DiGraph = wtl::SimpleGraph_Unweighted<true>;
        using Stack = wtl::Stack<std::size_t>;

        /// Number of vertices
        std::size_t m_Size;

        /// Number of strongly connected components in the graph
        std::size_t numberOfSCC = 0;

        const std::size_t NONE;

        /// Array to keep track of ids assigned to vertices during traversal
        std::unique_ptr<std::size_t[]> traversalIds;

        /// Array to keep track of which strongly connected component the vertices belong to.
        /// Used to store low-link values during Tarjan's algorithm
        std::unique_ptr<std::size_t[]> sccIndex;

        /// Track track of which vertices on currently on the stack
        std::unique_ptr<bool[]> onStack;

        /// Current Traversal id assigned during Tarjan's algorithm
        std::size_t traversalId = 0;

        [[nodiscard]] inline bool outOfBounds(std::size_t v) const noexcept {
            return v >= m_Size;
        }

        void performTarjan(const DiGraph& diGraph) {
            Stack visitedStack;
            for (std::size_t i = 0; i < m_Size; i++) {
                if (traversalIds[i] == NONE) {
                    dfs(diGraph, i, visitedStack);
                }
            }
        }

        void dfs(const DiGraph& diGraph, std::size_t v, Stack& visitedStack) {
            traversalIds[v] = traversalId;
            sccIndex[v] = traversalId;
            traversalId++;
            onStack[v] = true;
            visitedStack.push(v);

            for (std::size_t w : diGraph.adjacent(v)) {
                if (traversalIds[w] == NONE) {
                    dfs(diGraph, w, visitedStack);
                }
                if (onStack[w]) {
                    sccIndex[v] = (sccIndex[w] < sccIndex[v]) ? sccIndex[w] : sccIndex[v];
                }
            }
            if (traversalIds[v] == sccIndex[v]) {
                for (std::size_t node = visitedStack.peek();; node = visitedStack.peek()) {
                    onStack[node] = false;
                    visitedStack.pop();
                    sccIndex[node] = traversalIds[v];
                    if (node == v) {
                        break;
                    }

                }
                numberOfSCC++;
            }
        }

    public:

        /**
         * Constructor
         * @param diGraph
         */
        Tarjan(const DiGraph& diGraph)
                : m_Size(diGraph.vertex()), NONE(diGraph.vertex()),
                  traversalIds(std::make_unique<std::size_t[]>(m_Size)),
                  sccIndex(std::make_unique<std::size_t[]>(m_Size)),
                  onStack(std::make_unique<bool[]>(m_Size)) {
            for (std::size_t i = 0; i < m_Size; i++) {
                onStack[i] = false;
                traversalIds[i] = NONE;
            }
            performTarjan(diGraph);
        }

        /**
         * Copy constructor
         * @param other
         */
        Tarjan(const Tarjan& other) : m_Size(other.m_Size), NONE(other.NONE),
                                      sccIndex(std::make_unique<std::size_t[]>(m_Size)) {
            numberOfSCC = other.numberOfSCC;
            m_Size = other.m_Size;
            for (std::size_t i = 0; i < m_Size; i++) {
                sccIndex[i] = other.sccIndex[i];
            }
        }

        /**
         * Copy assignment
         * @param other
         */
        Tarjan& operator=(const Tarjan& other) {
            if (this != &other) {
                if (m_Size != other.m_Size) {
                    throw std::runtime_error("Different number of vertices");
                }
                numberOfSCC = other.numberOfSCC;
                for (std::size_t i = 0; i < m_Size; i++) {
                    sccIndex[i] = other.sccIndex[i];
                }
            }
            return *this;
        }

        /**
         * Get the number of connected components.
         * @return
         */
        [[nodiscard]] std::size_t getCount() const noexcept {
            return numberOfSCC;
        }

        /**
         * Check if 2 vertices are strongly connected.
         * @param v
         * @param w
         * @return true if strongly connected, false if not
         */
        [[nodiscard]] bool stronglyConnected(std::size_t v, std::size_t w) const {
            if (outOfBounds(v) || outOfBounds(w)) {
                throw std::invalid_argument("Invalid vertex");
            }
            return sccIndex[v] == sccIndex[w];
        }

        /**
         * Get the SCC id that a vertex belongs to.
         * @param v
         * @return
         */
        [[nodiscard]] std::size_t belongs(std::size_t v) const {
            if (outOfBounds(v)) {
                throw std::invalid_argument("Invalid vertex");
            }
            return sccIndex[v];
        }

        /**
         * Get all vertices in specified SCC id.
         * @param ccID ID of the CC
         * @return
         */
        [[nodiscard]] std::vector<std::size_t> set(std::size_t ccID) const {
            std::vector<std::size_t> result;
            for (std::size_t i = 0; i < m_Size; i++) {
                if (sccIndex[i] == ccID) {
                    result.push_back(i);
                }
            }
            return result;
        }

    };

}

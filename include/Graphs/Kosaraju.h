//
// Created by Nicholas on 29 Dec 2020.
//

#pragma once

#include <cstddef>
#include <stdexcept>
#include <vector>
#include <stack>
#include "SimpleGraph_Unweighted.h"
#include "DepthFirstOrder.h"

namespace wtl {

    /**
     * Kosaraju's algorithm to find strongly connected components of a directed graph.
     */
    class Kosaraju {
    private:

        using Graph = SimpleGraph_Unweighted<true>;

        /// Number of vertices
        const std::size_t m_Size;

        /// Number of CC
        std::size_t m_Count = 0;

        /// Keep track of which CC a vertex belongs to.
        std::size_t* m_Id = nullptr;

        [[nodiscard]] inline bool outOfBounds(std::size_t v) const noexcept {
            return v < 0 || v >= m_Size;
        }

        void dfs(const Graph& graph, std::size_t v, bool visited[]) {
            visited[v] = true;
            m_Id[v] = m_Count;
            for (std::size_t w : graph.adjacent(v)) {
                if (!visited[w]) {
                    dfs(graph, w, visited);
                }
            }
        }

        void scc(const Graph& graph) {
            bool* visited = new bool[m_Size]{false};
            Graph reversedGraph = graph.reverse();
            DepthFirstOrder<DFS_Order::REVERSE_POSTORDER> depthFirstOrder(reversedGraph);
            for (auto& v : depthFirstOrder.getOrdering()) {
                if (!visited[v]) {
                    dfs(graph, v, visited);
                    ++m_Count;
                }
            }
        }

    public:

        /**
         * Constructor
         * @param graph
         */
        Kosaraju(const Graph& graph)
                : m_Size(graph.vertex()) {
            m_Id = new std::size_t[m_Size];
            scc(graph);
        }

        /**
         * Get the number of connected components.
         * @return
         */
        [[nodiscard]] std::size_t getCount() const noexcept {
            return m_Count;
        }

        /**
         * Check if there exists a path between 2 vertices.
         * @param v
         * @param w
         * @return
         */
        [[nodiscard]] bool stronglyConnected(std::size_t v, std::size_t w) const {
            if (outOfBounds(v) || outOfBounds(w)) {
                throw std::invalid_argument("Invalid vertex");
            }
            return m_Id[v] == m_Id[w];
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
            return m_Id[v];
        }

        /**
         * Get all vertices in specified SCC.
         * @param ccID ID of the CC
         * @return
         */
        [[nodiscard]] std::vector<std::size_t> set(std::size_t ccID) const {
            if (ccID < 0 || ccID >= m_Count) {
                throw std::invalid_argument("CC ID does not exist");
            }
            std::vector<std::size_t> result;
            for (std::size_t i = 0; i < m_Size; i++) {
                if (m_Id[i] == ccID) {
                    result.push_back(i);
                }
            }
            return result;
        }

        ~Kosaraju() {
            delete[] m_Id;
        }

    };

}

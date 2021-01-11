//
// Created by Nicholas on 28 Dec 2020.
//

#pragma once

#include <cstddef>
#include <stdexcept>
#include <utility>
#include "SimpleGraph_Unweighted.h"

namespace wtl {

    /**
     * Check if an undirected graph has a cycle. Also works for unconnected graphs.
     */
    class UndirectedCycle {
    private:

        using Graph = SimpleGraph_Unweighted<false>;

        /// Keep track if graph has cycles
        bool m_HasCycle = false;

        void dfs(const Graph& graph, std::size_t v, std::size_t w, bool visited[]) {
            visited[v] = true;
            if (m_HasCycle) {
                return;
            }
            for (std::size_t u : graph.adjacent(v)) {
                if (!visited[u]) {
                    dfs(graph, u, v, visited);
                } else if (u != w || u == v) {
                    m_HasCycle = true;
                }
            }
        }

        void check(const Graph& graph) {
            std::unique_ptr<bool[]> visited = std::make_unique<bool[]>(graph.vertex());
            for (std::size_t i = 0; i < graph.vertex(); i++) {
                visited[i] = false;
            }
            for (std::size_t i = 0; i < graph.vertex(); i++) {
                if (m_HasCycle) {
                    break;
                } else if (!visited[i]) {
                    dfs(graph, i, i, visited.get());
                }
            }
        }

    public:

        /**
         * Constructor
         * @param graph
         */
        UndirectedCycle(const Graph& graph) {
            check(graph);
        }

        /**
         * Check if graph is cyclic
         * @return True if graph has cycles, false if it is acyclic.
         */
        [[nodiscard]] bool hasCycle() const noexcept {
            return m_HasCycle;
        }

    };

}

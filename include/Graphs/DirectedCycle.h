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
     * Check if an directed graph has a cycle. Also works for unconnected graphs.
     */
    class DirectedCycle {
    private:

        using Graph = SimpleGraph_Unweighted<true>;

        bool m_HasCycle = false;

        void dfs(const Graph& graph, std::size_t v, bool stack[], bool visited[]) {
            stack[v] = true;
            visited[v] = true;
            if (m_HasCycle) {
                return;
            }
            for (std::size_t w : graph.adjacent(v)) {
                if (!visited[w]) {
                    dfs(graph, w, stack, visited);
                } else if (stack[w]) {
                    m_HasCycle = true;
                }
            }
            stack[v] = false;
        }

        void check(const Graph& graph) {
            std::size_t size = graph.vertex();
            bool* stack = new bool[size]{false};
            bool* visited = new bool[size]{false};

            for (std::size_t i = 0; i < size; i++) {
                if (m_HasCycle) {
                    break;
                } else if (!visited[i]) {
                    dfs(graph, i, stack, visited);
                }
            }
            delete[] stack;
            delete[] visited;
        }

    public:

        /**
         * Constructor
         * @param graph
         */
        DirectedCycle(const Graph& graph) {
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

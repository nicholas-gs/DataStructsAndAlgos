//
// Created by Nicholas on 28 Dec 2020.
//

#pragma once

#include <cstddef>
#include <stdexcept>
#include <vector>
#include <stack>
#include <algorithm>
#include "DepthFirstOrder.h"
#include "SimpleGraph_Unweighted.h"
#include "DirectedCycle.h"

namespace wtl {

    /**
     * Get the topological ordering of vertices for a directed graph.
     * Also works for disconnected graphs.
     * Only works for directed acyclic graphs.
     */
    class TopologicalSort {
    private:

        using Graph = SimpleGraph_Unweighted<true>;

        /// Topological Ordering
        std::vector<std::size_t> m_Order;

        /**
         * Topological sort is the reverse post order of a directed acyclic graph
         * @param graph
         */
        void sort(const Graph& graph) {
            DepthFirstOrder<DFS_Order::REVERSE_POSTORDER> depthFirstOrder(graph);
            m_Order = depthFirstOrder.getOrdering();
        }

    public:

        /**
         * Constructor
         * @throw std::runtime_error if graph is cyclic
         * @param graph
         */
        TopologicalSort(const Graph& graph) {
            DirectedCycle directedCycle(graph);
            if (directedCycle.hasCycle()) {
                throw std::runtime_error("Graph is cyclic - Topological ordering does not exist");
            }
            sort(graph);
        }

        /**
         * Get topological ordering of vertices.
         * @return const reference to vector
         */
        [[nodiscard]] const std::vector<std::size_t>& getOrdering() const {
            return m_Order;
        }

    };

}

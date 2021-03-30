//
// Created by Nicholas on 29 Dec 2020.
//

#pragma once

#include <cstddef>
#include <stdexcept>
#include <vector>
#include <queue>
#include <memory>

#include "SimpleGraph_Unweighted.h"
#include "DirectedCycle.h"

namespace wtl {

    /**
     * Kahn's algorithm for topological sort.
     * Only works for directed, unweighted graphs.
     */
    class Kahn {
    private:

        using Graph = SimpleGraph_Unweighted<true>;

        /// Topological Ordering
        std::vector<std::size_t> m_Ordering;

        void sort(const Graph& graph) {
            std::size_t size = graph.vertex();
            std::queue<std::size_t> queue;
            std::unique_ptr<std::size_t[]> inDegrees = std::make_unique<std::size_t[]>(size);
            for (std::size_t i = 0; i < size; i++) {
                inDegrees[i] = graph.inDegree(i);
                if (inDegrees[i] == 0) {
                    queue.push(i);
                }
            }
            while (!queue.empty()) {
                std::size_t v = queue.front();
                m_Ordering.push_back(v);
                queue.pop();
                for (std::size_t w : graph.adjacent(v)) {
                    --inDegrees[w];
                    if (inDegrees[w] == 0) {
                        queue.push(w);
                    }
                }
            }
        }

    public:

        Kahn(const Graph& graph) {
            DirectedCycle directedCycle(graph);
            m_Ordering.reserve(graph.vertex());
            sort(graph);
            if (m_Ordering.size() != graph.vertex()) {
                std::runtime_error("Graph is cyclic - Topological ordering does not exist");
            }
        }

        /**
         * Get topological ordering of vertices. The vertices are arranged from lower index to upper index -
         * edges from vertices in lower index position in the vector to vertices at high index positions.
         * @return const reference to vector
         */
        [[nodiscard]] const std::vector<std::size_t>& getOrdering() const {
            return m_Ordering;
        }

    };

}

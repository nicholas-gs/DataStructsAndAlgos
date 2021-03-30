//
// Created by Nicholas on 11 Jan 2021.
//

#pragma once

#include <memory>

#include "SimpleGraph_Weighted.h"

namespace wtl {

    namespace impl {

        struct FloydWarshall_Path_Detail {
            std::size_t m_Prev = 0;
            double m_Weight = std::numeric_limits<double>::infinity();

            FloydWarshall_Path_Detail() = default;
        };

    }

    /**
     * Floyd-Warshall algorithm for finding all-pairs shortest path in O(V^3) time complexity.
     * @tparam directed
     */
    class FloydWarshall {
    private:

        using Graph = wtl::SimpleGraph_Weighted<true>;
        using PathDetail = impl::FloydWarshall_Path_Detail;

        /// Number of vertices
        const std::size_t m_VertexCount;

        /// Shortest distance from a vertex to another
        std::unique_ptr<double[]> m_Dist;

        [[nodiscard]] inline bool outOfBounds(std::size_t v) const {
            return v >= m_VertexCount;
        }

        /**
         * Since the usual 2D array is being represented as a 1D array,convert (v,w) to index.
         * @param v row index
         * @param w column index
         * @return
         */
        [[nodiscard]] inline std::size_t index(std::size_t v, std::size_t w) const noexcept {
            return (v * m_VertexCount) + w;
        }

        /**
         * Construct the matrix
         * @param diGraph
         */
        void construct(const Graph& graph) {
            for (std::size_t v = 0; v < m_VertexCount; v++) {
                for (std::size_t w = 0; w < m_VertexCount; w++) {
                    std::size_t i = index(v, w);
                    if (v == w) {
                        m_Dist[i] = 0;
                    } else {
                        m_Dist[i] = std::numeric_limits<double>::infinity();
                    }
                }
            }
            for (const auto& edge : graph.allEdges()) {
                m_Dist[index(edge.from(), edge.to())] = edge.getWeight();
            }
        }

        /**
         *
         * @param graph
         */
        void solve(const Graph& graph) {
            for (std::size_t k = 0; k < m_VertexCount; k++) {
                for (std::size_t i = 0; i < m_VertexCount; i++) {
                    for (std::size_t j = 0; j < m_VertexCount; j++) {
                        if (m_Dist[index(i, j)] > m_Dist[index(i, k)] + m_Dist[index(k, j)]) {
                            m_Dist[index(i, j)] = m_Dist[index(i, k)] + m_Dist[index(k, j)];
                        }
                    }
                }
            }
        }

        void checkNegCycle() {
            for (std::size_t k = 0; k < m_VertexCount; k++) {
                for (std::size_t i = 0; i < m_VertexCount; i++) {
                    for (std::size_t j = 0; j < m_VertexCount; j++) {
                        if (m_Dist[index(i, j)] > m_Dist[index(i, k)] + m_Dist[index(k, j)]) {
                            m_Dist[index(i, j)] = -std::numeric_limits<double>::infinity();
                        }
                    }
                }
            }
        }

    public:

        FloydWarshall(const Graph& graph)
                : m_VertexCount(graph.vertex()),
                  m_Dist(std::make_unique<double[]>(m_VertexCount * m_VertexCount)) {
            construct(graph);
            solve(graph);
            checkNegCycle();
        }

        /**
         * Check if there is a path from vertex v to vertex w.
         * @param v
         * @param w
         * @return
         */
        [[nodiscard]] bool hasPathTo(std::size_t v, std::size_t w) const {
            if (outOfBounds(v)) {
                throw std::invalid_argument("Invalid vertex");
            }
            return (m_Dist[index(v, w)] != std::numeric_limits<double>::infinity());
        }

        /**
         * Get the shortest distance from vertex v to vertex w.
         * @param v
         * @return distance, which could be infinity or negative infinity
         */
        [[nodiscard]] double distanceTo(std::size_t v, std::size_t w) const {
            if (outOfBounds(v)) {
                throw std::invalid_argument("Invalid vertex");
            }
            return m_Dist[index(v, w)];
        }

    };

}

//
// Created by Nicholas on 1/1/2021.
//

#pragma once

#include <cstddef>
#include <stdexcept>
#include <vector>
#include <optional>
#include <algorithm>
#include <memory>

#include "SimpleGraph_Weighted.h"
#include "Edges/Edges.h"
#include "../DataStructures/IndexedPriorityQueue.h"

namespace wtl {

    namespace impl {

        struct EagerDijkstra_Element {
            std::size_t m_V;
            double m_Weight;

            EagerDijkstra_Element() : m_V(0), m_Weight(0.0) {}

            EagerDijkstra_Element(std::size_t v, double weight) : m_V(v), m_Weight(weight) {}

            EagerDijkstra_Element(const EagerDijkstra_Element& other) : m_V(other.m_V), m_Weight(other.m_Weight) {}

            bool operator<(const EagerDijkstra_Element& other) const {
                return other.m_Weight < m_Weight;
            }

            ~EagerDijkstra_Element() = default;
        };

        struct EagerDijkstra_Compare {
            bool operator()(double lhs, double rhs) const {
                return rhs < lhs;
            }
        };

    }

    /**
     * Dijkstra's algorithm to find the shortest path between two vertices, in terms of sum of edge weights.
     * Dijkstra's algorithm works for both directed and undirected weighted graphs, but only for non-negative weights.
     * If there is a negative loop, Dijkstra's will go into an infinite loop.
     * @tparam directed
     * @tparam WeightType
     */
    template<bool directed>
    class EagerDijkstra {
    private:

        using Graph = SimpleGraph_Weighted<directed>;
        using Element = impl::EagerDijkstra_Element;
        using IPQ = wtl::IndexedPriorityQueue<std::size_t, double, impl::EagerDijkstra_Compare>;

        /// Number of vertices
        const std::size_t m_Size;

        /// Source vertex
        const std::size_t m_Source;

        const std::size_t NULL_VERTEX;

        /// Keep track the shortest distance from source to all other vertices
        std::unique_ptr<double[]> m_DistTo;

        /// Previous vertex in the shortest tree
        std::unique_ptr<Element[]> m_Prev;

        [[nodiscard]] inline bool outOfBounds(std::size_t v) const {
            return v >= m_Size;
        }

        void relax(const Graph& graph, IPQ& ipq, std::size_t v) {
            for (const auto& e : graph.adjacent(v)) {
                auto[v1, v2] = e.vertices();
                std::size_t other = v1 == v ? v2 : v1;
                double pathWeight = m_DistTo[v] + e.getWeight();
                if (pathWeight < m_DistTo[other]) {
                    m_DistTo[other] = pathWeight;
                    m_Prev[other].m_V = v;
                    m_Prev[other].m_Weight = e.getWeight();
                    if (ipq.exists(other)) {
                        ipq.update(other, pathWeight);
                    } else {
                        ipq.insert(other, pathWeight);
                    }
                }
            }
        }

        void performDijkstra(const Graph& graph) {
            IPQ ipq;
            ipq.insert(m_Source, 0.0);
            while (!ipq.isEmpty()) {
                std::size_t v = ipq.peek().first;
                ipq.poll();
                relax(graph, ipq, v);
            }
        }

    public:

        /**
         * Constructor
         * @param graph
         * @param source
         */
        EagerDijkstra(const Graph& graph, std::size_t source)
                : m_Size(graph.vertex()), m_Source(source), NULL_VERTEX(graph.vertex()),
                  m_DistTo(std::make_unique<double[]>(m_Size)), m_Prev(std::make_unique<Element[]>(m_Size)) {
            for (std::size_t i = 0; i < m_Size; i++) {
                m_DistTo[i] = std::numeric_limits<double>::infinity();
            }
            m_DistTo[m_Source] = 0.0;
            m_Prev[m_Source].m_V = NULL_VERTEX;
            performDijkstra(graph);
        }

        /**
         * Check if there is a path from source vertex to specified vertex.
         * @param v
         * @return
         */
        [[nodiscard]] bool hasPathTo(std::size_t v) const {
            if (outOfBounds(v)) {
                throw std::invalid_argument("Invalid vertex");
            }
            return (m_DistTo[v] != std::numeric_limits<double>::infinity());
        }

        /**
         * Get the total distance from the source to specified vertex.
         * @param v
         * @return
         */
        [[nodiscard]] std::optional<double> distanceTo(std::size_t v) const {
            if (outOfBounds(v)) {
                throw std::invalid_argument("Invalid vertex");
            }
            if (m_DistTo[v] != std::numeric_limits<double>::infinity()) {
                return m_DistTo[v];
            }
            return std::nullopt;
        }

        /**
         * Get the path from the source to specified vertex.
         * @param v
         * @return vector of edges, starting from the source vertex
         */
        [[nodiscard]] auto pathTo(std::size_t v) const {
            if (outOfBounds(v)) {
                throw std::invalid_argument("Invalid vertex");
            }
            if (m_DistTo[v] == std::numeric_limits<double>::infinity()) {
                throw std::runtime_error("No path to specified vertex");
            }
            if constexpr (!directed) {
                using Edge = wtl::UndirectedEdge;
                std::vector <Edge> result;
                while (m_Prev[v].m_V != NULL_VERTEX) {
                    result.emplace_back(m_Prev[v].m_V, v, m_Prev[v].m_Weight);
                    v = m_Prev[v].m_V;
                }
                std::reverse(result.begin(), result.end());
                return result;
            } else {
                using Edge = wtl::DirectedEdge;
                std::vector <Edge> result;
                while (m_Prev[v].m_V != NULL_VERTEX) {
                    result.emplace_back(m_Prev[v].m_V, v, m_Prev[v].m_Weight);
                    v = m_Prev[v].m_V;
                }
                std::reverse(result.begin(), result.end());
                return result;
            }
        }

        /**
         * Destructor
         */
        ~EagerDijkstra() = default;

    };

}
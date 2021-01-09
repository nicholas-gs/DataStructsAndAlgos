//
// Created by Nicholas on 1 Jan 2021.
//

#pragma once

#include <cstddef>
#include <stdexcept>
#include <vector>
#include <queue>
#include <utility>
#include <limits>
#include <memory>

#include "SimpleGraph_Weighted.h"
#include "Edges/Edges.h"

namespace wtl {

    namespace impl {

        template<typename WeightType>
        struct BellmanFord_PathReconstruction {

            // Weight of edge in the shortest path tree
            WeightType m_EdgeWeight;

            // "Parent" vertex in the shortest path tree
            std::size_t m_Prev;

        };

    }

    /**
     * The Bellman-Ford algorithm allows us to find the shortest path from a source vertex to any other vertex
     * in O(V*E) time complexity. In though its time complexity is worse than Dijkstra's algorithm, Bellman-Ford
     * is able to handle negative weighted cycles.
     * @tparam WeightType
     */
    template<typename WeightType>
    class BellmanFord {
    private:

        using DiGraph = wtl::SimpleGraph_Weighted<true, WeightType>;
        using Queue = std::queue<std::size_t>;
        using DIST_INFINITY = std::numeric_limits<WeightType>;
        using DiEdge = wtl::DirectedEdge<WeightType>;
        using DistInfo = impl::BellmanFord_PathReconstruction<WeightType>;

        /// Keep track of number of vertices in the graph
        const std::size_t m_Size;

        const std::size_t NONE_VERTEX;

        /// Source vertex
        const std::size_t m_Source;

        /// Keep track of the shortest distance from source vertex to other vertices.
        std::unique_ptr<WeightType[]> m_Dist;

        /// Keep track of the path info in the shortest path tree.
        std::unique_ptr<DistInfo[]> m_Path;

        bool m_NegCycle = false;

        [[nodiscard]] inline bool outOfBounds(std::size_t v) const noexcept {
            return v >= m_Size;
        }

        void relax(const DiGraph& digraph, std::size_t v, Queue& queue, bool* inQueue) {
            for (const wtl::DirectedEdge<WeightType>& edge : digraph.adjacent(v)) {
                std::size_t w = edge.to();
                WeightType lhs;
                WeightType rhs;
                if (edge.getWeight() > 0) {
                    lhs = m_Dist[v];
                    rhs = m_Dist[w] - edge.getWeight();
                } else {
                    lhs = m_Dist[v] + edge.getWeight();
                    rhs = m_Dist[w];
                }
                if (lhs < rhs) {
                    m_Path[w].m_Prev = v;
                    m_Path[w].m_EdgeWeight = edge.getWeight();
                    m_Dist[w] = m_Dist[v] + edge.getWeight();
                    if (!inQueue[w]) {
                        inQueue[w] = true;
                        queue.push(w);
                    }
                }
            }
        }

        void performBellmanFord(const DiGraph& digraph) {
            Queue queue;
            std::unique_ptr<bool[]> inQueue = std::make_unique<bool[]>(m_Size);
            for (std::size_t j = 0; j < m_Size; j++) {
                inQueue[j] = false;
            }
            std::size_t i = 0;
            // First iteration
            for (std::size_t j = 0; j < m_Size; j++) {
                relax(digraph, j, queue, inQueue.get());
            }
            i++;
            // Next m_Size - 2 iterations
            while (i < m_Size - 1 && !queue.empty()) {
                std::size_t num = queue.size();
                while (num > 0) {
                    std::size_t w = queue.front();
                    queue.pop();
                    inQueue[w] = false;
                    relax(digraph, w, queue, inQueue.get());
                    num--;
                }
                i++;
            }
        }

        void detectNegCycle(const DiGraph& diGraph) {
            for (std::size_t v = 0; v < m_Size && !m_NegCycle; v++) {
                for (const DirectedEdge<WeightType>& edge : diGraph.adjacent(v)) {
                    WeightType lhs;
                    WeightType rhs;
                    std::size_t w = edge.to();
                    if (edge.getWeight() > 0) {
                        lhs = m_Dist[v];
                        rhs = m_Dist[w] - edge.getWeight();
                    } else {
                        lhs = m_Dist[v] + edge.getWeight();
                        rhs = m_Dist[w];
                    }
                    if (lhs < rhs) {
                        //std::cout << "Even better path " << v << " to " << w << " from " << lhs << " to " << rhs << '\n';
                        m_NegCycle = true;
                        break;
                    }
                }
            }
        }

    public:

        /**
         * Constructor
         * @param diGraph
         * @param source
         */
        BellmanFord(const DiGraph& diGraph, std::size_t source)
                : m_Size(diGraph.vertex()), NONE_VERTEX(m_Size),
                  m_Source(source), m_Dist(std::make_unique<WeightType[]>(m_Size)),
                  m_Path(std::make_unique<DistInfo[]>(m_Size)) {
            for (std::size_t i = 0; i < m_Size; i++) {
                // Infinity is represented by the numeric limit
                m_Dist[i] = DIST_INFINITY::max();
            }
            m_Dist[m_Source] = 0;
            m_Path[m_Source].m_Prev = NONE_VERTEX;
            performBellmanFord(diGraph);
            detectNegCycle(diGraph);
            for (std::size_t i = 0; i < m_Size; i++) {
                // Infinity is represented by the numeric limit
               std::cout << m_Dist[i] << '\n';
            }
        }

        [[nodiscard]] bool hasPathTo(std::size_t v) const {
            if (outOfBounds(v)) {
                throw std::invalid_argument("Invalid vertex");
            }
            return m_Dist[v] != DIST_INFINITY::max();
        }

        /**
         * Get the total distance from the source to specified vertex.
         * @param v
         * @return
         */
        [[nodiscard]] std::optional<WeightType> distanceTo(std::size_t v) const {
            if (outOfBounds(v)) {
                throw std::invalid_argument("Invalid vertex");
            }
            if (m_Dist[v] != DIST_INFINITY::max()) {
                return m_Dist[v];
            }
            return std::nullopt;
        }

        /**
        * Get the path from the source to specified vertex.
        * @param v
        * @return vector of edges, starting from the source vertex
        */
        [[nodiscard]] std::vector<wtl::DirectedEdge<WeightType>> pathTo(std::size_t v) const {
            if (outOfBounds(v)) {
                throw std::invalid_argument("Invalid vertex");
            }
            if (m_Dist[v] == DIST_INFINITY::max()) {
                throw std::runtime_error("No path to specified vertex");
            }
            using Edge = wtl::DirectedEdge<WeightType>;
            std::vector<Edge> result;
            while (m_Path[v].m_Prev != NONE_VERTEX) {
                result.emplace_back(m_Path[v].m_Prev, v, m_Path[v].m_EdgeWeight);
                v = m_Path[v].m_Prev;
            }
            std::reverse(result.begin(), result.end());
            return result;
        }

        [[nodiscard]] bool hasNegCycle() const noexcept {
            return m_NegCycle;
        }

    };

}

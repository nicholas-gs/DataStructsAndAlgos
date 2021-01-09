//
// Created by Nicholas on 30 Dec 2020.
//

#pragma once

#include <cstddef>
#include <vector>

#include "SimpleGraph_Weighted.h"
#include "Edges/Edges.h"
#include "../SortingAlgorithms/MergeSort.h"
#include "../DataStructures/UnionFind.h"

namespace wtl {

    /**
     * Kruskal's algorithm finds the minimum spanning tree/forest from a undirected weighted graph.
     * Unlike Prim's algorithm, there is no easy way of identifying vertices and edges of a MST in a MSF if the graph
     * is disconnected.
     * @tparam WeightType
     */
    template<typename WeightType>
    class Kruskal {
    private:

        using Graph = SimpleGraph_Weighted<false, WeightType>;
        using Edge = UndirectedEdge<WeightType>;
        using Sort = wtl::MergeSort<Edge>;
        using UF = wtl::UnionFind;
        using Bucket = std::vector<Edge>;

        /// Number of vertices in the graph
        std::size_t m_Size = 0;

        /// Union Find object
        UF m_UF;

        /// Store the edges in each MST
        Bucket m_Bucket;

        /**
         * Sort the weighted edges in the graph
         * @param graph
         * @param sortedEdges
         */
        void sort(const Graph& graph, std::vector<Edge>& sortedEdges) {
            for (std::size_t i = 0; i < m_Size; i++) {
                for (const Edge& edge : graph.adjacent(i)) {
                    // Prevent duplicate edges since its a undirected graph
                    if (edge.other(i) > i) {
                        sortedEdges.push_back(edge);
                    }
                }
            }
            Sort::sort(sortedEdges);
        }

        void process(const Graph& graph, const std::vector<Edge>& sortedEdges) {
            for (const Edge& edge : sortedEdges) {
                if (m_Bucket.size() == m_Size - 1) {
                    break;
                }
                auto[v, w] = edge.vertices();
                if (!m_UF.connected(v, w)) {
                    m_UF.join(v, w);
                    m_Bucket.push_back(edge);
                }
            }
        }

    public:

        /**
         * Constructor
         * @param graph
         */
        explicit Kruskal(const Graph& graph)
                : m_Size(graph.vertex()), m_UF(m_Size) {
            std::vector<Edge> sortedEdges;
            sortedEdges.reserve(graph.edge());
            sort(graph, sortedEdges);
            m_Bucket.reserve(m_Size - 1);
            process(graph, sortedEdges);
        }

        /**
         * Get the number of MST. If the value is greater than 1, then there is a minimum spanning forest.
         * @return
         */
        [[nodiscard]] std::size_t getCount() const noexcept {
            return m_UF.getSets();
        }

        /**
         * Get all the edges in the MST/MSF.
         * @return
         */
        [[nodiscard]] std::vector<Edge> edges() const {
            return m_Bucket;
        }

    };

}

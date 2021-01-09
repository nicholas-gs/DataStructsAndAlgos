//
// Created by Nicholas on 29 Dec 2020.
//

#pragma once

#include <cstddef>
#include <stdexcept>
#include <list>
#include <iterator>

#include "Edges/Edges.h"
#include "../DataStructures/PriorityQueue.h"
#include "SimpleGraph_Weighted.h"

namespace wtl {

    namespace impl {

        /**
         * Functor for priority queue of weighted edges in MST algorithms.
         * @tparam WeightType
         */
        template<typename WeightType>
        struct LazyPrimComparator {
            bool operator()(const UndirectedEdge<WeightType>& lhs, const UndirectedEdge<WeightType>& rhs) const {
                return rhs < lhs;
            }
        };

    }

    /**
     * Prim's algorithm to find the minimum spanning tree/forest from a undirected, weighted graph.
     * This is the Lazy version of Prim's algorithm and uses a Heap based Priority Queue.
     * The algorithm works with graphs with negative weights, as well as disconnected graphs.
     * For disconnected graphs, it finds the minimum spanning forest (MSF) instead of minimum spanning tree (MST).
     */
    template<typename WeightType>
    class LazyPrim {
    private:

        using Graph = SimpleGraph_Weighted<false, WeightType>;
        using Edge = UndirectedEdge<WeightType>;
        using Bucket = std::list<Edge>;
        using Compare = impl::LazyPrimComparator<WeightType>;
        using PQ = wtl::PriorityQueue<Edge, Compare>;

        /// Number of vertices in the graph
        std::size_t m_Size = 0;

        /// Keep track of which MST a vertex belongs to
        std::size_t* m_Id = nullptr;

        /// Store the edges in each MST
        std::list<Bucket> m_Buckets;

        [[nodiscard]] inline bool outOfBounds(std::size_t v) const noexcept {
            return v < 0 || v >= m_Size;
        }

        /**
         * Consider all edges incident to specified vertex and insert them into the priority queue,
         * provided they are edges that point out of the cut.
         * @param graph
         * @param pq
         * @param v
         * @param inMST
         */
        void cut(const Graph& graph, PQ& pq, std::size_t v, bool inMST[]) {
            for (const Edge& e : graph.adjacent(v)) {
                auto[v1, v2] = e.vertices();
                if (!inMST[v1] || !inMST[v2]) {
                    pq.insert(e);
                }
            }
        }

        void primProcess(const Graph& graph, std::size_t i, bool inMST[]) {
            inMST[i] = true;
            m_Id[i] = m_Buckets.size() - 1;
            auto iter = m_Buckets.end();
            std::advance(iter, -1);
            PQ pq;
            cut(graph, pq, i, inMST);
            while (!pq.isEmpty()) {
                Edge edge = pq.peek();
                auto[v1, v2] = edge.vertices();
                pq.poll();
                if (!inMST[v1] || !inMST[v2]) {
                    std::size_t newVertex = !inMST[v1] ? v1 : v2;
                    inMST[newVertex] = true;
                    m_Id[newVertex] = m_Buckets.size() - 1;
                    iter->push_back(edge);
                    cut(graph, pq, newVertex, inMST);
                }
            }
        }

    public:

        /**
         * Constructor
         * @param graph
         */
        explicit LazyPrim(const Graph& graph) : m_Size(graph.vertex()) {
            m_Id = new std::size_t[m_Size];
            bool* inMST = new bool[m_Size]{false};
            for (std::size_t i = 0; i < m_Size; i++) {
                if (!inMST[i]) {
                    m_Buckets.push_back(Bucket());
                    primProcess(graph, i, inMST);
                }
            }
            delete[] inMST;
        }

        /**
         * Get the number of MST. If the value is greater than 1, then there is a minimum spanning forest.
         * @return
         */
        [[nodiscard]] std::size_t getCount() const noexcept {
            return m_Buckets.size();
        }

        /**
         * Get the weight of a specified MST
         * @param id
         * @return
         */
        [[nodiscard]] std::size_t getMSTWeight(std::size_t id) const {
            if (id < 0 || id >= m_Buckets.size()) {
                throw std::invalid_argument("Invalid MST id");
            }
            WeightType weight = 0;
            auto iter = m_Buckets.begin();
            std::advance(iter, id);
            for (const Edge& edge : (*iter)) {
                weight += edge.getWeight();
            }
            return weight;
        }

        /**
         * Get the MST id a vertex belongs to
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
         * Get all the vertices that belongs to the same MST.
         * @param treeId
         * @return stl vector containing all the vertices in the MST
         */
        [[nodiscard]] std::vector<std::size_t> vertices(std::size_t treeId) const {
            if (treeId >= m_Buckets.size()) {
                throw std::invalid_argument("Invalid MST id");
            }
            std::vector<std::size_t> vector;
            for (std::size_t i = 0; i < m_Size; i++) {
                if (m_Id[i] == treeId) {
                    vector.push_back(i);
                }
            }
            return vector;
        }

        /**
         * Get the edges of a specified MST
         * @param treeId
         * @return
         */
        [[nodiscard]] std::vector<Edge> edges(std::size_t treeId) const {
            if (treeId >= m_Buckets.size()) {
                throw std::invalid_argument("Invalid MST id");
            }
            auto iter = m_Buckets.begin();
            std::advance(iter, treeId);
            std::vector<Edge> result;
            for (const UndirectedEdge<WeightType>& edge : *iter) {
                result.push_back(edge);
            }
            return result;
        }

        /**
         * Destructor
         */
        ~LazyPrim() {
            delete[] m_Id;
        }

    };

}
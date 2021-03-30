//
// Created by Nicholas on 27/12/2020.
//

#pragma once

#include <cstddef>
#include <stdexcept>
#include <type_traits>
#include <vector>
#include <set>
#include <memory>

#include "Edges/Edges.h"

namespace wtl {

    /**
     * A Simple Graph is a graph that does not allow parallel edges between vertices. Self-loops are allowed.
     * The graphs can be either directed or undirected.
     * @tparam m_Directed If the graph is directed or undirected
     */
    template<bool m_Directed>
    class SimpleGraph_Unweighted {
    private:

        using Bucket = std::set<std::size_t>;

        /// Number of vertices in the graph
        const std::size_t m_VertexCount;

        /// Number of edges in the graph
        std::size_t m_EdgeCount = 0;

        /// Graph represented as an adjacency list
        std::unique_ptr<Bucket[]> m_Graph;

        /// Array to keep track of the in-degrees of all vertices. Only valid for directed graphs.
        std::unique_ptr<std::size_t[]> m_InDegree;

        /**
         * Check if an vertex is invalid
         * @param v
         * @return True if invalid, false if valid
         */
        [[nodiscard]] inline bool outOfBounds(std::size_t v) const noexcept {
            return v < 0 || v >= m_VertexCount;
        }

    public:

        /**
         * Constructor
         * @param vertexCount Number of vertices in the graph
         * @param directed True if graph is directed, false if undirected
         */
        SimpleGraph_Unweighted(std::size_t vertexCount)
                : m_VertexCount(vertexCount) {
            if (m_VertexCount < 1) {
                throw std::invalid_argument("Graph cannot have less than 1 vertex");
            }
            m_Graph = std::make_unique<Bucket[]>(m_VertexCount);
            if constexpr (m_Directed) {
                m_InDegree = std::make_unique<std::size_t[]>(m_VertexCount);
                for (std::size_t i = 0; i < m_VertexCount; i++) {
                    m_InDegree[i] = 0;
                }
            }
        }

        /**
         * Copy constructor
         * @param other
         */
        SimpleGraph_Unweighted(const SimpleGraph_Unweighted& other)
                : m_VertexCount(other.m_VertexCount) {
            m_Graph = std::make_unique<Bucket[]>(m_VertexCount);
            m_InDegree = std::make_unique<std::size_t[]>(m_VertexCount);
            for (std::size_t i = 0; i < m_VertexCount; i++) {
                m_Graph[i] = other.m_Graph[i];
                if constexpr (m_Directed) {
                    m_InDegree[i] = other.m_InDegree[i];
                }
            }
        }

        /**
         * Copy assignment
         * @param other
         * @return
         */
        SimpleGraph_Unweighted& operator=(const SimpleGraph_Unweighted& other) {
            if (this != &other) {
                if (m_VertexCount != other.m_VertexCount) {
                    throw std::runtime_error("Copy assignment of graphs with different number of vertices");
                }
                m_EdgeCount = other.m_EdgeCount;
                for (std::size_t i = 0; i < m_VertexCount; i++) {
                    m_Graph[i] = other.m_Graph[i];
                    if constexpr (m_Directed) {
                        m_InDegree[i] = other.m_InDegree[i];
                    }
                }
            }
            return *this;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        /**
         * Get the number of vertices in the graph
         * @return
         */
        [[nodiscard]] std::size_t vertex() const noexcept {
            return m_VertexCount;
        }

        /**
         * Get the number of edges in the graph
         * @return
         */
        [[nodiscard]] std::size_t edge() const noexcept {
            return m_EdgeCount;
        }

        /**
         * Check if the graph is directed
         * @return True if graph is directed, false if undirected
         */
        [[nodiscard]] constexpr bool directed() const noexcept {
            return m_Directed;
        }

        /**
         * Get all vertices adjacent to specified vertex. That is, all vertices reachable from specified vertex.
         * @param v
         * @return Vertex containing all adjacent vertex. Order is unspecified.
         */
        [[nodiscard]] std::vector<std::size_t> adjacent(std::size_t v) const {
            if (outOfBounds(v)) {
                throw std::invalid_argument("Invalid vertex");
            }
            std::vector<std::size_t> vector;
            for (auto w : m_Graph[v]) {
                vector.push_back(w);
            }
            return vector;
        }

        /**
         * Get all edges in the graph
         * @return vector of edges
         */
        [[nodiscard]] auto allEdges() const {
            if constexpr (m_Directed) {
                std::vector<wtl::UnWeightedDirectedEdge> vector;
                for (std::size_t v = 0; v < m_VertexCount; v++) {
                    for (std::size_t w : m_Graph[v]) {
                        vector.emplace_back(v, w);
                    }
                }
                return vector;
            } else {
                std::vector<wtl::UnWeightedUndirectedEdge> vector;
                for (std::size_t v = 0; v < m_VertexCount; v++) {
                    for (std::size_t w : m_Graph[v]) {
                        if (w >= v) {
                            vector.emplace_back(v, w);
                        }
                    }
                }
                return vector;
            }
        }

        /**
         * Check if vertex w is adjacent to vertex v - if an edge points from vertex v to vertex w.
         * @param v
         * @param w
         * @return True if vertex w is adjacent to vertex v, false if not.
         */
        [[nodiscard]] bool hasEdge(std::size_t v, std::size_t w) const {
            if (outOfBounds(v) || outOfBounds(w)) {
                throw std::invalid_argument("Invalid vertex");
            }
            return m_Graph[v].count(w) != 0;
        }

        /**
         * Check if a vertex has is a self-loop.
         * @param v
         * @return True if there is self-loop, false if not.
         */
        [[nodiscard]] bool hasSelfLoop(std::size_t v) const {
            if (outOfBounds(v)) {
                throw std::invalid_argument("Invalid vertex");
            }
            return m_Graph[v].count(v) != 0;
        }

        /**
         * Get the degree of a vertex.
         * @note Illegal operation for directed graph
         * @param v
         * @return
         */
        [[nodiscard]] std::size_t degree(std::size_t v) const {
            if constexpr (m_Directed) {
                throw std::runtime_error("Illegal operation for directed graph");
            } else {
                if (outOfBounds(v)) {
                    throw std::invalid_argument("Invalid vertex");
                }
                std::size_t count = 0;
                for (std::size_t w : m_Graph[v]) {
                    if (w != v) {
                        ++count;
                    } else {
                        count += 2;
                    }
                }
                return count;
            }
        }

        /**
         * Get the InDegree of a vertex
         * @param v
         * @return
         */
        [[nodiscard]] std::size_t inDegree(std::size_t v) const {
            if constexpr (!m_Directed) {
                throw std::runtime_error("Illegal Operation for a undirected graph");
            } else {
                if (outOfBounds(v)) {
                    throw std::invalid_argument("Invalid vertex");
                }
                return m_InDegree[v];
            }
        }

        /**
         * Get the OutDegree of a vertex
         * @param v
         * @return
         */
        [[nodiscard]] std::size_t outDegree(std::size_t v) const {
            if constexpr (!m_Directed) {
                throw std::runtime_error("Illegal Operation for a undirected graph");
            } else {
                if (outOfBounds(v)) {
                    throw std::invalid_argument("Invalid vertex");
                }
                return m_Graph[v].size();
            }
        }

        /**
         * Get a directed graph we the edges reversed.
         * @return
         */
        [[nodiscard]] SimpleGraph_Unweighted reverse() const {
            if constexpr (!m_Directed) {
                throw std::runtime_error("Illegal Operation for a undirected graph");
            } else {
                SimpleGraph_Unweighted reversedGraph(m_VertexCount);
                for (std::size_t i = 0; i < m_VertexCount; i++) {
                    for (const std::size_t w : m_Graph[i]) {
                        reversedGraph.addEdge(w, i);
                    }
                }
                return reversedGraph;
            }
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        /**
         * Add an edge from vertex v to vertex w. For an undirected graph, the parameter ordering does not matter.
         * If the same edge already exists, then no changes occur.
         * @param v
         * @param w
         * @return True if edge was inserted, false if not.
         */
        bool addEdge(std::size_t v, std::size_t w) {
            if (outOfBounds(v) || outOfBounds(w)) {
                throw std::invalid_argument("Invalid vertex");
            }
            if (!m_Graph[v].insert(w).second) {
                return false;
            }
            if constexpr (!m_Directed) {
                if (v != w) {
                    m_Graph[w].insert(v);
                }
            } else {
                ++m_InDegree[w];
            }
            m_EdgeCount++;
            return true;
        }

        /**
         * Remove an edge from the graph. For an undirected graph, the parameter ordering does not matter.
         * If the edge does not exist, then no changes occur.
         * @param v
         * @param w
         * @return True if edge was removed, false if not.
         */
        bool removeEdge(std::size_t v, std::size_t w) {
            if (outOfBounds(v) || outOfBounds(w)) {
                throw std::invalid_argument("Invalid vertex");
            }
            if (m_Graph[v].erase(w) == 0) {
                return false;
            }
            if constexpr (!m_Directed) {
                if (v != w) {
                    m_Graph[w].erase(v);
                }
            } else {
                --m_InDegree[w];
            }
            m_EdgeCount--;
            return true;
        }

        /**
         * Remove all edges in the graph.
         */
        void clear() noexcept {
            for (std::size_t i = 0; i < m_VertexCount; i++) {
                m_Graph[i].clear();
                if constexpr (m_Directed) {
                    m_InDegree[i] = 0;
                }
            }
            m_EdgeCount = 0;
        }

    };

}

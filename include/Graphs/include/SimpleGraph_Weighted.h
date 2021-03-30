//
// Created by Nicholas on 27/12/2020.
//

#pragma once

#include <cstddef>
#include <stdexcept>
#include <type_traits>
#include <vector>
#include <set>
#include <utility>
#include <memory>

#include "SimpleGraph_Unweighted.h"
#include "Edges/Edges.h"

namespace wtl {

    namespace impl {

        /**
         * Internal Node representation for the Graph
         */
        struct SimpleGraph_Weighted_Node {
            std::size_t w;
            double weight;

            SimpleGraph_Weighted_Node(std::size_t w, double weight)
                    : w(w), weight(weight) {}

            bool operator==(const SimpleGraph_Weighted_Node& other) const noexcept {
                return w == other.w;
            }
        };

        /**
         * Functor for std::set
         * @tparam Node
         */
        template<typename Node>
        struct SimpleGraph_Weighted_Compare {
            bool operator()(const Node& lhs, const Node& rhs) const {
                return lhs.w < rhs.w;
            }
        };

    }

    /**
     * A Simple Graph is a graph that does not allow parallel edges between vertices. Self-loops also allowed.
     * The graphs can be either directed or undirected.
     * @tparam m_Directed If the graph is directed or undirected
     */
    template<bool m_Directed>
    class SimpleGraph_Weighted {
    private:

        using UnweightedGraph = SimpleGraph_Unweighted<m_Directed>;
        using Node = impl::SimpleGraph_Weighted_Node;
        using Compare = impl::SimpleGraph_Weighted_Compare<Node>;
        using Bucket = std::set<Node, Compare>;
        using AdjList = std::unique_ptr<Bucket[]>;

        /// Number of vertices in the graph
        const std::size_t m_VertexCount;

        /// Number of edges in the graph
        std::size_t m_EdgeCount = 0;

        /// Graph represented as an adjacency list
        AdjList m_Graph;

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
        */
        explicit SimpleGraph_Weighted(std::size_t vertexCount)
                : m_VertexCount(vertexCount) {
            if (m_VertexCount < 1) {
                throw std::invalid_argument("Graph cannot have less than 1 vertex");
            }
            m_Graph = std::make_unique<Bucket[]>(m_VertexCount);
        }

        /**
         * Copy constructor
         * @param other
         */
        SimpleGraph_Weighted(const SimpleGraph_Weighted& other)
                : m_VertexCount(other.m_VertexCount) {
            m_Graph = std::make_unique<Bucket[]>(m_VertexCount);
            for (std::size_t i = 0; i < m_VertexCount; i++) {
                m_Graph[i] = other.m_Graph[i];
            }
        }

        /**
         * Copy assignment
         * @param other
         * @return
         */
        SimpleGraph_Weighted& operator=(const SimpleGraph_Weighted& other) {
            if (this != &other) {
                if (m_VertexCount != other.m_VertexCount || m_Directed != other.directed()) {
                    throw std::runtime_error("Different graph parameters");
                }
                m_EdgeCount = other.m_EdgeCount;
                for (std::size_t i = 0; i < m_VertexCount; i++) {
                    m_Graph[i] = other.m_Graph[i];
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
         * Get all edges adjacent to specified vertex. That is, all edges in the direction pointing away from v.
         * @param v
         * @return stl vector of edges
         */
        [[nodiscard]] auto adjacent(std::size_t v) const {
            if (outOfBounds(v)) {
                throw std::invalid_argument("Invalid vertex");
            }
            if constexpr (m_Directed) {
                std::vector<wtl::WeightedDirectedEdge> vector;
                for (const auto& e : m_Graph[v]) {
                    vector.push_back(wtl::WeightedDirectedEdge(v, e.w, e.weight));
                }
                return vector;
            } else {
                std::vector<wtl::WeightedUndirectedEdge> vector;
                for (const auto& e : m_Graph[v]) {
                    vector.push_back(wtl::WeightedUndirectedEdge(v, e.w, e.weight));
                }
                return vector;
            }
        }

        /**
         * Get all edges in the graph
         * @return vector of edges
         */
        [[nodiscard]] auto allEdges() const {
            if constexpr (m_Directed) {
                std::vector<wtl::WeightedDirectedEdge> vector;
                for (std::size_t v = 0; v < m_VertexCount; v++) {
                    for (const auto& e : m_Graph[v]) {
                        vector.push_back(wtl::WeightedDirectedEdge(v, e.w, e.weight));
                    }
                }
                return vector;
            } else {
                std::vector<wtl::WeightedUndirectedEdge> vector;
                for (std::size_t v = 0; v < m_VertexCount; v++) {
                    for (const auto& e : m_Graph[v]) {
                        if (e.w >= v) {
                            vector.push_back(wtl::WeightedUndirectedEdge(v, e.w, e.weight));
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
            return m_Graph[v].count({w, 0}) != 0;
        }

        /**
         * Get the weight of an edge from vertex v to vertex w. Return std::nullopt if edge does not exist.
         * @param v
         * @param w
         * @return
         */
        [[nodiscard]] std::optional<double> getEdgeWeight(std::size_t v, std::size_t w) const {
            if (outOfBounds(v) || outOfBounds(w)) {
                throw std::invalid_argument("Invalid vertex");
            }
            auto it = m_Graph[v].find({w, 0});
            if (it == m_Graph[v].end()) {
                return std::nullopt;
            }
            return it->weight;
        }

        /**
         * Update the weight of an edge. For a directed graph, the edge is from vertex v to vertex w.
         * If the edge does not exist, then nothing occurs.
         * @param v
         * @param w
         * @param weight
         * @return
         */
        bool updateWeight(std::size_t v, std::size_t w, double weight) {
            if (outOfBounds(v) || outOfBounds(w)) {
                throw std::invalid_argument("Invalid vertex");
            }
            auto it = m_Graph[v].find({w, 0.0});
            if (it == m_Graph[v].end()) {
                return false;
            }
            m_Graph[v].erase(it);
            m_Graph[v].emplace(w, weight);
            if (!m_Directed) {
                m_Graph[w].erase({v, 0.0});
                m_Graph[w].emplace(v, weight);
            }
            return true;
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
            return m_Graph[v].count({v, 0}) != 0;
        }

        /**
         * Get the degree of a vertex.
         * @note Illegal operation for directed graph
         * @param v
         * @return
         */
        [[nodiscard]] std::size_t degree(std::size_t v) const {
            if (m_Directed) {
                throw std::runtime_error("Illegal operation for directed graph");
            }
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

        /**
         * Get the InDegree of a vertex
         * @param v
         * @return
         */
        [[nodiscard]] std::size_t inDegree(std::size_t v) const {
            if (outOfBounds(v)) {
                throw std::invalid_argument("Invalid vertex");
            }
            std::size_t count = 0;
            for (std::size_t i = 0; i < m_VertexCount; i++) {
                for (std::size_t w : m_Graph[i]) {
                    if (w == v) {
                        count++;
                    }
                }
            }
            return count;
        }

        /**
         * Get the OutDegree of a vertex
         * @param v
         * @return
         */
        [[nodiscard]] std::size_t outDegree(std::size_t v) const {
            if (outOfBounds(v)) {
                throw std::invalid_argument("Invalid vertex");
            }
            return m_Graph[v].size();
        }

        /**
         * Create a unweighted graph from the weighted graph. The new graph is identical,
         * apart that edges are unweighted. Useful as certain graph processing algorithms,
         * where edge weights are have no bearing on the algorithm, only accept unweighted graphs.
         * @return
         */
        [[nodiscard]] UnweightedGraph convert() const noexcept {
            UnweightedGraph unweightedGraph(m_VertexCount);
            for (std::size_t i = 0; i < m_VertexCount; i++) {
                for (const auto& node : m_Graph[i]) {
                    unweightedGraph.addEdge(i, node.w);
                }
            }
            return unweightedGraph;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        /**
         * Add an edge from vertex v to vertex w. For an undirected graph, the parameter ordering does not matter.
         * If the same edge already exists, then no changes occur.
         * @param v
         * @param w
         * @return True if edge was inserted, false if not.
         */
        bool addEdge(std::size_t v, std::size_t w, double weight) {
            if (outOfBounds(v) || outOfBounds(w)) {
                throw std::invalid_argument("Invalid vertex");
            }
            auto result = m_Graph[v].emplace(w, weight);
            if (!result.second) {
                return false;
            }
            if (!m_Directed && v != w) {
                m_Graph[w].emplace(v, weight);
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
            auto result = m_Graph[v].erase({w, 0});
            if (result == 0) {
                return false;
            }
            if (!m_Directed && v != w) {
                m_Graph[w].erase({v, 0});
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
            }
            m_EdgeCount = 0;
        }

    };

}

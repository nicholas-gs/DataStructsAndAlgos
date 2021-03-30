//
// Created by Nicholas on 7 Jan 2021.
//

#pragma once

#include <cstddef>
#include <stdexcept>
#include <forward_list>
#include <memory>
#include <type_traits>
#include <vector>
#include <optional>

namespace wtl {

    /**
     * A directed edge in a flow network.
     * @tparam CapacityType
     */
    template<typename CapacityType>
    struct FlowEdge {
        std::size_t m_Source;
        std::size_t m_Dest;
        CapacityType m_Capacity;
        CapacityType m_Flow = 0;

        FlowEdge(std::size_t source, std::size_t dest, CapacityType capacity)
                : m_Source(source), m_Dest(dest), m_Capacity(capacity) {}

        [[nodiscard]] std::size_t source() const noexcept {
            return m_Source;
        }

        [[nodiscard]] std::size_t dest() const noexcept {
            return m_Dest;
        }

        [[nodiscard]] CapacityType getFlow() const noexcept {
            return m_Flow;
        }

        [[nodiscard]] CapacityType getCapacity() const noexcept {
            return m_Capacity;
        }

        [[nodiscard]] std::size_t other(std::size_t v) const {
            if (v == m_Source)
                return m_Dest;
            else if (v == m_Dest) {
                return m_Source;
            } else {
                throw std::invalid_argument("Invalid vertex");
            }
        }

        /**
         * Retrieve the residual capacity of the flow edge. Note that the residual capacity depends on
         * which direction of the directed flow edge we are traversing.
         * @param v
         * @return
         */
        [[nodiscard]] CapacityType getResidualCapacity(std::size_t v) const {
            if (v == m_Dest) {
                return m_Capacity - m_Flow;
            } else if (v == m_Source) {
                return m_Flow;
            } else {
                throw std::invalid_argument("Invalid vertex");
            }
        }

        /**
         * Add flow to the flow edge. If the flow is in the same direction as the directed edge,
         * then the flow is added to the current flow, else it is subtracted.
         * @param flow
         * @param v
         */
        void addFlowTo(std::size_t v, CapacityType flow) {
            if (flow < 0 || getResidualCapacity(v) < flow) {
                throw std::invalid_argument("Flow cannot be negative");
            }
            if (v == m_Dest) {
                m_Flow += flow;
            } else if (v == m_Source) {
                m_Flow -= flow;
            }
        }

    };

    namespace impl {

        template<typename CapacityType>
        struct FlowNetwork_FlowEdge_Compare {
        private:
            using FlowEdge = FlowEdge<CapacityType>;

        public:
            bool operator()(const FlowEdge& lhs, const FlowEdge& rhs) const {
                return (lhs.m_Source == rhs.m_Source && lhs.m_Dest == rhs.m_Dest)
                       || (lhs.m_Source == rhs.m_Dest && lhs.m_Dest == rhs.m_Source);
            }

        };

    }

    /**
     * A flow network is a directed graph where each edge has a capacity and flow value.
     * The amount of flow on an edge cannot exceed the capacity of the edge.
     * The vertices of the flow network are labelled from [0, number of vertices - 1].
     * Flow Network does not allow for more than 1 edge between 2 vertices, regardless of edge direction.
     * @tparam CapacityType
     */
    template<typename CapacityType>
    class FlowNetwork {
    private:

        using Edge = FlowEdge<CapacityType>;
        using Edge_Ptr = Edge*;
        using Compare = impl::FlowNetwork_FlowEdge_Compare<CapacityType>;
        using Edges = std::forward_list<Edge>;
        using Bucket = std::vector<Edge_Ptr>;
        using AdjList = std::unique_ptr<Bucket[]>;

        template<typename T>
        friend
        class FordFulkerson;

        template<typename T>
        friend
        class EdmondsKarp;

        template<typename T>
        friend
        class Dinic;

        /// Number of vertices in the graph
        const std::size_t m_VertexCount;

        /// Keep track of number of edges.
        std::size_t m_EdgeCount = 0;

        /// Store all the edge objects
        Edges m_Edges;

        /// Graph represented as an adjacency list, which stores pointers to edges
        AdjList m_Network;

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
         * @param vertexCount
         */
        explicit FlowNetwork(std::size_t vertexCount)
                : m_VertexCount(vertexCount) {
            static_assert(std::is_arithmetic_v<CapacityType>, "CapacityType must be arithmetic type");
            if (m_VertexCount < 1) {
                throw std::invalid_argument("Flow network cannot have less than 1 vertex");
            }
            m_Network = std::make_unique<Bucket[]>(m_VertexCount);
        }

        /**
         * Copy constructor
         * @param other
         * @return
         */
        FlowNetwork(const FlowNetwork& other) = delete;

        /**
         * Copy assignment
         * @param other
         * @return
         */
        FlowNetwork& operator=(const FlowNetwork& other) = delete;

        /**
         * Get the number of vertices in the flow network.
         * @return
         */
        [[nodiscard]] std::size_t vertex() const noexcept {
            return m_VertexCount;
        }

        /**
         * Get the number of edges in the flow network.
         * @return
         */
        [[nodiscard]] std::size_t edge() const noexcept {
            return m_EdgeCount;
        }

        /**
         * Get all edges adjacent to the specified vertex, which includes all edges pointing away
         * and pointing towards the specified vertex.
         * @param v
         * @return vector of edge objects
         */
        [[nodiscard]] std::vector<Edge> adjacent(std::size_t v) const {
            if (outOfBounds(v)) {
                throw std::invalid_argument("Invalid vertex");
            }
            std::vector<Edge> result;
            for (const Edge_Ptr edgePtr : m_Network[v]) {
                result.push_back(*edgePtr);
            }
            return result;
        }

        /**
         * Get all edges in the flow network.
         * @return vector of edge objects
         */
        [[nodiscard]] std::vector<Edge> allEdges() const noexcept {
            std::vector<Edge> result;
            result.reserve(m_EdgeCount);
            for (std::size_t i = 0; i < m_VertexCount; i++) {
                for (const Edge_Ptr edgePtr : m_Network[i]) {
                    if (edgePtr->m_Source == i) {
                        result.push_back(*edgePtr);
                    }
                }
            }
            return result;
        }

        /**
         * Check there is an edge between vertex v and vertex w, regardless of the edge direction.
         * @param v
         * @param w
         * @return true if there is an edge, false if not.
         */
        [[nodiscard]] bool hasEdge(std::size_t v, std::size_t w) const {
            if (outOfBounds(v) || outOfBounds(w)) {
                throw std::invalid_argument("Invalid vertex");
            }
            Edge e(v, w, 1);
            Compare compare;
            for (const Edge_Ptr edgePtr : m_Network[v]) {
                if (compare(e, *edgePtr)) {
                    return true;
                }
            }
            return false;
        }

        /**
         * Get the edge between vertex v and vertex w, regardless of the edge direction.
         * @param v
         * @param w
         * @return const reference to the edge.
         */
        [[nodiscard]] const Edge& getEdge(std::size_t v, std::size_t w) const {
            if (outOfBounds(v) || outOfBounds(w)) {
                throw std::invalid_argument("Invalid vertex");
            }
            Edge e(v, w, 1);
            Compare compare;
            for (const Edge_Ptr edgePtr : m_Network[v]) {
                if (compare(e, *edgePtr)) {
                    return *edgePtr;
                }
            }
            throw std::runtime_error("Edge does not exist");
        }

        /**
         * Add an edge to the flow network. Since more than 1 edge cannot exist between 2 vertices, duplicate edges
         * are not inserted.
         * @param source
         * @param dest
         * @param capacity
         * @return True if edge was inserted, false if not
         */
        bool addEdge(std::size_t source, std::size_t dest, CapacityType capacity) {

            if (outOfBounds(source) || outOfBounds(dest)) {
                throw std::invalid_argument("Invalid vertices");
            }
            if (capacity < 0) {
                throw std::invalid_argument("Capacity cannot be less than 0");
            }
            if (hasEdge(source, dest)) {
                return false;
            }
            m_Edges.emplace_front(source, dest, capacity);
            m_Network[source].push_back(&(*m_Edges.begin()));
            m_Network[dest].push_back(&(*m_Edges.begin()));
            return true;
        }

    };

}

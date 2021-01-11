//
// Created by Nicholas on 1 Jan 2021.
//

#pragma once

#include <cstddef>
#include <stdexcept>
#include <vector>

#include "SimpleGraph_Weighted.h"
#include "Edges/Edges.h"
#include "TopologicalSort.h"

namespace wtl {

    namespace impl {

        struct Topological_SP_Element {
            std::size_t m_V;
            double m_Weight;

            Topological_SP_Element() : m_V(0), m_Weight(0.0) {}

            Topological_SP_Element(std::size_t v, double weight) : m_V(v), m_Weight(weight) {}

            Topological_SP_Element(const Topological_SP_Element& other) : m_V(other.m_V), m_Weight(other.m_Weight) {}

            bool operator<(const Topological_SP_Element& other) const {
                return other.m_Weight < m_Weight;
            }

            ~Topological_SP_Element() = default;
        };
    }

    /**
     * Get the shortest path from a source vertex to any other vertex in O(V+E) time complexity using the topological
     * ordering of vertices. Only works for weighted and directed acyclic graphs.
     * @tparam WeightType
     */
    class Topological_SP {
    private:

        using DiGraph = wtl::SimpleGraph_Weighted<true>;
        using DiEdge = wtl::DirectedEdge;
        using TopSort = wtl::TopologicalSort;
        using Element = impl::Topological_SP_Element;

        /// Number of vertices
        const std::size_t m_Size;

        /// Source vertex
        const std::size_t m_Source;

        const std::size_t NULL_VERTEX;

        /// Keep track the shortest distance from source to all other vertices
        double* m_DistTo = nullptr;

        /// Previous vertex in the shortest tree
        Element* m_Prev = nullptr;

        [[nodiscard]] inline bool outOfBounds(std::size_t v) const {
            return v >= m_Size;
        }

        void init() {
            m_DistTo = new double[m_Size];
            for (std::size_t i = 0; i < m_Size; i++) {
                m_DistTo[i] = std::numeric_limits<double>::infinity();
            }
            m_DistTo[m_Source] = 0.0;
            m_Prev = new Element[m_Size];
            m_Prev[m_Source].m_V = NULL_VERTEX;
        }

        void process(const DiGraph& digraph, const std::vector <std::size_t>& ordering) {
            for (std::size_t v : ordering) {
                for (const DiEdge& e : digraph.adjacent(v)) {
                    std::size_t to = e.to();
                    if (m_DistTo[v] + e.getWeight() < m_DistTo[to]) {
                        m_DistTo[to] = m_DistTo[v] + e.getWeight();
                        m_Prev[to].m_V = v;
                        m_Prev[to].m_Weight = e.getWeight();
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
        Topological_SP(const DiGraph& diGraph, std::size_t source)
                : m_Size(diGraph.vertex()), m_Source(source), NULL_VERTEX(diGraph.vertex()) {
            const auto ug = diGraph.convert();
            // Throws if the graph is cyclic
            TopSort topSort(ug);
            init();
            std::vector <std::size_t> ordering = topSort.getOrdering();
            process(diGraph, ordering);
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
            return m_DistTo[v] != std::numeric_limits<double>::infinity();
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
            std::vector <DiEdge> result;
            while (m_Prev[v].m_V != NULL_VERTEX) {
                result.emplace_back(m_Prev[v].m_V, v, m_Prev[v].m_Weight);
                v = m_Prev[v].m_V;
            }
            std::reverse(result.begin(), result.end());
            return result;
        }

        ~Topological_SP() {
            delete[] m_DistTo;
            delete[] m_Prev;
        }

    };

}

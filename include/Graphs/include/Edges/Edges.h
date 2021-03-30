//
// Created by Nicholas on 27/12/2020.
//

#pragma once

#include <cstddef>
#include <stdexcept>
#include <utility>

namespace wtl {

    /**
     * Base class for weighted undirected edge and weighted directed edge.
     * @tparam WeightType
     */
    class WeightedEdgeBase {
    protected:

        std::size_t m_V;
        std::size_t m_W;
        double m_Weight;

    public:

        WeightedEdgeBase() = default;

        /**
         * Constructor
         * @param v
         * @param w
         * @param weight
         */
        WeightedEdgeBase(std::size_t v, std::size_t w, double weight)
                : m_V(v), m_W(w), m_Weight(weight) {}

        [[nodiscard]] double getWeight() const noexcept {
            return m_Weight;
        }

        bool operator<(const WeightedEdgeBase& other) const noexcept {
            return m_Weight < other.m_Weight;
        }

        [[nodiscard]] std::pair<std::size_t, std::size_t> vertices() const noexcept {
            return {m_V, m_W};
        }

    };

    /**
     * Undirected weighted edge
     * @tparam WeightType
     */
    class WeightedUndirectedEdge : public WeightedEdgeBase {
    public:

        WeightedUndirectedEdge() = default;

        WeightedUndirectedEdge(std::size_t v, std::size_t w, double weight)
                : WeightedEdgeBase(v, w, weight) {}

        [[nodiscard]] std::size_t either() const noexcept {
            return WeightedEdgeBase::m_V;
        }

        [[nodiscard]] std::size_t other(std::size_t v) const {
            if (v == WeightedEdgeBase::m_V) {
                return WeightedEdgeBase::m_W;
            }
            if (v == WeightedEdgeBase::m_W) {
                return WeightedEdgeBase::m_V;
            }
            throw std::invalid_argument("Invalid vertex");
        }

        bool operator==(const WeightedUndirectedEdge& other) const {
            return (this->m_Weight == other.m_Weight)
                   && ((this->m_W == other.m_W) && (this->m_V == other.m_V)
                       || (this->m_W == other.m_V) && (this->m_V == other.m_W));
        }

    };

    /**
     * Directed weighted edge
     * @tparam WeightType
     */
    class WeightedDirectedEdge : public WeightedEdgeBase {
    public:

        WeightedDirectedEdge() = default;

        WeightedDirectedEdge(std::size_t v, std::size_t w, double weight)
                : WeightedEdgeBase(v, w, weight) {}

        [[nodiscard]] std::size_t from() const noexcept {
            return WeightedEdgeBase::m_V;
        }

        [[nodiscard]] std::size_t to() const noexcept {
            return WeightedEdgeBase::m_W;
        }

        bool operator==(const WeightedDirectedEdge& other) const {
            return (this->m_Weight == other.m_Weight)
                   && (this->m_W == other.m_W) && (this->m_V == other.m_V);
        }

    };

}

namespace wtl {

    /**
     * Base class for unweighted undirected edge and unweighted directed edge.
     */
    class UnWeightedEdgeBase {
    protected:

        std::size_t m_V;
        std::size_t m_W;

    public:

        UnWeightedEdgeBase() = default;

        /**
         * Constructor
         * @param v
         * @param w
         */
        UnWeightedEdgeBase(std::size_t v, std::size_t w)
                : m_V(v), m_W(w) {}

        [[nodiscard]] std::pair<std::size_t, std::size_t> vertices() const noexcept {
            return {m_V, m_W};
        }

    };

    /**
     * Undirected unweighted edge
     */
    class UnWeightedUndirectedEdge : public UnWeightedEdgeBase {
    public:

        UnWeightedUndirectedEdge() = default;

        UnWeightedUndirectedEdge(std::size_t v, std::size_t w)
                : UnWeightedEdgeBase(v, w) {}

        [[nodiscard]] std::size_t either() const noexcept {
            return UnWeightedEdgeBase::m_V;
        }

        [[nodiscard]] std::size_t other(std::size_t v) const {
            if (v == UnWeightedEdgeBase::m_V) {
                return UnWeightedEdgeBase::m_W;
            }
            if (v == UnWeightedEdgeBase::m_W) {
                return UnWeightedEdgeBase::m_V;
            }
            throw std::invalid_argument("Invalid vertex");
        }

        bool operator==(const UnWeightedUndirectedEdge& other) const {
            return ((this->m_W == other.m_W) && (this->m_V == other.m_V)
                    || (this->m_W == other.m_V) && (this->m_V == other.m_W));
        }

    };

    /**
     * Directed unweighted edge
     */
    class UnWeightedDirectedEdge : public UnWeightedEdgeBase {
    public:

        UnWeightedDirectedEdge() = default;

        UnWeightedDirectedEdge(std::size_t v, std::size_t w)
                : UnWeightedEdgeBase(v, w) {}

        [[nodiscard]] std::size_t from() const noexcept {
            return UnWeightedEdgeBase::m_V;
        }

        [[nodiscard]] std::size_t to() const noexcept {
            return UnWeightedEdgeBase::m_W;
        }

        bool operator==(const UnWeightedDirectedEdge& other) const {
            return (this->m_W == other.m_W) && (this->m_V == other.m_V);
        }

    };

}

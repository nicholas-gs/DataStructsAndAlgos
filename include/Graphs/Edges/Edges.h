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
    class EdgeBase {
    protected:

        std::size_t m_V;
        std::size_t m_W;
        double m_Weight;

    public:

        EdgeBase() = default;

        /**
         * Constructor
         * @param v
         * @param w
         * @param weight
         */
        EdgeBase(std::size_t v, std::size_t w, double weight)
                : m_V(v), m_W(w), m_Weight(weight) {}

        [[nodiscard]] double getWeight() const noexcept {
            return m_Weight;
        }

        bool operator<(const EdgeBase& other) const noexcept {
            return m_Weight < other.m_Weight;
        }

        [[nodiscard]] std::pair <std::size_t, std::size_t> vertices() const noexcept {
            return {m_V, m_W};
        }

    };

    /**
     * Undirected weighted edge
     * @tparam WeightType
     */
    class UndirectedEdge : public EdgeBase {
    public:

        UndirectedEdge() = default;

        UndirectedEdge(std::size_t v, std::size_t w, double weight)
                : EdgeBase(v, w, weight) {}

        [[nodiscard]] std::size_t either() const noexcept {
            return EdgeBase::m_V;
        }

        [[nodiscard]] std::size_t other(std::size_t v) const {
            if (v == EdgeBase::m_V) {
                return EdgeBase::m_W;
            }
            if (v == EdgeBase::m_W) {
                return EdgeBase::m_V;
            }
            throw std::invalid_argument("Invalid vertex");
        }

        bool operator==(const UndirectedEdge& other) const {
            return (this->m_Weight == other.m_Weight)
                   && ((this->m_W == other.m_W) && (this->m_V == other.m_V)
                       || (this->m_W == other.m_V) && (this->m_V == other.m_W));
        }

    };

    /**
     * Directed weighted edge
     * @tparam WeightType
     */
    class DirectedEdge : public EdgeBase {
    public:

        DirectedEdge() = default;

        DirectedEdge(std::size_t v, std::size_t w, double weight)
                : EdgeBase(v, w, weight) {}

        [[nodiscard]] std::size_t from() const noexcept {
            return EdgeBase::m_V;
        }

        [[nodiscard]] std::size_t to() const noexcept {
            return EdgeBase::m_W;
        }

        bool operator==(const DirectedEdge& other) const {
            return (this->m_Weight == other.m_Weight)
                   && (this->m_W == other.m_W) && (this->m_V == other.m_V);
        }

    };

}

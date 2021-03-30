//
// Created by Nicholas on 9 Jan 2021.
//

#pragma once

#include <vector>
#include <stack>
#include <memory>

#include "FlowNetwork.h"

namespace wtl {

    /**
     * From a max flow network, get the vertices and edges in the min cut.
     * @tparam CapacityType
     */
    template<typename CapacityType>
    class MinCut {
    private:

        using FlowNetwork = FlowNetwork<CapacityType>;
        using FlowEdge = wtl::FlowEdge<CapacityType>;
        /// Store all the vertices in the min-cut
        std::vector<std::size_t> m_MinCutVertices;

        /// Store all the edges from the min-cut, whose sum of flow is equal to the max-flow
        std::vector<FlowEdge> m_MinCutEdges;

        void solveVertices(const FlowNetwork& flowNetwork, std::size_t source) {
            std::unique_ptr<bool[]> visited = std::make_unique<bool[]>(flowNetwork.vertex());
            for (std::size_t i = 0; i < flowNetwork.vertex(); ++i) {
                visited[i] = false;
            }
            visited[source] = true;
            std::stack<std::size_t> stack;
            stack.push(source);
            m_MinCutVertices.push_back(source);
            while (!stack.empty()) {
                std::size_t v = stack.top();
                stack.pop();
                for (const FlowEdge& flowEdge : flowNetwork.adjacent(v)) {
                    std::size_t w = flowEdge.other(v);
                    if (!visited[w] && flowEdge.getResidualCapacity(w) > 0) {
                        visited[w] = true;
                        stack.push(w);
                        m_MinCutVertices.push_back(w);
                    }
                }
            }
        }

        void solveEdges(const FlowNetwork& flowNetwork) {
            std::unique_ptr<bool[]> inMinCut = std::make_unique<bool[]>(flowNetwork.vertex());
            for (std::size_t i = 0; i < flowNetwork.vertex(); ++i) {
                inMinCut[i] = false;
            }
            for (std::size_t v : m_MinCutVertices) {
                inMinCut[v] = true;
            }
            for (std::size_t v : m_MinCutVertices) {
                for(const FlowEdge& flowEdge : flowNetwork.adjacent(v)) {
                    if(flowEdge.source() == v && !inMinCut[flowEdge.dest()]) {
                        m_MinCutEdges.push_back(flowEdge);
                    }
                }
            }
        }

    public:

        MinCut(const FlowNetwork& flowNetwork, std::size_t source) {
            solveVertices(flowNetwork, source);
            solveEdges(flowNetwork);
        }

        /**
         * Get all the vertices in the min cut
         * @return
         */
        [[nodiscard]] std::vector<std::size_t> getMinCut() const {
            return m_MinCutVertices;
        }

        /**
         * Get all the edges in the min cut
         * @return
         */
        [[nodiscard]] std::vector<FlowEdge> getEdges() const {
            return m_MinCutEdges;
        }

    };

}

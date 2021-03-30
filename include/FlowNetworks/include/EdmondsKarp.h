//
// Created by Nicholas on 8 Jan 2021.
//

#pragma once

#include <cstddef>
#include <stdexcept>
#include <limits>
#include <vector>
#include <queue>
#include <algorithm>
#include <memory>

#include "FlowNetwork.h"
#include "MinCut.h"

namespace wtl {

    /**
     * Carry out the Edmonds-Karp algorithm to find the max cut and min flow of a flow network
     * in O(|V||E|^2) time complexity. The Edmonds-Karp algorithm is an implementation of the Ford-Fulkerson method
     * which specifies that breadth-first search is used to find the shortest available augmenting path.
     * @tparam CapacityType
     */
    template<typename CapacityType>
    class EdmondsKarp {
    private:

        using FlowNetwork = FlowNetwork<CapacityType>;
        using FlowEdge = FlowEdge<CapacityType>;
        using FlowEdge_Ptr = FlowEdge*;

        /// Source vertex
        const std::size_t m_Source;

        /// Sink vertex
        const std::size_t m_Sink;

        /// Max Flow & Min Cut value
        CapacityType m_MaxFlow = 0;

        /// Store all the vertices in the min cut.
        std::vector<std::size_t> m_MinCutVertices;

        /// Store all the edges from the min-cut, whose sum of flow is equal to the max-flow
        std::vector<FlowEdge> m_MinCutEdges;

        void solve(FlowNetwork& flowNetwork) {
            std::vector<FlowEdge_Ptr> augmentingPath;
            while (hasAugmentingPath(flowNetwork, augmentingPath)) {
                CapacityType bottleNeckValue = getBottleNeck(augmentingPath);
                augmentPath(augmentingPath, bottleNeckValue);
                m_MaxFlow += bottleNeckValue;
            }
        }

        bool hasAugmentingPath(FlowNetwork& flowNetwork, std::vector<FlowEdge_Ptr>& augmentingPath) {
            std::unique_ptr<bool[]> visited = std::make_unique<bool[]>(flowNetwork.vertex());
            std::unique_ptr<FlowEdge_Ptr[]> flowEdgeTo = std::make_unique<FlowEdge_Ptr[]>(flowNetwork.vertex());
            for (std::size_t i = 0; i < flowNetwork.vertex(); i++) {
                visited[i] = false;
                flowEdgeTo[i] = nullptr;
            }
            visited[m_Source] = true;
            std::queue<std::size_t> queue;
            queue.push(m_Source);
            while (!queue.empty()) {
                std::size_t v = queue.front();
                queue.pop();
                for (FlowEdge_Ptr edgePtr : flowNetwork.m_Network[v]) {
                    std::size_t w = edgePtr->other(v);
                    if (!visited[w] && edgePtr->getResidualCapacity(w) > 0) {
                        visited[w] = true;
                        queue.push(w);
                        flowEdgeTo[w] = edgePtr;
                        if (w == m_Sink) {
                            break;
                        }
                    }
                }
            }
            if (!visited[m_Sink]) {
                return false;
            }
            return reconstructPath(flowEdgeTo.get(), augmentingPath);
        }

        bool reconstructPath(FlowEdge_Ptr flowEdgeTo[], std::vector<FlowEdge_Ptr>& augmentingPath) {
            augmentingPath.clear();
            std::size_t v = m_Sink;
            while (v != m_Source) {
                augmentingPath.push_back(flowEdgeTo[v]);
                if (flowEdgeTo[v] == nullptr) {
                    break;
                }
                v = flowEdgeTo[v]->other(v);
            }
            std::reverse(std::begin(augmentingPath), std::end(augmentingPath));
            return v == m_Source;
        }

        [[nodiscard]] CapacityType getBottleNeck(const std::vector<FlowEdge_Ptr>& augmentingPath) const {
            std::size_t v = m_Source;
            CapacityType bottleNeck = std::numeric_limits<CapacityType>::max();
            for (const FlowEdge_Ptr ptr : augmentingPath) {
                CapacityType residual = ptr->getResidualCapacity(ptr->other(v));
                if (residual < bottleNeck) {
                    bottleNeck = residual;
                }
                v = ptr->other(v);
            }
            return bottleNeck;
        }

        void augmentPath(std::vector<FlowEdge_Ptr>& augmentingPath, CapacityType bottleNeckValue) {
            std::size_t v = m_Source;
            for (FlowEdge_Ptr ptr : augmentingPath) {
                ptr->addFlowTo(ptr->other(v), bottleNeckValue);
                v = ptr->other(v);
            }
        }

    public:

        /**
         * Constructor
         * @param flowNetwork
         */
        EdmondsKarp(FlowNetwork& flowNetwork, std::size_t source, std::size_t sink)
                : m_Source(source), m_Sink(sink) {
            if (flowNetwork.outOfBounds(source) || flowNetwork.outOfBounds(sink)) {
                throw std::invalid_argument("Invalid source or sink");
            }
            solve(flowNetwork);
            MinCut minCut(flowNetwork, m_Source);
            m_MinCutVertices = minCut.getMinCut();
            m_MinCutEdges = minCut.getEdges();
        }

        /**
         * Get the Max Flow / Min Cut value
         * @return
         */
        [[nodiscard]] CapacityType getMaxFlow() const noexcept {
            return m_MaxFlow;
        }

        /**
         * Get all vertices in the min cut
         * @return
         */
        [[nodiscard]] auto getMinCutVertices() const {
            return m_MinCutVertices;
        }

        /**
         * Get all edges from the min-cut, whose sum of flow is equal to the max-flow
         * @return
         */
        [[nodiscard]] auto getMinCutEdges() const {
            return m_MinCutEdges;
        }

    };

}

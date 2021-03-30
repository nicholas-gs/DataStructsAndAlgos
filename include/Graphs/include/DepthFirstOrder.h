//
// Created by Nicholas on 29 Dec 2020.
//

#pragma once

#include <cstddef>
#include <vector>
#include <stack>
#include <algorithm>
#include "SimpleGraph_Unweighted.h"

namespace wtl {

    enum class DFS_Order {
        PREORDER, POSTORDER, REVERSE_POSTORDER
    };

    template<DFS_Order dfsOrder>
    class DepthFirstOrder {
    private:

        template<bool directed>
        using Graph = SimpleGraph_Unweighted<directed>;

        /// Number of vertices in the graph
        const std::size_t m_Size;

        /// Ordering
        std::vector<std::size_t> m_Vector;

        template<bool directed>
        void dfs(const Graph<directed>& graph, std::size_t i, bool visited[]) {
            visited[i] = true;
            if constexpr (dfsOrder == DFS_Order::PREORDER) {
                m_Vector.push_back(i);
            }
            for (std::size_t w : graph.adjacent(i)) {
                if (!visited[w]) {
                    dfs(graph, w, visited);
                }
            }
            if constexpr (dfsOrder == DFS_Order::POSTORDER || dfsOrder == DFS_Order::REVERSE_POSTORDER) {
                m_Vector.push_back(i);
            }
        }

    public:

        template<bool directed>
        DepthFirstOrder(const Graph<directed>& graph)
                : m_Size(graph.vertex()) {

            m_Vector.reserve(m_Size);
            auto* visited = new bool[m_Size]{false};
            for (std::size_t i = 0; i < m_Size; i++) {
                if (!visited[i]) {
                    dfs(graph, i, visited);
                }
            }
            delete[] visited;
            if constexpr (dfsOrder == DFS_Order::REVERSE_POSTORDER) {
                std::reverse(m_Vector.begin(), m_Vector.end());
            }
        }

        [[nodiscard]] DFS_Order order() const noexcept {
            return dfsOrder;
        }

        [[nodiscard]] const std::vector<std::size_t>& getOrdering() const noexcept {
            return m_Vector;
        }

    };

}

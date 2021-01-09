//
// Created by Nicholas on 28 Dec 2020.
//

#pragma once

#include <cstddef>
#include <queue>
#include <utility>
#include "SimpleGraph_Unweighted.h"

namespace wtl {

    /**
     * A bipartite graph is a graph whose vertices can be divided into two disjoint and independent sets
     * such that every edge only connects a vertex in one set to a vertex in another. This definition also works for
     * disconnected graphs.
     * This algorithm only works for undirected graphs.
     */
    class UndirectedBipartite {
    private:

        using Graph = SimpleGraph_Unweighted<false>;

        /// Keep track if graph is bipartite
        bool m_Bipartite;

        [[nodiscard]] bool bfs(const Graph& graph, std::size_t i, bool colors[], bool visited[]) {
            if (!visited[i]) {
                visited[i] = true;
                std::queue<std::size_t> queue;
                queue.push(i);
                while (!queue.empty()) {
                    std::size_t v = queue.front();
                    queue.pop();
                    for (std::size_t adjV : graph.adjacent(v)) {
                        if (!visited[adjV]) {
                            colors[adjV] = !colors[v];
                            visited[adjV] = true;
                            queue.push(adjV);
                        } else if (colors[v] == colors[adjV]) {
                            return false;
                        }
                    }
                }
            }
            return true;
        }

        [[nodiscard]] bool check(const Graph& graph) {
            std::size_t size = graph.vertex();
            bool* colors = new bool[size]{false};
            bool* visited = new bool[size]{false};
            std::optional<bool> result = std::nullopt;
            for (std::size_t i = 0; i < size; i++) {
                if (!bfs(graph, i, colors, visited)) {
                    result = false;
                    break;
                }
            }
            delete[] colors;
            delete[] visited;
            return result.has_value() ? false : true;
        }

    public:

        UndirectedBipartite(const Graph& graph) {
            m_Bipartite = check(graph);
        }

        /**
         * Check if graph is bipartite
         * @return True if graph is bipartite, false if it is not
         */
        [[nodiscard]] bool isBipartite() const noexcept {
            return m_Bipartite;
        }

    };

}

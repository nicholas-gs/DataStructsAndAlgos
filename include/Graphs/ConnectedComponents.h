//
// Created by Nicholas on 28 Dec 2020.
//

#pragma once

#include <cstddef>
#include <stdexcept>
#include <queue>
#include <algorithm>
#include <memory>

#include "SimpleGraph_Unweighted.h"

namespace wtl {

    /**
     * A Connected component is a maximal set of vertices in a graph that are linked to each other by paths.
     * Therefore, there exists a path between any 2 vertices in the CC,
     * and no path exists between 2 vertices in different CC.
     * This algorithm works for unweighted, undirected graph.
     */
    class ConnectedComponents {
    private:

        using Graph = SimpleGraph_Unweighted<false>;

        /// Number of vertices
        const std::size_t m_Size;

        /// Number of CC
        std::size_t m_Count = 0;

        /// Keep track of which CC a vertex belongs to.
        std::unique_ptr<std::size_t[]> m_Id;

        [[nodiscard]] inline bool outOfBounds(std::size_t v) const noexcept {
            return v < 0 || v >= m_Size;
        }

        void bfs(const Graph& graph, std::size_t source, bool visited[]) {
            std::queue<std::size_t> queue;
            queue.push(source);
            m_Id[source] = m_Count;
            while (!queue.empty()) {
                std::size_t v = queue.front();
                queue.pop();
                for (std::size_t adjV : graph.adjacent(v)) {
                    if (!visited[adjV]) {
                        visited[adjV] = true;
                        m_Id[adjV] = m_Count;
                        queue.push(adjV);
                    }
                }
            }
        }

        void cc(const Graph& graph) {
            bool visited[m_Size]{false};
            for (std::size_t i = 0; i < m_Size; i++) {
                if (!visited[i]) {
                    bfs(graph, i, visited);
                    m_Count++;
                }
            }
        }

    public:

        ConnectedComponents(const Graph& graph)
                : m_Size(graph.vertex()), m_Id(std::make_unique<std::size_t[]>(m_Size)) {
            cc(graph);
        }

        /**
         * Get the number of connected components.
         * @return
         */
        [[nodiscard]] std::size_t getCount() const noexcept {
            return m_Count;
        }

        /**
         * Check if there exists a path between 2 vertices.
         * @param v
         * @param w
         * @return
         */
        [[nodiscard]] bool connected(std::size_t v, std::size_t w) const {
            if (outOfBounds(v) || outOfBounds(w)) {
                throw std::invalid_argument("Invalid vertex");
            }
            return m_Id[v] == m_Id[w];
        }

        /**
         * Get the CC id that a vertex belongs to.
         * @param v
         * @return
         */
        [[nodiscard]] std::size_t belongs(std::size_t v) const {
            if (outOfBounds(v)) {
                throw std::invalid_argument("Invalid vertex");
            }
            return m_Id[v];
        }

        /**
         * Get all vertices in specified CC.
         * @param ccID ID of the CC
         * @return
         */
        [[nodiscard]] std::vector<std::size_t> set(std::size_t ccID) const {
            if (ccID < 0 || ccID >= m_Count) {
                throw std::invalid_argument("CC ID does not exist");
            }
            std::vector<std::size_t> result;
            for (std::size_t i = 0; i < m_Size; i++) {
                if (m_Id[i] == ccID) {
                    result.push_back(i);
                }
            }
            return result;
        }

        ~ConnectedComponents() = default;

    };

}

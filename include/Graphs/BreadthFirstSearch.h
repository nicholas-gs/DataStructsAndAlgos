//
// Created by Nicholas on 28 Dec 2020.
//

#pragma once

#include <cstddef>
#include <stdexcept>
#include <queue>
#include <stack>
#include <algorithm>
#include "SimpleGraph_Unweighted.h"

namespace wtl {

    /**
     * Breadth first search on a unweighted graph. The path from the source vertex to a specified vertex, if it exists,
     * is the shortest path in terms of the number of edges.
     * Works for both directed and undirected graphs.
     * @tparam directed
     */
    template<bool directed>
    class BreadthFirstSearch {
    private:

        using Graph = SimpleGraph_Unweighted<directed>;

        /// Source vertex
        const std::size_t m_Source;

        /// Number of vertices
        const std::size_t m_Size;

        /// Keep track if vertex was visited during BFS
        bool* m_Visited = nullptr;

        /// Keep track of previous vertex visited before this one
        std::size_t* m_Previous = nullptr;

        void bfs(const Graph& graph) {
            std::queue<std::size_t> queue;
            queue.push(m_Source);
            m_Visited[m_Source] = true;
            while (!queue.empty()) {
                std::size_t v = queue.front();
                queue.pop();
                for (std::size_t adjV : graph.adjacent(v)) {
                    if (!m_Visited[adjV]) {
                        m_Visited[adjV] = true;
                        m_Previous[adjV] = v;
                        queue.push(adjV);
                    }
                }
            }
        }

    public:

        /**
         * Constructor
         * @param graph
         * @param source
         */
        BreadthFirstSearch(const Graph& graph, std::size_t source)
                : m_Source(source), m_Size(graph.vertex()) {
            m_Visited = new bool[m_Size]{false};
            m_Previous = new std::size_t[m_Size];
            for (std::size_t i = 0; i < m_Size; i++) {
                m_Previous[i] = i;
            }
            bfs(graph);
        }

        /**
         * Get the source vertex
         * @return
         */
        [[nodiscard]] std::size_t source() const noexcept {
            return m_Source;
        }

        /**
         * Check if there is a path from the source vertex to the specified vertex v
         * @param v
         * @return
         */
        [[nodiscard]] bool hasPathTo(std::size_t v) const {
            if (v < 0 || v >= m_Size) {
                throw std::invalid_argument("Invalid vertex");
            }
            return m_Visited[v];
        }

        /**
         * Get the path from source vertex to vertex v. Throws if there is no path to specified vertex.
         * @param v Destination vertex
         * @return stl vector of vertices representing the path, starting from the source vertex and ending at
         * vertex v.
         */
        [[nodiscard]] std::vector<std::size_t> pathTo(std::size_t v) const {
            if (v < 0 || v >= m_Size) {
                throw std::invalid_argument("Invalid vertex");
            }
            if (!m_Visited[v]) {
                throw std::runtime_error("There is no path to vertex");
            }
            std::vector<std::size_t> path;
            for (std::size_t w = v; w != m_Source; w = m_Previous[w]) {
                path.push_back(w);
            }
            path.push_back(m_Source);
            std::reverse(std::begin(path), std::end(path));
            return path;
        }

        /**
         * Destructor
         */
        ~BreadthFirstSearch() {
            delete[] m_Visited;
            delete[] m_Previous;
        }
    };

}

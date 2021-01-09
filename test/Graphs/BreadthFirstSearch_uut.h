//
// Created by Nicholas on 28 Dec 2020.
//

#pragma once

#include "../catch.hpp"
#include "../../include/Graphs/BreadthFirstSearch.h"
#include <vector>

TEST_CASE("Breadth First Search - Undirected graph") {

    wtl::SimpleGraph_Unweighted<false> graph(6);
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(0, 5);
    graph.addEdge(2, 1);
    graph.addEdge(2, 3);
    graph.addEdge(2, 4);
    graph.addEdge(3, 5);
    graph.addEdge(3, 4);

    wtl::BreadthFirstSearch bfs(graph, 0);

    REQUIRE(bfs.source() == 0);

    SECTION("Check connectivity") {
        for (std::size_t i = 0; i < graph.vertex(); i++) {
            REQUIRE(bfs.hasPathTo(i) == true);
        }
    }

    SECTION("Check shortest path") {
        REQUIRE(bfs.pathTo(1) == std::vector<std::size_t>{0, 1});
        REQUIRE(bfs.pathTo(3).size() == 3);
        REQUIRE(bfs.pathTo(4) == std::vector<std::size_t>{0, 2, 4});
        REQUIRE(bfs.pathTo(5) == std::vector<std::size_t>{0, 5});
    }
}

TEST_CASE("Breadth First Search - Directed graph") {

    wtl::SimpleGraph_Unweighted<true> graph(6);
    graph.addEdge(1, 0);
    graph.addEdge(0, 2);
    graph.addEdge(0, 5);
    graph.addEdge(5, 3);
    graph.addEdge(3, 4);
    graph.addEdge(3, 2);
    graph.addEdge(4, 2);


    wtl::BreadthFirstSearch bfs(graph, 0);

    SECTION("Check connectivity") {
        REQUIRE(bfs.hasPathTo(1) == false);
    }

    SECTION("Check shortest path") {
        REQUIRE(bfs.pathTo(3) == std::vector<std::size_t>{0, 5, 3});
        REQUIRE(bfs.pathTo(4) == std::vector<std::size_t>{0, 5, 3, 4});
        REQUIRE(bfs.pathTo(2) == std::vector<std::size_t>{0, 2});
    }
}

//
// Created by Nicholas on 28 Dec 2020.
//

#pragma once

#include "catch.hpp"
#include "DepthFirstSearch.h"
#include <vector>

TEST_CASE("Depth First Search - Undirected graph") {

    wtl::SimpleGraph_Unweighted<false> graph(6);
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(0, 5);
    graph.addEdge(2, 1);
    graph.addEdge(2, 3);
    graph.addEdge(2, 4);
    graph.addEdge(3, 5);
    graph.addEdge(3, 4);

    wtl::DepthFirstSearch dfs(graph, 0);

    REQUIRE(dfs.source() == 0);

    SECTION("Check connectivity") {
        for (std::size_t i = 0; i < graph.vertex(); i++) {
            REQUIRE(dfs.hasPathTo(i) == true);
        }
    }
}

TEST_CASE("Depth First Search - Directed graph") {

    wtl::SimpleGraph_Unweighted<false> graph(6);
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(0, 5);
    graph.addEdge(1, 2);
    graph.addEdge(2, 3);
    graph.addEdge(4, 2);
    graph.addEdge(3, 5);
    graph.addEdge(5, 3);
    graph.addEdge(3, 4);

    wtl::DepthFirstSearch dfs(graph, 0);

    SECTION("Check connectivity") {
        for (std::size_t i = 0; i < graph.vertex(); i++) {
            REQUIRE(dfs.hasPathTo(i) == true);
        }
    }
}

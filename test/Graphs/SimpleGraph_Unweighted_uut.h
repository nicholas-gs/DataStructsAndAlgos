//
// Created by Nicholas on 29 Dec 2020.
//

#pragma once

#include "../catch.hpp"
#include "../../include/Graphs/SimpleGraph_Unweighted.h"

TEST_CASE("SimpleGraph_Unweighted - Directed Graph") {

    wtl::SimpleGraph_Unweighted<true> graph(13);
    graph.addEdge(0, 1);
    graph.addEdge(0, 5);
    graph.addEdge(0, 6);
    graph.addEdge(2, 0);
    graph.addEdge(2, 3);
    graph.addEdge(3, 5);
    graph.addEdge(5, 4);
    graph.addEdge(6, 4);
    graph.addEdge(6, 9);
    graph.addEdge(7, 6);
    graph.addEdge(8, 7);
    graph.addEdge(9, 10);
    graph.addEdge(9, 11);
    graph.addEdge(9, 12);
    graph.addEdge(11, 12);
    // Duplicate edges
    graph.addEdge(9, 11);
    graph.addEdge(9, 12);
    graph.addEdge(11, 12);

    REQUIRE(graph.vertex() == 13);
    CHECK(graph.directed() == true);
    REQUIRE(graph.edge() == 15);
    REQUIRE(graph.outDegree(9) == 3);
    REQUIRE(graph.outDegree(1) == 0);
    REQUIRE(graph.inDegree(4) == 2);
    REQUIRE(graph.inDegree(12) == 2);
    REQUIRE(graph.hasEdge(7, 8) == false);
    REQUIRE(graph.hasEdge(8, 7) == true);
}


TEST_CASE("SimpleGraph_Unweighted - Undirected Graph") {

    wtl::SimpleGraph_Unweighted<false> graph(13);
    graph.addEdge(0, 1);
    graph.addEdge(0, 5);
    graph.addEdge(0, 6);
    graph.addEdge(2, 0);
    graph.addEdge(2, 3);
    graph.addEdge(3, 5);
    graph.addEdge(5, 4);
    graph.addEdge(6, 4);
    graph.addEdge(6, 9);
    graph.addEdge(7, 6);
    graph.addEdge(8, 7);
    graph.addEdge(9, 10);
    graph.addEdge(9, 11);
    graph.addEdge(9, 12);
    graph.addEdge(11, 12);
    // Duplicate edges
    graph.addEdge(9, 11);
    graph.addEdge(9, 12);
    graph.addEdge(11, 12);

    REQUIRE(graph.vertex() == 13);
    CHECK(graph.directed() == false);
    REQUIRE(graph.edge() == 15);
    REQUIRE(graph.degree(9) == 4);
    REQUIRE(graph.degree(1) == 1);
    REQUIRE(graph.degree(4) == 2);
    REQUIRE(graph.degree(12) == 2);
    REQUIRE(graph.hasEdge(7, 8) == true);
    REQUIRE(graph.hasEdge(8, 7) == true);
}

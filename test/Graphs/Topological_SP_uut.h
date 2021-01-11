//
// Created by Nicholas on 1 Jan 2021.
//

#pragma once

#include <cstddef>
#include <vector>

#include "../catch.hpp"
#include "../../include/Graphs/Edges/Edges.h"
#include "../../include/Graphs/SimpleGraph_Weighted.h"
#include "../../include/Graphs/Topological_SP.h"

TEST_CASE("Shortest path using Topological_SP (1)") {
    wtl::SimpleGraph_Weighted<true> graph(8);
    graph.addEdge(0, 1, 5);
    graph.addEdge(0, 7, 8);
    graph.addEdge(0, 4, 9);
    graph.addEdge(1, 7, 4);
    graph.addEdge(1, 3, 15);
    graph.addEdge(1, 2, 12);
    graph.addEdge(2, 3, 3);
    graph.addEdge(2, 6, 11);
    graph.addEdge(3, 6, 9);
    graph.addEdge(4, 7, 5);
    graph.addEdge(4, 5, 4);
    graph.addEdge(4, 6, 20);
    graph.addEdge(5, 2, 1);
    graph.addEdge(5, 6, 13);
    graph.addEdge(7, 5, 6);
    graph.addEdge(7, 2, 7);

    wtl::Topological_SP sp(graph, 0);
    using Edge = wtl::DirectedEdge;

    SECTION("Getting shortest path between 2 vertices that exists (1)") {
        std::vector<Edge> refSP;
        refSP.emplace_back(0, 4, 9);
        refSP.emplace_back(4, 5, 4);
        refSP.emplace_back(5, 2, 1);
        refSP.emplace_back(2, 3, 3);
        std::vector<Edge> result = sp.pathTo(3);
        REQUIRE(refSP == result);
    }

    SECTION("Getting shortest path between 2 vertices that exists (2)") {
        std::vector<Edge> refSP;
        refSP.emplace_back(0, 7, 8);
        std::vector<Edge> result = sp.pathTo(7);
        REQUIRE(refSP == result);
    }

    SECTION("Getting shortest path from source vertex to itself") {
        REQUIRE(sp.pathTo(0).size() == 0);
    }
}

TEST_CASE("Shortest Path using Topological_SP (2)") {
    wtl::SimpleGraph_Weighted<true> graph(8);
    graph.addEdge(0, 1, 3);
    graph.addEdge(0, 2, 6);
    graph.addEdge(1, 4, 11);
    graph.addEdge(1, 3, 4);
    graph.addEdge(1, 2, 4);
    graph.addEdge(2, 3, 8);
    graph.addEdge(2, 6, 11);
    graph.addEdge(3, 4, -4);
    graph.addEdge(3, 5, 5);
    graph.addEdge(3, 6, 2);
    graph.addEdge(4, 7, 9);
    graph.addEdge(5, 7, 1);
    graph.addEdge(6, 7, 2);

    wtl::Topological_SP sp(graph, 0);
    using Edge = wtl::DirectedEdge;

    SECTION("Getting shortest path between 2 vertices that exists (1)") {
        std::vector<Edge> refSP;
        refSP.emplace_back(0, 1, 3);
        refSP.emplace_back(1, 3, 4);
        std::vector<Edge> result = sp.pathTo(3);
        REQUIRE(refSP == result);
    }

    SECTION("Getting shortest path from source vertex to itself") {
        REQUIRE(sp.pathTo(0).size() == 0);
    }

}

TEST_CASE("Shortest Path using Topological_SP (3)") {

    wtl::SimpleGraph_Weighted<true> graph(8);
    graph.addEdge(5, 4, 0.35);
    graph.addEdge(4, 7, 0.37);
    graph.addEdge(5, 7, 0.28);
    graph.addEdge(5, 1, 0.32);
    graph.addEdge(4, 0, 0.38);
    graph.addEdge(0, 2, 0.26);
    graph.addEdge(3, 7, 0.39);
    graph.addEdge(1, 3, 0.29);
    graph.addEdge(7, 2, 0.34);
    graph.addEdge(6, 2, 0.4);
    graph.addEdge(3, 6, 0.52);
    graph.addEdge(6, 0, 0.58);
    graph.addEdge(6, 4, 0.93);

    using Edge = wtl::DirectedEdge;

    SECTION("Getting shortest path between 2 vertices that exists (1)") {
        wtl::Topological_SP sp(graph, 5);
        std::vector<Edge> refSP;
        refSP.emplace_back(5, 1, 0.32);
        refSP.emplace_back(1, 3, 0.29);
        refSP.emplace_back(3, 6, 0.52);
        REQUIRE(refSP == sp.pathTo(6));
    }

    SECTION("Getting shortest path between 2 vertices that exists (2)") {
        wtl::Topological_SP sp(graph, 7);
        std::vector<Edge> refSP;
        refSP.emplace_back(7, 2, 0.34);
        REQUIRE(refSP == sp.pathTo(2));
    }

    SECTION("Getting shortest path between 2 vertices that don't exist") {
        wtl::Topological_SP sp(graph, 1);
        REQUIRE_THROWS(sp.pathTo(5));
    }

}

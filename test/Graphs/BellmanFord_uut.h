//
// Created by Nicholas on 4 Jan 2021.
//

#pragma once


#include <cstddef>
#include <vector>

#include "../catch.hpp"
#include "../../include/Graphs/Edges/Edges.h"
#include "../../include/Graphs/SimpleGraph_Weighted.h"
#include "../../include/Graphs/BellmanFord.h"

TEST_CASE("Bellman-Ford Algorithm on directed graph (1)") {
    wtl::SimpleGraph_Weighted<true> graph(6);
    graph.addEdge(0, 1, 5);
    graph.addEdge(0, 2, 1);
    graph.addEdge(1, 2, 2);
    graph.addEdge(1, 3, 3);
    graph.addEdge(1, 4, 20);
    graph.addEdge(2, 1, 3);
    graph.addEdge(2, 4, 12);
    graph.addEdge(3, 2, 3);
    graph.addEdge(3, 4, 2);
    graph.addEdge(3, 5, 6);
    graph.addEdge(4, 5, 1);

    using Edge = wtl::DirectedEdge<>;

    SECTION("Getting shortest path between 2 vertices that exists (1)") {
        std::vector<Edge> refSP;
        refSP.emplace_back(0, 2, 1);
        refSP.emplace_back(2, 1, 3);
        refSP.emplace_back(1, 3, 3);
        wtl::BellmanFord bellmanFord(graph, 0);
        REQUIRE(bellmanFord.hasNegCycle() == false);
        std::vector<Edge> result = bellmanFord.pathTo(3);
        REQUIRE(result == refSP);
        REQUIRE(bellmanFord.distanceTo(3) == 7);
    }

    SECTION("Getting shortest path between 2 vertices that exists (2)") {
        std::vector<Edge> refSP;
        refSP.emplace_back(3, 2, 3);
        refSP.emplace_back(2, 1, 3);
        wtl::BellmanFord bellmanFord(graph, 3);
        REQUIRE(bellmanFord.hasNegCycle() == false);
        std::vector<Edge> result = bellmanFord.pathTo(1);
        REQUIRE(result == refSP);
        REQUIRE(bellmanFord.distanceTo(1) == 6);
    }

    SECTION("Getting shortest path between 2 vertices that don't exist") {
        wtl::BellmanFord bellmanFord(graph, 1);
        REQUIRE(bellmanFord.hasNegCycle() == false);
        REQUIRE_THROWS(bellmanFord.pathTo(0));
    }

}

TEST_CASE("Bellman-Ford Algorithm on directed graph (2)") {
    wtl::SimpleGraph_Weighted<true, int> graph(8);
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

    using Edge = wtl::DirectedEdge<int>;

    SECTION("Getting shortest path between 2 vertices that exists (1)") {
        std::vector<Edge> refSP;
        refSP.emplace_back(4, 5, 4);
        refSP.emplace_back(5, 2, 1);
        refSP.emplace_back(2, 3, 3);
        wtl::BellmanFord bellmanFord(graph, 4);
        REQUIRE(bellmanFord.hasNegCycle() == false);
        std::vector<Edge> result = bellmanFord.pathTo(3);
        REQUIRE(result == refSP);
        REQUIRE(bellmanFord.distanceTo(3) == 8);
    }

    SECTION("Getting shortest path between 2 vertices that exists (2)") {
        std::vector<Edge> refSP;
        refSP.emplace_back(4, 5, 4);
        refSP.emplace_back(5, 2, 1);
        refSP.emplace_back(2, 6, 11);
        wtl::BellmanFord bellmanFord(graph, 4);
        std::vector<Edge> result = bellmanFord.pathTo(6);
        REQUIRE(refSP == result);
        REQUIRE(bellmanFord.distanceTo(6) == 16);
    }

    SECTION("Getting shortest path between 2 vertices that don't exist") {
        wtl::BellmanFord bellmanFord(graph, 1);
        REQUIRE(bellmanFord.hasNegCycle() == false);
        REQUIRE_THROWS(bellmanFord.pathTo(0));
    }

}


TEST_CASE("Bellman-Ford Algorithm on directed graph (3)") {

    wtl::SimpleGraph_Weighted<true, double> graph(8);
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

    using Edge = wtl::DirectedEdge<double>;

    SECTION("Getting shortest path between 2 vertices that exists (1)") {
        wtl::BellmanFord bellmanFord(graph, 5);
        REQUIRE(bellmanFord.hasNegCycle() == false);
        std::vector<Edge> refSP;
        refSP.emplace_back(5, 1, 0.32);
        refSP.emplace_back(1, 3, 0.29);
        refSP.emplace_back(3, 6, 0.52);
        REQUIRE(refSP == bellmanFord.pathTo(6));
    }

    SECTION("Getting shortest path between 2 vertices that exists (2)") {
        wtl::BellmanFord bellmanFord(graph, 7);
        std::vector<Edge> refSP;
        refSP.emplace_back(7, 2, 0.34);
        REQUIRE(refSP == bellmanFord.pathTo(2));
    }

    SECTION("Getting shortest path between 2 vertices that don't exist") {
        wtl::BellmanFord bellmanFord(graph, 1);
        REQUIRE_THROWS(bellmanFord.pathTo(5));
    }

}

TEST_CASE("Bellman-Ford Algorithm on a graph with negative cycles (4)") {
    wtl::SimpleGraph_Weighted<true, int> graph(10);
    graph.addEdge(0, 1, 5);
    graph.addEdge(1, 6, 60);
    graph.addEdge(1, 5, 30);
    graph.addEdge(1, 2, 20);
    graph.addEdge(2, 3, 10);
    graph.addEdge(2, 4, 75);
    graph.addEdge(3, 2, -15);
    graph.addEdge(4, 9, 100);
    graph.addEdge(5, 6, 5);
    graph.addEdge(5, 8, 50);
    graph.addEdge(5, 4, 25);
    graph.addEdge(6, 7, -50);
    graph.addEdge(7, 8, -10);

    wtl::BellmanFord bellmanFord(graph, 0);

    SECTION("Check if negative cycle was detected") {
      REQUIRE(bellmanFord.hasNegCycle() == true);
    }

}

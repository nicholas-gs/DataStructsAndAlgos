//
// Created by Nicholas on 1/1/2021.
//

#pragma once

#include <cstddef>
#include <vector>

#include "../catch.hpp"
#include "../../include/Graphs/Edges/Edges.h"
#include "../../include/Graphs/SimpleGraph_Weighted.h"
#include "../../include/Graphs/LazyDijkstra.h"

TEST_CASE("Lazy Dijkstra's Algorithm on directed graph (1)") {
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

    using Edge = wtl::WeightedDirectedEdge;

    SECTION("Getting shortest path between 2 vertices that exists (1)") {
        std::vector<Edge> refSP;
        refSP.emplace_back(0, 2, 1);
        refSP.emplace_back(2, 1, 3);
        refSP.emplace_back(1, 3, 3);
        wtl::LazyDijkstra dijkstra(graph, 0);
        std::vector<Edge> result = dijkstra.pathTo(3);
        REQUIRE(result == refSP);
        REQUIRE(dijkstra.distanceTo(3) == 7);
    }

    SECTION("Getting shortest path between 2 vertices that exists (2)") {
        std::vector<Edge> refSP;
        refSP.emplace_back(3, 2, 3);
        refSP.emplace_back(2, 1, 3);
        wtl::LazyDijkstra dijkstra(graph, 3);
        std::vector<Edge> result = dijkstra.pathTo(1);
        REQUIRE(result == refSP);
        REQUIRE(dijkstra.distanceTo(1) == 6);
    }

    SECTION("Getting shortest path between 2 vertices that don't exist") {
        wtl::LazyDijkstra dijkstra(graph, 1);
        REQUIRE_THROWS(dijkstra.pathTo(0));
    }

}

TEST_CASE("Lazy Dijkstra's Algorithm on directed graph (2)") {
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

    using Edge = wtl::WeightedDirectedEdge;

    SECTION("Getting shortest path between 2 vertices that exists (1)") {
        std::vector<Edge> refSP;
        refSP.emplace_back(4, 5, 4);
        refSP.emplace_back(5, 2, 1);
        refSP.emplace_back(2, 3, 3);
        wtl::LazyDijkstra lazyDijkstra(graph, 4);
        std::vector<Edge> result = lazyDijkstra.pathTo(3);
        REQUIRE(result == refSP);
        REQUIRE(lazyDijkstra.distanceTo(3) == 8);
    }

    SECTION("Getting shortest path between 2 vertices that exists (2)") {
        std::vector<Edge> refSP;
        refSP.emplace_back(4, 5, 4);
        refSP.emplace_back(5, 2, 1);
        refSP.emplace_back(2, 6, 11);
        wtl::LazyDijkstra lazyDijkstra(graph, 4);
        std::vector<Edge> result = lazyDijkstra.pathTo(6);
        REQUIRE(refSP == result);
        REQUIRE(lazyDijkstra.distanceTo(6) == 16);
    }

    SECTION("Getting shortest path between 2 vertices that don't exist") {
        wtl::LazyDijkstra dijkstra(graph, 1);
        REQUIRE_THROWS(dijkstra.pathTo(0));
    }

}

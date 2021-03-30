//
// Created by Nicholas on 11 Jan 2021.
//

#pragma once

#include "catch.hpp"
#include "FloydWarshall.h"

TEST_CASE("Floyd-Warshall on simple graph") {
    wtl::SimpleGraph_Weighted<true> graph(4);
    graph.addEdge(0, 2, -2);
    graph.addEdge(1, 0, 4);
    graph.addEdge(1, 2, 3);
    graph.addEdge(2, 3, 2);
    graph.addEdge(3, 1, -1);

    wtl::FloydWarshall floydWarshall(graph);

    SECTION("Checking shortest distance") {
        std::vector<double> ref = {0, -1, -2, 0, 4, 0, 2, 4, 5, 1, 0, 2, 3, -1, 1, 0};
        std::vector<double> result;
        for (std::size_t i = 0; i < 4; i++) {
            for (std::size_t j = 0; j < 4; j++) {
                result.push_back(floydWarshall.distanceTo(i, j));
            }
        }
        REQUIRE_THAT(result, Catch::Matchers::Equals(ref));
    }

}

TEST_CASE("Floyd-Warshall on graph with negative cycle (1)") {
    wtl::SimpleGraph_Weighted<true> graph(7);
    graph.addEdge(0, 1, 4);
    graph.addEdge(0, 6, 2);
    graph.addEdge(1, 1, -1);
    graph.addEdge(1, 2, 3);
    graph.addEdge(2, 3, 3);
    graph.addEdge(2, 4, 1);
    graph.addEdge(3, 5, -2);
    graph.addEdge(4, 5, 2);
    graph.addEdge(6, 4, 2);

    wtl::FloydWarshall floydWarshall(graph);

    SECTION("Checking shortest distance") {
        REQUIRE(floydWarshall.distanceTo(0, 6) == 2);
        REQUIRE(floydWarshall.distanceTo(0, 2) == -std::numeric_limits<double>::infinity());
        REQUIRE(floydWarshall.distanceTo(1, 1) == -std::numeric_limits<double>::infinity());
        REQUIRE(floydWarshall.distanceTo(1, 4) == -std::numeric_limits<double>::infinity());
        REQUIRE(floydWarshall.distanceTo(2, 5) == 1);
    }

    SECTION("Checking if path exists") {
        REQUIRE(floydWarshall.hasPathTo(2, 0) == false);
        REQUIRE(floydWarshall.hasPathTo(3, 6) == false);
        for (std::size_t i = 0; i < 7; i++) {
            REQUIRE(floydWarshall.hasPathTo(0, i) == true);
        }

    }
}

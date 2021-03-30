//
// Created by Nicholas on 28 Dec 2020.
//

#pragma once

#include "catch.hpp"
#include "SimpleGraph_Unweighted.h"
#include "ConnectedComponents.h"
#include <vector>

TEST_CASE("Connected Components") {
    wtl::SimpleGraph_Unweighted<false> graph(13);
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(0, 5);
    graph.addEdge(0, 6);
    graph.addEdge(3, 4);
    graph.addEdge(3, 5);
    graph.addEdge(4, 6);
    graph.addEdge(4, 5);

    graph.addEdge(7, 8);

    graph.addEdge(9, 10);
    graph.addEdge(9, 11);
    graph.addEdge(9, 12);
    graph.addEdge(11, 12);

    wtl::ConnectedComponents connectedComponents(graph);

    SECTION("Checking number of CC") {
        REQUIRE(connectedComponents.getCount() == 3);
    }

    SECTION("Getting vertices in CC") {
        REQUIRE_THAT(connectedComponents.set(connectedComponents.belongs(0)),
                     Catch::Matchers::UnorderedEquals(std::vector<std::size_t>{0, 1, 2, 3, 4, 5, 6}));

        REQUIRE_THAT(connectedComponents.set(connectedComponents.belongs(7)),
                     Catch::Matchers::UnorderedEquals(std::vector<std::size_t>{7, 8}));

        REQUIRE_THAT(connectedComponents.set(connectedComponents.belongs(9)),
                     Catch::Matchers::UnorderedEquals(std::vector<std::size_t>{9, 10, 11, 12}));
    }

}

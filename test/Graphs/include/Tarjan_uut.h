//
// Created by Nicholas on 4 Jan 2021.
//

#pragma once

#include <cstddef>
#include "catch.hpp"
#include "SimpleGraph_Unweighted.h"
#include "Tarjan.h"

TEST_CASE("Tarjan's algorithm (1)") {

    wtl::SimpleGraph_Unweighted<true> graph(13);
    graph.addEdge(0, 1);
    graph.addEdge(0, 5);
    graph.addEdge(2, 0);
    graph.addEdge(2, 3);
    graph.addEdge(3, 2);
    graph.addEdge(3, 5);
    graph.addEdge(4, 2);
    graph.addEdge(4, 3);
    graph.addEdge(5, 4);
    graph.addEdge(6, 0);
    graph.addEdge(6, 4);
    graph.addEdge(6, 9);
    graph.addEdge(7, 6);
    graph.addEdge(7, 8);
    graph.addEdge(8, 7);
    graph.addEdge(8, 9);
    graph.addEdge(9, 10);
    graph.addEdge(9, 11);
    graph.addEdge(10, 12);
    graph.addEdge(11, 4);
    graph.addEdge(11, 12);
    graph.addEdge(12, 9);

    wtl::Tarjan tarjan1(graph);
    wtl::Tarjan tarjan = tarjan1;

    REQUIRE(tarjan.getCount() == 5);
    REQUIRE_THAT(tarjan.set(tarjan.belongs(1)),
                 Catch::Matchers::UnorderedEquals(std::vector<std::size_t>{1}));
    REQUIRE_THAT(tarjan.set(tarjan.belongs(0)),
                 Catch::Matchers::UnorderedEquals(std::vector<std::size_t>{0, 2, 3, 4, 5}));
    REQUIRE_THAT(tarjan.set(tarjan.belongs(10)),
                 Catch::Matchers::UnorderedEquals(std::vector<std::size_t>{9, 10, 11, 12}));
    REQUIRE_THAT(tarjan.set(tarjan.belongs(6)),
                 Catch::Matchers::UnorderedEquals(std::vector<std::size_t>{6}));
    REQUIRE_THAT(tarjan.set(tarjan.belongs(7)),
                 Catch::Matchers::UnorderedEquals(std::vector<std::size_t>{7, 8}));
}

TEST_CASE("Tarjan's algorithm (2)") {
    wtl::SimpleGraph_Unweighted<true> graph(8);
    graph.addEdge(0, 1);
    graph.addEdge(1, 2);
    graph.addEdge(2, 0);
    graph.addEdge(3, 7);
    graph.addEdge(3, 4);
    graph.addEdge(4, 5);
    graph.addEdge(5, 6);
    graph.addEdge(5, 0);
    graph.addEdge(6, 0);
    graph.addEdge(6, 2);
    graph.addEdge(6, 4);
    graph.addEdge(7, 3);
    graph.addEdge(7, 5);

    wtl::Tarjan tarjan1(graph);
    wtl::Tarjan tarjan = tarjan1;

    REQUIRE(tarjan.getCount() == 3);
    REQUIRE_THAT(tarjan.set(tarjan.belongs(0)), Catch::Matchers::UnorderedEquals(std::vector<std::size_t>{0, 1, 2}));
    REQUIRE_THAT(tarjan.set(tarjan.belongs(3)), Catch::Matchers::UnorderedEquals(std::vector<std::size_t>{3, 7}));
    REQUIRE_THAT(tarjan.set(tarjan.belongs(6)), Catch::Matchers::UnorderedEquals(std::vector<std::size_t>{4, 5, 6}));
}

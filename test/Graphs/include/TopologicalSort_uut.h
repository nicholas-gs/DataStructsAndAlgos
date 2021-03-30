//
// Created by Nicholas on 28 Dec 2020.
//

#pragma once

#include <cstddef>
#include "catch.hpp"
#include "SimpleGraph_Unweighted.h"
#include "TopologicalSort.h"

TEST_CASE("Topological Sort") {

    SECTION("Connected Directed acyclic graph") {
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
        wtl::TopologicalSort topologicalSort(graph);
        std::vector<std::size_t> result;
        for (std::size_t i = 0; i < graph.vertex(); i++) {
            result.push_back(i);
        }
        REQUIRE_THAT(topologicalSort.getOrdering(), Catch::Matchers::UnorderedEquals(result));
    }

}

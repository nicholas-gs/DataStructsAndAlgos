//
// Created by Nicholas on 30 Dec 2020.
//

#pragma once


#include <cstddef>
#include <vector>
#include "../catch.hpp"
#include "../../include/Graphs/Edges/Edges.h"
#include "../../include/Graphs/SimpleGraph_Weighted.h"
#include "../../include/Graphs/Kruskal.h"

TEST_CASE("Kruskal's algorithm on a connected graph") {
    wtl::SimpleGraph_Weighted<false> graph(8);
    graph.addEdge(0, 7, 1);
    graph.addEdge(2, 3, 2);
    graph.addEdge(1, 7, 3);
    graph.addEdge(5, 7, 6);
    graph.addEdge(1, 3, 7);
    graph.addEdge(0, 2, 4);
    graph.addEdge(1, 5, 9);
    graph.addEdge(6, 0, 46);
    graph.addEdge(6, 4, 51);
    graph.addEdge(2, 7, 14);
    graph.addEdge(4, 5, 16);
    graph.addEdge(1, 2, 17);
    graph.addEdge(0, 4, 20);
    graph.addEdge(6, 2, 40);
    graph.addEdge(3, 6, 45);
    graph.addEdge(4, 7, 18);

    std::vector<wtl::UndirectedEdge> mstRef;
    mstRef.emplace_back(0, 7, 1);
    mstRef.emplace_back(2, 3, 2);
    mstRef.emplace_back(1, 7, 3);
    mstRef.emplace_back(0, 2, 4);
    mstRef.emplace_back(5, 7, 6);
    mstRef.emplace_back(4, 5, 16);
    mstRef.emplace_back(6, 2, 40);

    wtl::Kruskal kruskal(graph);
    REQUIRE(kruskal.getCount() == 1);
    REQUIRE_THAT(kruskal.edges(), Catch::Matchers::UnorderedEquals(mstRef));
}

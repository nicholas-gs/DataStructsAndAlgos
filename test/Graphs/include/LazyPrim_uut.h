//
// Created by Nicholas on 29 Dec 2020.
//

#pragma once

#include <cstddef>
#include <vector>
#include "catch.hpp"
#include "Edges/Edges.h"
#include "SimpleGraph_Weighted.h"
#include "LazyPrim.h"

TEST_CASE("Lazy Prim's algorithm on a connected graph") {
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

    std::vector<wtl::WeightedUndirectedEdge> mstRef;
    mstRef.emplace_back(0, 7, 1);
    mstRef.emplace_back(2, 3, 2);
    mstRef.emplace_back(1, 7, 3);
    mstRef.emplace_back(0, 2, 4);
    mstRef.emplace_back(5, 7, 6);
    mstRef.emplace_back(4, 5, 16);
    mstRef.emplace_back(6, 2, 40);

    wtl::LazyPrim lazyPrim(graph);
    REQUIRE(lazyPrim.getCount() == 1);
    REQUIRE(lazyPrim.getMSTWeight(lazyPrim.belongs(6)) == 72);
    REQUIRE_THAT(lazyPrim.edges(lazyPrim.belongs(0)), Catch::Matchers::UnorderedEquals(mstRef));
}


TEST_CASE("Lazy Prim's algorithm on a disconnected graph") {

    wtl::SimpleGraph_Weighted<false> graph(8);
    graph.addEdge(0, 1, 1);
    graph.addEdge(0, 2, 5);
    graph.addEdge(0, 3, 2);
    graph.addEdge(1, 2, 3);
    graph.addEdge(3, 2, 6);
    graph.addEdge(1, 3, 4);
    graph.addEdge(4, 5, -5);

    wtl::LazyPrim lazyPrim(graph);
    REQUIRE(lazyPrim.getCount() == 4);

    SECTION("First MST in MSF") {
        // The correct reference MST
        std::vector<wtl::WeightedUndirectedEdge> mstRef;
        mstRef.emplace_back(0, 1, 1);
        mstRef.emplace_back(0, 3, 2);
        mstRef.emplace_back(1, 2, 3);

        REQUIRE_THAT(lazyPrim.vertices(lazyPrim.belongs(3)),
                     Catch::Matchers::UnorderedEquals(std::vector<std::size_t>{0, 1, 2, 3}));
        REQUIRE_THAT(lazyPrim.edges(lazyPrim.belongs(0)),
                     Catch::Matchers::UnorderedEquals(mstRef));
    }

    SECTION("Second MST in MSF") {
        // The correct reference MST
        std::vector<wtl::WeightedUndirectedEdge> mstRef;
        mstRef.emplace_back(4, 5, -5);

        REQUIRE(lazyPrim.getMSTWeight(lazyPrim.belongs(5)) == -5);
        REQUIRE_THAT(lazyPrim.vertices(lazyPrim.belongs(4)),
                     Catch::Matchers::UnorderedEquals(std::vector<std::size_t>{4, 5}));
        REQUIRE_THAT(lazyPrim.edges(lazyPrim.belongs(5)),
                     Catch::Matchers::UnorderedEquals(mstRef));
    }

    SECTION("Last 2 MST in MSF") {
        REQUIRE(lazyPrim.getMSTWeight(lazyPrim.belongs(6)) == 0);
        REQUIRE(lazyPrim.getMSTWeight(lazyPrim.belongs(7)) == 0);
        REQUIRE_THAT(lazyPrim.vertices(lazyPrim.belongs(6)),
                     Catch::Matchers::Equals(std::vector<std::size_t>{6}));
        REQUIRE(lazyPrim.edges(lazyPrim.belongs(7)).size() == 0);
    }

}

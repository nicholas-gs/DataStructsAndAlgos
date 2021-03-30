//
// Created by Nicholas on 28 Dec 2020.
//

#pragma once

#include <cstddef>
#include "catch.hpp"
#include "SimpleGraph_Unweighted.h"
#include "UndirectedCycle.h"

TEST_CASE("Undirected Cycle") {

    SECTION("Single vertex with self-loop") {
        wtl::SimpleGraph_Unweighted<false> graph(1);
        graph.addEdge(0, 0);
        wtl::UndirectedCycle undirectedCycle(graph);
        REQUIRE(undirectedCycle.hasCycle() == true);
    }

    SECTION("3 vertex loop") {
        wtl::SimpleGraph_Unweighted<false> graph(3);
        graph.addEdge(0, 1);
        graph.addEdge(1,2);
        graph.addEdge(0,2);
        wtl::UndirectedCycle undirectedCycle(graph);
        REQUIRE(undirectedCycle.hasCycle() == true);
    }

    SECTION("Unconnected graph with loop") {
        wtl::SimpleGraph_Unweighted<false> graph(13);
        graph.addEdge(0,6);
        graph.addEdge(0,5);
        graph.addEdge(5,3);
        graph.addEdge(4,6);
        graph.addEdge(0,1);
        graph.addEdge(0,2);
        graph.addEdge(7,8);
        graph.addEdge(9,10);
        graph.addEdge(9,11);
        graph.addEdge(9,12);
        graph.addEdge(11,12);
        wtl::UndirectedCycle undirectedCycle(graph);
        REQUIRE(undirectedCycle.hasCycle() == true);
    }

    SECTION("Unconnected graph with no loop") {
        wtl::SimpleGraph_Unweighted<false> graph(13);
        graph.addEdge(0,6);
        graph.addEdge(0,5);
        graph.addEdge(5,3);
        graph.addEdge(4,6);
        graph.addEdge(0,1);
        graph.addEdge(0,2);
        graph.addEdge(7,8);
        graph.addEdge(9,10);
        graph.addEdge(9,11);
        graph.addEdge(11,12);
        wtl::UndirectedCycle undirectedCycle(graph);
        REQUIRE(undirectedCycle.hasCycle() == false);
    }

}

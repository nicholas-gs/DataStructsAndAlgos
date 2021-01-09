//
// Created by Nicholas on 28 Dec 2020.
//

#pragma once

#include <cstddef>
#include "../catch.hpp"
#include "../../include/Graphs/SimpleGraph_Unweighted.h"
#include "../../include/Graphs/DirectedCycle.h"

TEST_CASE("Directed Cycle") {

    SECTION("Single vertex with self-loop") {
        wtl::SimpleGraph_Unweighted<true> graph(1);
        graph.addEdge(0, 0);
        wtl::DirectedCycle directedCycle(graph);
        REQUIRE(directedCycle.hasCycle() == true);
    }

    SECTION("4 vertex graph with loop") {
        wtl::SimpleGraph_Unweighted<true> graph(4);
        graph.addEdge(0, 1);
        graph.addEdge(1, 2);
        graph.addEdge(2, 3);
        graph.addEdge(3, 1);
        wtl::DirectedCycle directedCycle(graph);
        REQUIRE(directedCycle.hasCycle() == true);
    }

    SECTION("7 vertex graph without loop") {
        wtl::SimpleGraph_Unweighted<true> graph(7);
        graph.addEdge(0, 1);
        graph.addEdge(1, 2);
        graph.addEdge(2, 3);
        graph.addEdge(0, 4);
        graph.addEdge(4, 5);
        graph.addEdge(5, 6);
        graph.addEdge(6, 3);
        wtl::DirectedCycle directedCycle(graph);
        REQUIRE(directedCycle.hasCycle() == false);
    }

    SECTION("13 vertex graph without loops") {
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

        wtl::DirectedCycle directedCycle(graph);
        REQUIRE(directedCycle.hasCycle() == false);
    }

    SECTION("13 vertex graph with loop") {
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
        graph.addEdge(9, 8);
        graph.addEdge(9, 10);
        graph.addEdge(9, 11);
        graph.addEdge(9, 12);
        graph.addEdge(11, 12);

        wtl::DirectedCycle directedCycle(graph);
        REQUIRE(directedCycle.hasCycle() == true);
    }

    SECTION("Disconnected graph with loops") {
        wtl::SimpleGraph_Unweighted<true> graph(13);
        graph.addEdge(0, 1);
        graph.addEdge(0, 5);
        graph.addEdge(0, 6);
        graph.addEdge(2, 0);
        graph.addEdge(2, 3);
        graph.addEdge(3, 5);
        graph.addEdge(5, 4);
        graph.addEdge(6, 4);
        graph.addEdge(8, 7);
        graph.addEdge(10, 9);
        graph.addEdge(9, 11);
        graph.addEdge(9, 12);
        graph.addEdge(11, 12);
        graph.addEdge(12, 10);

        wtl::DirectedCycle directedCycle(graph);
        REQUIRE(directedCycle.hasCycle() == true);
    }

}

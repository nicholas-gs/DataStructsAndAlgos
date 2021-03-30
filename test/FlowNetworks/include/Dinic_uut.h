//
// Created by Nicholas on 9 Jan 2021.
//

#pragma once

#include "catch.hpp"
#include "Dinic.h"

TEST_CASE("Dinic's algorithm on flow network (1)") {

    wtl::FlowNetwork<std::size_t> flowNetwork(8);
    flowNetwork.addEdge(0, 1, 10);
    flowNetwork.addEdge(0, 2, 5);
    flowNetwork.addEdge(0, 3, 15);
    flowNetwork.addEdge(1, 4, 9);
    flowNetwork.addEdge(1, 5, 15);
    flowNetwork.addEdge(1, 2, 4);
    flowNetwork.addEdge(2, 5, 8);
    flowNetwork.addEdge(2, 3, 4);
    flowNetwork.addEdge(3, 6, 16);
    flowNetwork.addEdge(4, 7, 10);
    flowNetwork.addEdge(4, 5, 15);
    flowNetwork.addEdge(5, 7, 10);
    flowNetwork.addEdge(5, 6, 15);
    flowNetwork.addEdge(6, 2, 6);
    flowNetwork.addEdge(6, 7, 10);

    wtl::Dinic dinic(flowNetwork, 0, 7);
    REQUIRE(dinic.getMaxFlow() == 28);

    using FlowEdge = wtl::FlowEdge<std::size_t>;

    SECTION("Getting min cut vertices") {
        std::vector<std::size_t> refMinCut = {0, 2, 3, 6};
        std::vector<std::size_t> minCut = dinic.getMinCutVertices();
        REQUIRE_THAT(minCut, Catch::Matchers::UnorderedEquals(refMinCut));
    }

    SECTION("Getting the min cut edges") {
        std::vector<FlowEdge> minCut = dinic.getMinCutEdges();
        // The sum of flow of the min cut edges must be equal to the max flow value
        std::size_t minCutVal = 0;
        for (const FlowEdge flowEdge : minCut) {
            minCutVal += flowEdge.getFlow();
        }
        REQUIRE(minCutVal == 28);
    }

}

TEST_CASE("Dinic's algorithm on flow network (2)") {

    wtl::FlowNetwork<double> flowNetwork(11);
    flowNetwork.addEdge(0, 1, 7);
    flowNetwork.addEdge(0, 2, 2);
    flowNetwork.addEdge(0, 3, 1);
    flowNetwork.addEdge(1, 4, 2);
    flowNetwork.addEdge(1, 5, 4);
    flowNetwork.addEdge(2, 5, 5);
    flowNetwork.addEdge(2, 6, 6);
    flowNetwork.addEdge(3, 4, 4);
    flowNetwork.addEdge(3, 8, 8);
    flowNetwork.addEdge(4, 7, 7);
    flowNetwork.addEdge(4, 8, 1);
    flowNetwork.addEdge(5, 7, 3);
    flowNetwork.addEdge(5, 9, 3);
    flowNetwork.addEdge(5, 6, 8);
    flowNetwork.addEdge(6, 9, 3);
    flowNetwork.addEdge(7, 10, 1);
    flowNetwork.addEdge(8, 10, 3);
    flowNetwork.addEdge(9, 10, 4);

    wtl::Dinic dinic(flowNetwork, 0, 10);
    REQUIRE(dinic.getMaxFlow() == 7);

    SECTION("Getting min cut vertices") {
        std::vector<std::size_t> refMinCut = {0, 1, 2, 4, 5, 6, 7, 9};
        std::vector<std::size_t> minCut = dinic.getMinCutVertices();
        REQUIRE_THAT(minCut, Catch::Matchers::UnorderedEquals(refMinCut));
    }

    using FlowEdge = wtl::FlowEdge<double>;

    SECTION("Getting the min cut edges") {
        std::vector<FlowEdge> minCut = dinic.getMinCutEdges();

        // The sum of flow of the min cut edges must be equal to the max flow value
        double minCutVal = 0;
        for (const auto& e : minCut) {
            minCutVal += e.getFlow();
        }
        REQUIRE(minCutVal == 7);
    }

}

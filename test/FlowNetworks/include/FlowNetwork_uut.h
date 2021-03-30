//
// Created by Nicholas on 8 Jan 2021.
//

#pragma once

#include "catch.hpp"
#include "FlowNetwork.h"

TEST_CASE("Flow Network") {
    wtl::FlowNetwork<int> flowNetwork(4);
    REQUIRE(flowNetwork.addEdge(0, 1, 10) == true);
    REQUIRE(flowNetwork.addEdge(0, 2, 11) == true);
    REQUIRE(flowNetwork.addEdge(2, 1, 12) == true);

    SECTION("Inserting duplicate edges") {
        REQUIRE(flowNetwork.addEdge(0, 1, 5) == false);
        REQUIRE(flowNetwork.addEdge(1, 0, 5) == false);
    }

    SECTION("Checking that edges exists") {
        REQUIRE(flowNetwork.hasEdge(0, 1) == true);
        REQUIRE(flowNetwork.hasEdge(1, 0) == true);
        REQUIRE(flowNetwork.hasEdge(0, 1) == true);
        REQUIRE_THROWS(flowNetwork.getEdge(0, 3));
        auto e = flowNetwork.getEdge(1, 2);
        REQUIRE(e.source() == 2);
        REQUIRE(e.dest() == 1);
        REQUIRE(e.getCapacity() == 12);
    }

}

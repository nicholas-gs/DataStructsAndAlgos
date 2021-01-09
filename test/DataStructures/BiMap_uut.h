//
// Created by Nicholas on 22 Dec 2020.
//

#pragma once

#include "../catch.hpp"
#include "../../include/DataStructures/BiMap.h"
#include <string>
#include <cstddef>
#include <vector>
#include <optional>

TEST_CASE("BiMap Test") {
    wtl::BiMap<std::string, int> biMap;
    for (std::size_t i = 0; i < 10; i++) {
        biMap.insert(std::to_string(i), i);
    }

    SECTION("Checking if element exists") {
        for (std::size_t i = 0; i < 10; i++) {
            REQUIRE(biMap.find(std::to_string(i)).value() == i);
            REQUIRE(biMap.find(i).value().get() == std::to_string(i));
        }
        REQUIRE(biMap.find(100) == std::nullopt);
        REQUIRE(biMap.find("100") == std::nullopt);
    }


    SECTION("Erasing elements") {
        biMap.erase(9);
        biMap.erase("8");
        for (std::size_t i = 0; i < 8; i++) {
            REQUIRE(biMap.find(std::to_string(i)).value() == i);
            REQUIRE(biMap.find(i).value().get() == std::to_string(i));
        }
        REQUIRE(biMap.find(8) == std::nullopt);
        REQUIRE(biMap.find("9") == std::nullopt);
    }

}

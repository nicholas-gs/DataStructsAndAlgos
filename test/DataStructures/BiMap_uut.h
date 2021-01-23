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

TEST_CASE("BiMap Test with different types") {
    wtl::BiMap<std::string, int> biMap;
    for (std::size_t i = 0; i < 10; i++) {
        biMap.insert(std::to_string(i), i);
    }

    SECTION("Checking if element exists") {
        for (std::size_t i = 0; i < 10; i++) {
            REQUIRE(biMap.find(std::to_string(i)).value() == static_cast<int>(i));
            REQUIRE(biMap.find(i).value().get() == std::to_string(i));
        }
        REQUIRE(biMap.find(100) == std::nullopt);
        REQUIRE(biMap.find("100") == std::nullopt);
    }

    SECTION("Erasing elements") {
        biMap.erase(9);
        biMap.erase("8");
        for (std::size_t i = 0; i < 8; i++) {
            REQUIRE(biMap.find(std::to_string(i)).value() == static_cast<int>(i));
            REQUIRE(biMap.find(i).value().get() == std::to_string(i));
        }
        REQUIRE(biMap.find(8) == std::nullopt);
        REQUIRE(biMap.find("9") == std::nullopt);
    }

}

TEST_CASE("BiMap Test with same types") {
    wtl::BiMap<int, int> biMap;
    for (std::size_t i = 0; i < 10; i++) {
        biMap.insert(i, i * 2);
    }

    SECTION("Checking if element exists") {
        for (std::size_t i = 0; i < 10; i++) {
            REQUIRE(biMap.find(i, wtl::bimap_left_tag).value() == static_cast<int>(i * 2));
            REQUIRE(biMap.find(i * 2, wtl::bimap_right_tag).value() == static_cast<int>(i));
        }
        REQUIRE(biMap.find(100, wtl::bimap_left_tag) == std::nullopt);
        REQUIRE(biMap.find(100, wtl::bimap_right_tag) == std::nullopt);
    }

    SECTION("Erasing elements") {
        REQUIRE(biMap.erase(9, wtl::bimap_left_tag) == true);
        REQUIRE(biMap.erase(16, wtl::bimap_right_tag) == true);
        for (std::size_t i = 0; i < 8; i++) {
            REQUIRE(biMap.find(i, wtl::bimap_left_tag).value() == static_cast<int>(i * 2));
            REQUIRE(biMap.find(i * 2, wtl::bimap_right_tag).value() == static_cast<int>(i));
        }
        REQUIRE(biMap.find(9, wtl::bimap_left_tag) == std::nullopt);
        REQUIRE(biMap.find(16, wtl::bimap_right_tag) == std::nullopt);
    }

}

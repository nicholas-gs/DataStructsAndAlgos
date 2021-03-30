//
// Created by Nicholas on 6/12/2020.
//

#pragma once

#include <cstddef>
#include "Array.h"
#include "catch.hpp"
#include "Entity.h"

TEST_CASE("Array storing fundamental type") {

    constexpr std::size_t SIZE = 10;
    wtl::Array<double, SIZE> sa;
    for (std::size_t i = 0; i < SIZE; i++) {
        sa[i] = 0.5 * i;
    }

    SECTION("Checking static array size") {
        REQUIRE(sa.getSize() == SIZE);
    }

    SECTION("Retrieving items from static array") {
        REQUIRE(sa.at(0) == (0 * 0.5));
        REQUIRE(sa.at(4) == (4 * 0.5));
        REQUIRE(sa.at(6) == (6 * 0.5));
        REQUIRE(sa[4] == (4 * 0.5));
        REQUIRE(sa.front() == (0 * 0.5));
        REQUIRE(sa.back() == ((SIZE - 1) * 0.5));
    }

}

TEST_CASE("Array storing custom class type") {

}


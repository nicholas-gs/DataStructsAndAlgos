//
// Created by Nicholas on 15 Dec 2020.
//

#pragma once

#include <cstddef>
#include "../catch.hpp"
#include "../../include/DataStructures/FenwickTree.h"

TEST_CASE("Fenwick tree") {
    constexpr int length = 12;
    std::vector<double> orgArr = {3, 4, -2, 7, 3, 11, 5, -8, -9, 2, 4, -8};
    wtl::FenwickTree<double> ft(orgArr);

    SECTION("Retrieving original elements from Fenwick Tree") {
        std::vector<double> vec;
        for (std::size_t i = 0; i < length; i++) {
            vec.push_back(ft.retrieve(i));
        }
        REQUIRE_THAT(vec, Catch::Matchers::Equals(orgArr));
    }

    SECTION("Point queries") {
        REQUIRE(ft.query(0) == 3);
        REQUIRE(ft.query(2) == 5);
        REQUIRE(ft.query(5) == 26);
        REQUIRE(ft.query(9) == 16);
        REQUIRE(ft.query(11) == 12);
    }

    SECTION("Range query") {
        REQUIRE(ft.query(0, 5) == 26);
        REQUIRE(ft.query(2, 5) == 19);
        REQUIRE(ft.query(7, 10) == -11);
        REQUIRE(ft.query(1, 1) == 4);
        REQUIRE(ft.query(0, 0) == 3);
        REQUIRE(ft.query(11, 11) == -8);
    }

    SECTION("Updating Fenwick Tree") {
        ft.replace(1, 8);
        ft.replace(6, 10);
        ft.update(9, -4);
        REQUIRE(ft.retrieve(9) == -2);
        REQUIRE(ft.retrieve(6) == 10);
        REQUIRE(ft.query(6) == 40);
        REQUIRE(ft.query(0, 5) == 30);
        REQUIRE(ft.query(7, 10) == -15);
        REQUIRE(ft.query(1, 1) == 8);
    }
}


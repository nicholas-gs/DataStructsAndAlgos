//
// Created by Nicholas on 17/12/2020.
//

#pragma once

#include "catch.hpp"
#include <cstddef>
#include <vector>
#include "SparseTable.h"

TEST_CASE("Operation on min sparse table") {
    std::vector<double> vector = {5, 2.1, 7, -9.1, 6.6, 2.9, 0, -10.1, -12.3, 4.1, 5.6, -7.9};
    wtl::SparseTable<double> st(vector.begin(), vector.end(), wtl::SparseTable<double>::Operation::MIN);

    SECTION("Check min values over a range") {
        REQUIRE(st.query(0, 11) == -12.3);
        REQUIRE(st.query(0, 2) == 2.1);
        REQUIRE(st.query(0, 6) == -9.1);
        REQUIRE(st.query(4, 6) == 0);
        REQUIRE(st.query(3, 9) == -12.3);
        REQUIRE(st.query(10, 11) == -7.9);
    }

    SECTION("Check min values over for single index") {
        REQUIRE(st.query(0, 0) == vector[0]);
        REQUIRE(st.query(11, 11) == vector[11]);
    }

}

TEST_CASE("Operation on max sparse table") {
    std::vector<double> vector = { 5, 2.1, 7, -9.1, 6.6, 2.9, 0, -10.1, -12.3, 4.1, 5.6, -7.9 };
    wtl::SparseTable<double> st(vector.begin(), vector.end(), wtl::SparseTable<double>::Operation::MAX);

    SECTION("Check min values over a range") {
        REQUIRE(st.query(0, 11) == 7);
        REQUIRE(st.query(0, 2) == 7);
        REQUIRE(st.query(0, 6) == 7);
        REQUIRE(st.query(4, 6) == 6.6);
        REQUIRE(st.query(3, 9) == 6.6);
        REQUIRE(st.query(10, 11) == 5.6);
    }

    SECTION("Check min values over for single index") {
        REQUIRE(st.query(0, 0) == vector[0]);
        REQUIRE(st.query(11, 11) == vector[11]);
    }
}

//
// Created by Nicholas on 14/12/2020.
//

#pragma once

#include "UnionFind.h"
#include "catch.hpp"

TEST_CASE("Union Find") {
    wtl::UnionFind uf(8);

    REQUIRE(uf.getSize() == 8);
    REQUIRE(uf.getSets() == 8);

    REQUIRE(uf.join(0, 1) == true);
    REQUIRE(uf.join(2, 0) == true);
    REQUIRE(uf.join(3, 5) == true);
    REQUIRE(uf.join(6, 3) == true);
    REQUIRE(uf.join(5, 6) == false);
    REQUIRE(uf.join(1, 0) == false);
    REQUIRE(uf.join(4, 4) == false);

    REQUIRE(uf.getSets() == 4);
    REQUIRE(uf.connected(2, 1) == true);
    REQUIRE(uf.connected(1, 2) == true);
    REQUIRE(uf.connected(6, 5) == true);
    REQUIRE(uf.connected(2, 3) == false);
    REQUIRE(uf.connected(0, 4) == false);
    REQUIRE(uf.connected(1, 1) == true);
    REQUIRE(uf.connected(7, 7) == true);

    REQUIRE(uf.join(5, 1) == true);
    REQUIRE(uf.getSets() == 3);
}

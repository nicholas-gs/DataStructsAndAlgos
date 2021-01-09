//
// Created by Nicholas on 31/12/2020.
//

#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "../catch.hpp"
#include "../util/Entity.h"
#include "../../include/DataStructures/IndexedPriorityQueue.h"

TEST_CASE("Indexed Priority Queue in descending order") {

    using Entity = wtl_test::Entity;

    constexpr int size = 8;
    wtl::IndexedPriorityQueue<std::string, Entity> ipq;
    for (int i = 0; i < size; i++) {
        ipq.emplace(std::to_string(i), i);
    }

    REQUIRE(ipq.getSize() == size);

    SECTION("Checking if keys exists") {
        for (int i = 0; i < size; i++) {
            REQUIRE(ipq.exists(std::to_string(i)) == true);
            REQUIRE(ipq.find(std::to_string(i)).m_Id == i);
        }
    }

    SECTION("Removing and polling keys") {
        REQUIRE_NOTHROW(ipq.remove("6"));
        REQUIRE_NOTHROW(ipq.remove("7"));
        REQUIRE(ipq.getSize() == 6);

        int i = 5;
        while (!ipq.isEmpty()) {
            const auto result = ipq.peek();
            REQUIRE(result.first.get() == std::to_string(i));
            REQUIRE(result.second.get().m_Id == (i));
            --i;
            ipq.poll();
        }
    }

    SECTION("Updating priority of elements") {
        // Reverse priority of elements
        for (std::size_t i = 0; i < 8; i++) {
            REQUIRE_NOTHROW(ipq.update(std::to_string(i), Entity(7 - i)));
        }

        int i = 0;
        while (!ipq.isEmpty()) {
            const auto result = ipq.peek();
            REQUIRE(result.first.get() == std::to_string(i));
            REQUIRE(result.second.get().m_Id == (7 - i));
            ++i;
            ipq.poll();
        }
    }

}

//
// Created by Nicholas on 9/1/2021.
//

#pragma once

#include "catch.hpp"
#include "Entity.h"
#include "Queue.h"

TEST_CASE("Queue with int") {

    constexpr std::size_t size = 6;
    wtl::Queue<int> temp;
    for (std::size_t i = 0; i < size; i++) {
        temp.enqueue(i);
    }
    wtl::Queue<int> queue = temp;
    REQUIRE(queue.getSize() == size);

    SECTION("Removing elements from queue") {
        for (std::size_t i = 0; i < size; i++) {
            REQUIRE(queue.peek() == static_cast<int>(i));
            queue.dequeue();
        }
        REQUIRE(queue.isEmpty() == true);
    }

}

TEST_CASE("Queue with custom objects") {

    constexpr std::size_t size = 6;
    using Entity = wtl_test::Entity;
    wtl::Queue<Entity> temp;
    for (std::size_t i = 0; i < size; i++) {
        temp.emplace(i);
    }
    wtl::Queue<Entity> queue = temp;
    REQUIRE(queue.getSize() == size);

    SECTION("Removing elements from queue") {
        for (std::size_t i = 0; i < size; i++) {
            REQUIRE(queue.peek() == Entity(i));
            queue.dequeue();
        }
        REQUIRE(queue.isEmpty() == true);
    }

}

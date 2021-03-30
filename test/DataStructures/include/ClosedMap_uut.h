//
// Created by Nicholas on 21 Dec 2020.
//

#pragma once

#include "catch.hpp"
#include <cstddef>
#include <string>
#include <vector>
#include <utility>
#include "ClosedMap.h"
#include "Entity.h"

TEST_CASE("ClosedMap test"){

    wtl::ClosedMap<std::string, wtl_test::Entity> closedMap;
    for(std::size_t i = 0; i < 16; i++){
        closedMap.insert(std::to_string(i), wtl_test::Entity(i));
    }

    SECTION("Searching for object"){
        REQUIRE(closedMap.getSize() == 16);
        for(std::size_t i = 0; i < 16; i++){
            REQUIRE(closedMap.at(std::to_string(i)).value().get().m_Id == static_cast<int>(i));
        }
        REQUIRE(closedMap.at("-1").has_value() == false);
        REQUIRE(closedMap.at("100").has_value() == false);
    }

    SECTION("Removing objects"){
        closedMap.erase("0");
        closedMap.erase("1");
        closedMap.erase("2");
        closedMap.erase("3");
        REQUIRE(closedMap.getSize() == 12);
        for(std::size_t i = 0; i < 4; i++){
            REQUIRE(closedMap.contains(std::to_string(i)) == false);
        }
        for(std::size_t i = 4; i < 16   ; i++){
            REQUIRE(closedMap.at(std::to_string(i)).value().get().m_Id == static_cast<int>(i));
        }
    }

}

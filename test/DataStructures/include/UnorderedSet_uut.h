//
// Created by Nicholas on 20 Dec 2020.
//

#pragma once

#include "catch.hpp"
#include <cstddef>
#include <string>
#include <vector>
#include <utility>
#include "UnorderedSet.h"

TEST_CASE("Unordered set test") {
    wtl::UnorderedSet<std::string> unorderedSet;
    std::vector<std::string> names = {"Amy", "Barry", "Cindy", "Derek", "Emily", "Felicity", "Harry", "John",
                                      "Nicholas", "Mary", "Zen"};
    for (std::string& name : names) {
        unorderedSet.insert(name);
    }

    SECTION("Inserting not present element") {
        auto result = unorderedSet.insert("Ken");
        REQUIRE(result.second == true);
        REQUIRE(result.first.get() == "Ken");
        REQUIRE(unorderedSet.getSize() == names.size() + 1);
    }

    SECTION("Inserting already present element") {
        auto result = unorderedSet.insert(names[0]);
        REQUIRE(result.second == false);
        REQUIRE(result.first.get() == names[0]);
    }

    SECTION("Searching for present elements"){
        for(std::string& name : names){
            auto result = unorderedSet.find(name);
            REQUIRE(result.value().get() == name);
        }
    }

    SECTION("Removing elements"){
        REQUIRE(unorderedSet.erase("Ken") == false);
        REQUIRE(unorderedSet.erase("Zen") == true);
        REQUIRE(unorderedSet.erase("Mary") == true);
        names.pop_back();
        names.pop_back();
        for(std::string& name : names){
            auto result = unorderedSet.find(name);
            REQUIRE(result.value().get() == name);
        }
        REQUIRE(unorderedSet.find("Zen") == std::nullopt);
        REQUIRE(unorderedSet.find("Mary") == std::nullopt);
    }

}

//
// Created by Nicholas on 1 Jan 2021.
//

#pragma once

#include <cstddef>
#include <vector>

#include "../catch.hpp"
#include "../../include/SortingAlgorithms/BubbleSort.h"
#include "../../include/SortingAlgorithms/BucketSort.h"
#include "../../include/SortingAlgorithms/CountingSort.h"
#include "../../include/SortingAlgorithms/HeapSort.h"
#include "../../include/SortingAlgorithms/InsertionSort.h"
#include "../../include/SortingAlgorithms/MergeSort.h"
#include "../../include/SortingAlgorithms/QuickSort.h"
#include "../../include/SortingAlgorithms/RadixSort.h"
#include "../../include/SortingAlgorithms/SelectionSort.h"
#include "../../include/SortingAlgorithms/TreeSort.h"

TEST_CASE("Sorting integer types in ascending order") {

    std::vector<int> vector = {-40, -37, 42, -39, 53, -10, 96, 35, -87, 92, -11, -94, -33, 11, -36, 13, 83, -19, -89,
                               31, -69, 17, 62, 29, 70, -54, -9, -98, -13, -60, -20, -29, -30, 72, 14, 41, 44, 100, -4,
                               51, -18, 76, 47, 61, 79, -8, -72, -64, 95, 88, 19, 98, -85, -77, 24, 99, 78, 6, -22, -31,
                               18, 40, -91, 75, 10, -25, -59, -24, -70, -90, -88, 94, 74, -99, 0, -55, 91, 59, 80, -100,
                               48, -96, 86, -15, -42, 68, 64, 16, 21, -75, 56, -83, -1, 38, 33, 9, -95, -7, 23, -92};

    std::vector<int> sortedVector = {-100, -99, -98, -96, -95, -94, -92, -91, -90, -89, -88, -87, -85, -83, -77, -75,
                                     -72, -70, -69, -64, -60, -59, -55, -54, -42, -40, -39, -37, -36, -33, -31, -30,
                                     -29, -25, -24, -22, -20, -19, -18, -15, -13, -11, -10, -9, -8, -7, -4, -1, 0,
                                     6, 9, 10, 11, 13, 14, 16, 17, 18, 19, 21, 23, 24, 29, 31, 33, 35, 38, 40, 41, 42,
                                     44, 47, 48, 51, 53, 56, 59, 61, 62, 64, 68, 70, 72, 74, 75, 76, 78, 79, 80, 83, 86,
                                     88, 91, 92, 94, 95, 96, 98, 99, 100};

    SECTION("BubbleSort") {
        wtl::BubbleSort<int>::sort(vector);
        REQUIRE(vector == sortedVector);
    }

    SECTION("BucketSort") {
        wtl::BucketSort<int>::sort(vector);
        REQUIRE(vector == sortedVector);
    }

    SECTION("CountingSort") {
        wtl::CountingSort<int>::sort(vector);
        REQUIRE(vector == sortedVector);
    }

    SECTION("HeapSort") {
        wtl::HeapSort<int>::sort(vector);
        REQUIRE(vector == sortedVector);
    }

    SECTION("InsertionSort") {
        wtl::InsertionSort<int>::sort(vector);
        REQUIRE(vector == sortedVector);
    }

    SECTION("MergeSort") {
        wtl::MergeSort<int>::sort(vector);
        REQUIRE(vector == sortedVector);
    }

    SECTION("QuickSort_Hoare") {
        wtl::QuickSort_Hoare<int>::sort(vector);
        REQUIRE(vector == sortedVector);
    }

    SECTION("QuickSort_Lomuto") {
        wtl::QuickSort_Lomuto<int>::sort(vector);
        REQUIRE(vector == sortedVector);
    }

    SECTION("RadixSort") {
        wtl::RadixSort<int>::sort(vector);
        REQUIRE(vector == sortedVector);
    }

    SECTION("SelectionSort") {
        wtl::SelectionSort<int>::sort(vector);
        REQUIRE(vector == sortedVector);
    }

    SECTION("TreeSort") {
        wtl::TreeSort<int>::sort(vector);
        REQUIRE(vector == sortedVector);
    }
}


TEST_CASE("Sorting integer types in descending order") {

    std::vector<int> vector = {-40, -37, 42, -39, 53, -10, 96, 35, -87, 92, -11, -94, -33, 11, -36, 13, 83, -19, -89,
                               31, -69, 17, 62, 29, 70, -54, -9, -98, -13, -60, -20, -29, -30, 72, 14, 41, 44, 100, -4,
                               51, -18, 76, 47, 61, 79, -8, -72, -64, 95, 88, 19, 98, -85, -77, 24, 99, 78, 6, -22, -31,
                               18, 40, -91, 75, 10, -25, -59, -24, -70, -90, -88, 94, 74, -99, 0, -55, 91, 59, 80, -100,
                               48, -96, 86, -15, -42, 68, 64, 16, 21, -75, 56, -83, -1, 38, 33, 9, -95, -7, 23, -92};

    std::vector<int> sortedVector = {100, 99, 98, 96, 95, 94, 92, 91, 88, 86, 83, 80, 79, 78, 76, 75, 74, 72, 70, 68,
                                     64, 62, 61, 59, 56, 53, 51, 48, 47, 44, 42, 41, 40, 38, 35, 33, 31, 29, 24, 23, 21,
                                     19, 18, 17, 16, 14, 13, 11, 10, 9, 6, 0, -1, -4, -7, -8, -9, -10, -11, -13, -15,
                                     -18, -19, -20, -22, -24, -25, -29, -30, -31, -33, -36, -37, -39, -40, -42, -54,
                                     -55, -59, -60, -64, -69, -70, -72, -75, -77, -83, -85, -87, -88, -89, -90, -91,
                                     -92, -94, -95, -96, -98, -99, -100};

    struct Compare {
        bool operator()(int lhs, int rhs) const {
            return lhs > rhs;
        }
    };

    SECTION("BubbleSort") {
        wtl::BubbleSort<int, Compare>::sort(vector);
        REQUIRE(vector == sortedVector);
    }

    SECTION("BucketSort") {
        wtl::BucketSort<int>::sort(vector, wtl::BucketSort<int>::Order::DESCENDING);
        REQUIRE(vector == sortedVector);
    }

    SECTION("CountingSort") {
        wtl::CountingSort<int>::sort(vector, wtl::CountingSort<int>::Order::DESCENDING);
        REQUIRE(vector == sortedVector);
    }

    SECTION("HeapSort") {
        wtl::HeapSort<int, Compare>::sort(vector);
        REQUIRE(vector == sortedVector);
    }

    SECTION("InsertionSort") {
        wtl::InsertionSort<int, Compare>::sort(vector);
        REQUIRE(vector == sortedVector);
    }

    SECTION("MergeSort") {
        wtl::MergeSort<int, Compare>::sort(vector);
        REQUIRE(vector == sortedVector);
    }

    SECTION("QuickSort_Hoare") {
        wtl::QuickSort_Hoare<int, Compare>::sort(vector);
        REQUIRE(vector == sortedVector);
    }

    SECTION("QuickSort_Lomuto") {
        wtl::QuickSort_Lomuto<int, Compare>::sort(vector);
        REQUIRE(vector == sortedVector);
    }

    SECTION("RadixSort") {
        wtl::RadixSort<int>::sort(vector, wtl::RadixSort<int>::Order::DESCENDING);
        REQUIRE(vector == sortedVector);
    }

    SECTION("SelectionSort") {
        wtl::SelectionSort<int, Compare>::sort(vector);
        REQUIRE(vector == sortedVector);
    }

    SECTION("TreeSort") {
        wtl::TreeSort<int, Compare>::sort(vector);
        REQUIRE(vector == sortedVector);
    }
}

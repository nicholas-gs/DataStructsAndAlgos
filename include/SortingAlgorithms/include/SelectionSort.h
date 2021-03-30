//
// Created by Nicholas on 9/12/2020.
//

#pragma once

#include "impl/SortingAlgorithmsImpl.h"
#include <vector>
#include <cstddef>

namespace wtl {

    /**
     * Selection sort algorithm.
     * @tparam T
     */
    template<typename T, typename Comparator = impl::sort::StandardComparator<T>>
    class SelectionSort {
    public:

        /**
         * Sort elements in a vector in-place.
         * Elements are compared using the supplied comparison function compare.
         * @tparam Comparator
         * @param arr
         * @param comparator
         */
        static void sort(std::vector<T>& arr) {
            Comparator comparator;
            if (arr.size() < 2) {
                return;
            }
            for (std::size_t i = 0; i < arr.size(); i++) {
                std::size_t smallestIndex = i;
                for (std::size_t j = i; j < arr.size(); j++) {
                    if (comparator(arr[j], arr[smallestIndex])) {
                        smallestIndex = j;
                    }
                }
                if (smallestIndex != i) {
                    std::swap(arr[i], arr[smallestIndex]);
                }
            }
        }
    };

}
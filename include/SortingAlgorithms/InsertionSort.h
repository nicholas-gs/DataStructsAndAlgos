//
// Created by Nicholas on 9/12/2020.
//

#pragma once

#include "impl/SortingAlgorithmsImpl.h"
#include <cstddef>
#include <vector>

namespace wtl {

    /**
     * Insertion sort algorithm
     * @tparam T
     */
    template<typename T, typename Comparator = impl::sort::StandardComparator<T>>
    class InsertionSort {
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
            for (std::size_t i = 1; i < arr.size(); i++) {
                for (std::size_t j = i; j > 0; j--) {
                    if (comparator(arr[j], arr[j - 1])) {
                        std::swap(arr[j], arr[j - 1]);
                    } else {
                        break;
                    }
                }
            }
        }
    };
}
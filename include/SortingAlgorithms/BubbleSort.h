//
// Created by Nicholas on 9/12/2020.
//

#pragma once

#include "impl/SortingAlgorithmsImpl.h"
#include <cstddef>
#include <vector>

namespace wtl {

    /**
     * Bubble sort algorithm.
     * @tparam T
     */
    template<typename T, typename Comparator = impl::sort::StandardComparator<T>>
    class BubbleSort {
    public:

        /**
         * Sort elements in a vector in-place.
         * Elements are compared using the supplied comparison function compare.
         * @tparam Comparator
         * @param vector
         * @param compare
         */
        static void sort(std::vector<T>& vector) {
            Comparator compare;
            if (vector.size() < 2) {
                return;
            }
            for (std::size_t i = 0; i < vector.size() - 1; i++) {
                bool swapped = false;
                for (std::size_t j = 0; j < vector.size() - 1; j++) {
                    if (!(compare(vector[j], vector[j + 1]))) {
                        std::swap(vector[j], vector[j + 1]);
                        swapped = true;
                    }
                }
                if (!swapped) {
                    break;
                }
            }
        }

    };

}

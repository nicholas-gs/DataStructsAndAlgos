//
// Created by Nicholas on 9/12/2020.
//

#pragma once

#include "impl/SortingAlgorithmsImpl.h"
#include <cstddef>
#include <vector>
#include <utility>

namespace wtl {

    /**
     * Recursive heap sort algorithm
     * @tparam T
     */
    template<typename T, typename Comparator = impl::sort::StandardComparator<T>>
    class HeapSort {
    private:

        static void constructHeap(std::vector<T>& arr, const Comparator& comparator) {
            heapify(arr, 1, arr.size(), comparator);
        }

        static void
        heapify(std::vector<T>& arr, std::size_t root, std::size_t numberOfElements, const Comparator& comparator) {
            if (root * 2 > numberOfElements) {
                return;
            }
            std::size_t leftNode = 2 * root;
            std::size_t rightNode = leftNode + 1;
            heapify(arr, leftNode, numberOfElements, comparator);
            if (rightNode <= numberOfElements) {
                heapify(arr, rightNode, numberOfElements, comparator);
            }
            fixHeap(arr, root, numberOfElements, comparator);
        }

        static void
        fixHeap(std::vector<T>& arr, std::size_t root, std::size_t numberOfElements, const Comparator& comparator) {
            if (root * 2 > numberOfElements) {
                return;
            }
            std::size_t leftNode = 2 * root;
            std::size_t rightNode = leftNode + 1;
            std::size_t childIndex = -1;
            if (rightNode <= numberOfElements) {
                // arr[leftNode - 1] >= arr[rightNode - 1]
                childIndex = (comparator(arr[rightNode - 1], arr[leftNode - 1]) ||
                              !comparator(arr[leftNode - 1], arr[rightNode - 1])) ? leftNode : rightNode;
            } else {
                childIndex = leftNode;
            }
            if (comparator(arr[root - 1], arr[childIndex - 1])) {
                std::swap(arr[root - 1], arr[childIndex - 1]);
                fixHeap(arr, childIndex, numberOfElements, comparator);
            }
        }

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
            std::size_t size = arr.size();
            if (size < 2) {
                return;
            }
            constructHeap(arr, comparator);
            for (std::size_t i = 0; i < size; i++) {
                std::swap(arr[0], arr[size - i - 1]);
                fixHeap(arr, 1, size - i - 1, comparator);
            }
        }
    };

}

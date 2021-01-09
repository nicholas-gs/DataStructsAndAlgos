//
// Created by Nicholas on 9/12/2020.
//

#pragma once

#include "impl/SortingAlgorithmsImpl.h"
#include <vector>
#include <cstddef>

namespace wtl {

    /**
     * Recursive in-place merge sort algorithm
     * @tparam T
     */
    template<typename T, typename Comparator = impl::sort::StandardComparator<T>>
    class MergeSort {
    private:

        static void
        sortIn(std::vector<T>& vector, std::size_t startPos, std::size_t endPos, const Comparator& compare) {
            if (endPos <= startPos) {
                return;
            }
            std::size_t midPos = (startPos + endPos) / 2;
            sortIn(vector, startPos, midPos, compare);
            sortIn(vector, midPos + 1, endPos, compare);
            merge(vector, startPos, endPos, midPos, compare);
        }

        static void merge(std::vector<T>& vector, std::size_t startPos, std::size_t endPos, std::size_t midPos,
                          const Comparator& compare) {
            std::size_t leftArrLength = midPos - startPos + 1;
            std::size_t rightArrLength = endPos - midPos;

            T leftArr[leftArrLength];
            T rightArr[rightArrLength];

            for (std::size_t i = 0; i < leftArrLength; i++) {
                leftArr[i] = std::move(vector[startPos + i]);
            }
            for (std::size_t i = 0; i < rightArrLength; i++) {
                rightArr[i] = std::move(vector[midPos + i + 1]);
            }

            std::size_t leftIndex = 0;
            std::size_t rightIndex = 0;
            std::size_t arrIndex = startPos;

            while (leftIndex < leftArrLength && rightIndex < rightArrLength) {
                if (compare(leftArr[leftIndex], rightArr[rightIndex])) {
                    vector[arrIndex] = std::move(leftArr[leftIndex]);
                    leftIndex++;
                    arrIndex++;
                } else if (compare(rightArr[rightIndex], leftArr[leftIndex])) {
                    vector[arrIndex] = std::move(rightArr[rightIndex]);
                    rightIndex++;
                    arrIndex++;
                } else {
                    vector[arrIndex] = std::move(leftArr[leftIndex]);
                    leftIndex++;
                    arrIndex++;
                    vector[arrIndex] = std::move(rightArr[rightIndex]);
                    rightIndex++;
                    arrIndex++;
                }
            }

            while (leftIndex < leftArrLength) {
                vector[arrIndex] = std::move(leftArr[leftIndex]);
                arrIndex++;
                leftIndex++;
            }

            while (rightIndex < rightArrLength) {
                vector[arrIndex] = std::move(rightArr[rightIndex]);
                arrIndex++;
                rightIndex++;
            }
        }

    public:

        /**
         * Sort elements in a vector in-place.
         * Elements are compared using the supplied comparison function compare.
         * @tparam Compare
         * @param vector
         * @param compare
         */
        static void sort(std::vector<T>& vector) {
            if (vector.size() < 2) {
                return;
            }
            Comparator comparator;
            sortIn(vector, 0, vector.size() - 1, comparator);
        }

    };

}

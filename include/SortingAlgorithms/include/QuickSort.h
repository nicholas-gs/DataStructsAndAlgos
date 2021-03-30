//
// Created by Nicholas on 9/12/2020.
//

#pragma once

#include "impl/SortingAlgorithmsImpl.h"
#include <vector>
#include <cstddef>
#include <utility>
#include <type_traits>

namespace wtl {

    /**
     * Recursive in-place quick sort algorithm using Lomuto's partitioning.
     * @tparam T
     */
    template<typename T, typename Comparator = impl::sort::StandardComparator<T>>
    class QuickSort_Lomuto {
    private:

        static void
        inplaceSort(std::vector<T>& arr, std::size_t startPos, std::size_t endPos, const Comparator& comparator) {
            if (startPos >= endPos) {
                return;
            }
            std::size_t midPos = (startPos + endPos) / 2;
            T pivotVal = std::exchange(arr[midPos], arr[endPos]);

            std::size_t nextSwap = 0;
            for (std::size_t i = 0; i < endPos; i++) {
                // lhs <= rhs
                if (comparator(arr[i], pivotVal) || !comparator(pivotVal, arr[i])) {
                    if (i != nextSwap) {
                        std::swap(arr[i], arr[nextSwap]);
                    }
                    nextSwap++;
                }
            }
            arr[endPos] = std::move(arr[nextSwap]);
            arr[nextSwap] = std::move(pivotVal);
            if (nextSwap != 0) {
                inplaceSort(arr, startPos, nextSwap - 1, comparator);
            }
            inplaceSort(arr, nextSwap + 1, endPos, comparator);
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
            inplaceSort(arr, 0, arr.size() - 1, comparator);
        }
    };

    /**
     * Recursive in-place quick sort algorithm using Hoare's partitioning.
     * @tparam T
     */
    template<typename T,typename Comparator = impl::sort::StandardComparator<T>>
    class QuickSort_Hoare {
    private:

        static void
        quickSort(std::vector<T>& arr, long int startPos, long int endPos, const Comparator& comparator) {
            if (startPos < endPos) {
                long partitionPos = partition(arr, startPos, endPos, comparator);
                quickSort(arr, startPos, partitionPos, comparator);
                quickSort(arr, partitionPos + 1, endPos, comparator);
            }
        }

        // TODO : change algorithm such that negative index not required. Truncation of size_t occurs.
        static long int
        partition(std::vector<T>& arr, long int startPos, long int endPos, const Comparator& comparator) {
            T pivotVal = arr[(startPos + endPos) / 2];
            long int i = startPos - 1;
            long int j = endPos + 1;
            while (true) {
                do {
                    i++;
                } while (comparator(arr[i], pivotVal));
                do {
                    j--;
                } while (comparator(pivotVal, arr[j]));

                if (i < j) {
                    std::swap(arr[i], arr[j]);
                } else {
                    return j;
                }
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
            if (arr.size() < 2) {
                return;
            }
            quickSort(arr, 0, arr.size() - 1, comparator);
        }
    };

}
//
// Created by Nicholas on 10/12/2020.
//

#include <vector>
#include <cstddef>
#include <type_traits>

#pragma once

namespace wtl {

    /**
     * Radix sort that supports negative numbers. Only supports integers.
     * @tparam T
     */
    template<typename T>
    class RadixSort {
    public:

        enum class Order {
            ASCENDING, DESCENDING
        };

        static void sortIn(std::vector<T>& vector, std::vector<T>& elements, std::size_t count, std::size_t startingPos,
                           std::size_t iterations, bool negative) {
            std::vector<T> buckets[10];
            std::size_t radix = 1;

            for (; iterations > 0; iterations--) {
                for (std::size_t i = 0; i < count; i++) {
                    std::size_t index = (elements[i] / radix) % 10;
                    buckets[index].push_back(elements[i]);
                }

                // Insert the values from the bucket back into the elements array in the correct order
                std::size_t index = 0;
                for (auto& bucket : buckets) {
                    for (T val : bucket) {
                        elements[index] = val;
                        index++;
                    }
                    if (bucket.size() != 0) {
                        bucket.clear();
                    }
                }
                radix *= 10;
            }

            // Copy over the values, in the correct order, to the original array at the correct starting index.
            if (!negative) {
                for (std::size_t i = 0; i < count; i++) {
                    vector[startingPos + i] = elements[i];
                }
            } else {
                startingPos = count - 1;
                for (std::size_t i = 0; i < count; i++) {
                    vector[startingPos - i] = -1 * elements[i];
                }
            }
        }

    public:

        static void sort(std::vector<T>& vector, Order order = Order::ASCENDING) {
            static_assert(std::is_integral_v<T>, "Radix sort only works with integrals");

            if (vector.size() < 2) {
                return;
            }

            std::vector<T> positiveNumbers;
            T posMax = 0;
            bool checkedPos = false;
            std::size_t posCount = 0;

            std::vector<T> negativeNumbers;
            T negMax = 1;
            bool checkedNeg = false;
            std::size_t negCount = 0;

            for (std::size_t i = 0; i < vector.size(); i++) {
                if (vector[i] >= 0) {
                    posCount++;
                    positiveNumbers.push_back(vector[i]);
                    if (!checkedPos) {
                        posMax = vector[i];
                        checkedPos = true;
                    } else {
                        if (vector[i] > posMax) {
                            posMax = vector[i];
                        }
                    }
                } else {
                    negCount++;
                    // Convert the negative number into a positive number
                    negativeNumbers.push_back(-1 * vector[i]);
                    if (!checkedNeg) {
                        negMax = vector[i];
                        checkedNeg = true;
                    } else {
                        if (vector[i] < negMax) {
                            negMax = vector[i];
                        }
                    }
                }
            }

            // Get the number of digits of the longest positive number
            std::size_t posIterations = 0;
            for (; posMax > 0; posIterations++) {
                posMax /= 10;
            }

            // Get the number of digits of the longest negative number
            negMax *= -1;
            std::size_t negIterations = 0;
            for (; negMax > 0; negIterations++) {
                negMax /= 10;
            }

            if (posCount > 0) {
                sortIn(vector, positiveNumbers, posCount, negCount, posIterations, false);
            }
            if (negCount > 0) {
                sortIn(vector, negativeNumbers, negCount, 0, negIterations, true);
            }

            if (order == Order::DESCENDING) {
                std::size_t count = vector.size();
                for (std::size_t i = 0; i < count / 2; i++) {
                    std::swap(vector[i], vector[count - i - 1]);
                }
            }
        }
    };

}

//
// Created by Nicholas on 10/12/2020.
//

#pragma once

#include <vector>
#include <cstddef>
#include <cmath>
#include <type_traits>
#include <algorithm>
#include <memory>


namespace wtl {

    /**
     * Bucket sort is a sorting algorithm that distributes elements into buckets,
     * then perform actual sorting on the bucket.
     * Positive and negative and floating point numbers are supported.
     * @tparam T
     */
    template<typename T>
    class BucketSort {
    public:

        enum class Order {
            ASCENDING, DESCENDING
        };

    private:

        static void
        sortIn(std::vector<T>& vector, std::vector<T>& originalVector, std::size_t startPos, T min, T max, bool neg) {
            std::size_t numberOfBuckets = std::ceil(std::sqrt(vector.size()));
            // Create an array of vectors, where each vector is a bucket
            std::unique_ptr<std::vector<T>[]> buckets = std::make_unique<std::vector<T>[]>(numberOfBuckets);
            double bucketSize = ((double) max - (double) min) / (double) numberOfBuckets;

            for (const T& val : vector) {
                std::size_t bid = -1;
                if (min == max) {
                    bid = 0;
                } else {
                    bid = std::floor((val - min) / bucketSize);
                    if (bid >= numberOfBuckets) {
                        bid = numberOfBuckets - 1;
                    }
                }
                buckets[bid].push_back(val);
            }

            if (neg) {
                startPos = vector.size() - 1;
            }
            for (std::size_t i = 0; i < numberOfBuckets; i++) {
                if (buckets[i].size() > 0) {
                    std::sort(buckets[i].begin(), buckets[i].end());
                }
                for (std::size_t j = 0; j < buckets[i].size(); j++) {
                    if (neg) {
                        originalVector[startPos] = buckets[i][j];
                        originalVector[startPos] *= -1;
                        startPos--;
                    } else {
                        originalVector[startPos] = buckets[i][j];
                        startPos++;
                    }
                }
            }
        }

    public:

        static void sort(std::vector<T>& vector, Order order = Order::ASCENDING) {
            if (vector.size() < 2) {
                return;
            }

            std::vector<T> posVector;
            T posMin = 0;
            T posMax = 0;
            bool iteratePos = false;
            std::vector<T> negVector;
            T negMin = -1;
            T negMax = -1;
            bool iterateNeg = false;

            // All elements in the vector are the same
            if (std::adjacent_find(vector.begin(), vector.end(), std::not_equal_to<>()) == vector.end()) {
                return;
            }

            for (const T& val : vector) {
                if (val < 0) {
                    if (iterateNeg) {
                        if (val < negMin) {
                            negMin = val;
                        } else if (val > negMax) {
                            negMax = val;
                        }
                    } else {
                        iterateNeg = true;
                        negMin = negMax = val;
                    }
                    negVector.push_back(-1 * val);
                } else {
                    if (iteratePos) {
                        if (val < posMin) {
                            posMin = val;
                        } else if (val > posMax) {
                            posMax = val;
                        }
                    } else {
                        iteratePos = true;
                        posMin = posMax = val;
                    }
                    posVector.push_back(val);
                }
            }

            sortIn(posVector, vector, negVector.size(), posMin, posMax, false);
            sortIn(negVector, vector, 0, -1 * negMax, -1 * negMin, true);

            if (order == Order::DESCENDING) {
                std::reverse(vector.begin(), vector.end());
            }
        }
    };

}
//
// Created by Nicholas on 10/12/2020.
//

#pragma once

#include <vector>
#include <cstddef>
#include <type_traits>
#include <memory>

namespace wtl {

    /**
     * Counting sort algorithm. Only works for integer data types. Supports both negative and positive numbers.
     * @tparam T
     */
    template<typename T>
    class CountingSort {

    public:
        enum class Order {
            ASCENDING, DESCENDING
        };
    private:

        static void sortIn(std::vector<T>& vector, const T& min, const T& max, Order ascending) {
            std::size_t range = static_cast<std::size_t>(max - min + 1);
            std::unique_ptr<T[]> buckets = std::make_unique<T[]>(range);
            for(std::size_t i =0; i < range; i++) {
                buckets[i] = 0;
            }

            for (const T& element : vector) {
                buckets[element - min] += 1;
            }

            std::size_t index = 0;
            if (ascending == Order::ASCENDING) {
                for (std::size_t i = 0; i < range; i++) {
                    while (buckets[i] > 0) {
                        vector[index] = static_cast<T>(i) + min;
                        index++;
                        buckets[i]--;
                    }
                }
            } else {
                for (std::size_t i = range; i > 0; i--) {
                    while (buckets[i - 1] > 0) {
                        vector[index] = static_cast<T>(i - 1) + min;
                        index++;
                        buckets[i - 1]--;
                    }
                }
            }
        }

    public:

        /**
         * @param vector
         * @param ascending
         */
        static void sort(std::vector<T>& vector, Order ascending = Order::ASCENDING) {
            static_assert(std::is_integral_v<T>, "Counting sort only works with integrals");

            if (vector.size() < 2) {
                return;
            }
            T min = vector[0];
            T max = vector[0];
            for (typename std::vector<T>::iterator it = vector.begin() + 1; it != vector.end(); it++) {
                if (*it < min) {
                    min = *it;
                } else if (*it > max) {
                    max = *it;
                }
            }
            sortIn(vector, min, max, ascending);
        }
    };

}
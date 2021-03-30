//
// Created by Nicholas on 11/12/2020.
//

#pragma once

#include "impl/SearchAlgorithmsImpl.h"
#include <type_traits>
#include <cstddef>
#include <optional>
#include <iterator>
#include <utility>

namespace wtl {

    namespace impl {

        template<typename T>
        struct InterpolationSearch_Comparable{
            T operator()(const T& arg) const {
                return arg;
            }
        };

    }

    /**
     * Interpolation search.
     * @tparam ContainerType Standard library container
     * @tparam Comparable
     * @tparam Comparator
     * @tparam T Type of object stored in the container
     */

    // todo - Fix comparator and comparable template type problem
    template<typename ContainerType, typename Comparable,
            typename Comparator = impl::search::StandardComparator<
                    typename std::iterator_traits<typename ContainerType::const_iterator>::value_type>>
    class InterpolationSearch {
    public:

        using T = typename std::iterator_traits<typename ContainerType::const_iterator>::value_type;

        /**
         * Search the container for the element using the specified binary predicate and specified
         * comparable value.
         * @param container Standard library container
         * @param value Element to search for in the container
         * @param comparator
         * @param comparable
         * @return
         */
        static std::pair<std::optional<typename ContainerType::const_iterator>, std::size_t>
        search(const ContainerType& container, const T& value) {
            std::size_t size = std::distance(container.begin(), container.end());
            if (size == 0) {
                return {std::nullopt, -1};
            }

            impl::search::ComparatorHelper<T, Comparator> comparator;
            Comparable comparable;

            std::size_t low = 0;
            std::size_t high = size - 1;
            typename ContainerType::const_iterator frontIterator = container.begin();
            typename ContainerType::const_iterator lowIterator = frontIterator;
            typename ContainerType::const_iterator highIterator = frontIterator;
            std::advance(lowIterator, low);
            std::advance(highIterator, high);

            // arr[low] <= value && arr[high] >= value
            while (comparator.dualCompare(*lowIterator, value) &&
                   comparator.dualCompare(value, *highIterator)) {
                auto d = comparable(*highIterator) - comparable(*lowIterator);
                if (d != 0) {
                    std::size_t pos = low + (((high - low) / d) * (comparable(value) - comparable(*lowIterator)));
                    std::advance(frontIterator, pos);
                    const T& element = *(frontIterator);
                    // element < value
                    if (comparator(comparable(element), comparable(value))) {
                        low = pos + 1;
                    } else if (comparator.equal(value, element)) { // value == element
                        return {frontIterator, pos};
                    } else {
                        high = pos - 1;
                    }
                } else {
                    if (comparator.equal(*lowIterator, value)) { // arr[low] == value
                        return {lowIterator, low};
                    } else {
                        return {std::nullopt, -1};
                    }
                }

                frontIterator = container.begin();
                lowIterator = frontIterator;
                highIterator = frontIterator;
                std::advance(lowIterator, low);
                std::advance(highIterator, high);
            }
            return {std::nullopt, -1};
        }

    };

}

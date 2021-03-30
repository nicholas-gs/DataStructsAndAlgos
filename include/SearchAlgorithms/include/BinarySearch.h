//
// Created by Nicholas on 10/12/2020.
//

#pragma once

#include "impl/SearchAlgorithmsImpl.h"
#include <type_traits>
#include <cstddef>
#include <optional>
#include <iterator>
#include <utility>

namespace wtl {

    /**
     * Binary search algorithm. Accepts standard library containers.
     * Search using either the operator< or a specified binary predicate.
     * @tparam ContainerType Type of stl container
     * @tparam Comparator binary predicate.
     * @tparam T Type of element stored in the container.
     */
    template<typename ContainerType,
            // typename Comparator = impl::search::StandardComparator<T>
            typename Comparator = impl::search::StandardComparator<
                    typename std::iterator_traits<typename std::remove_reference_t<ContainerType>::const_iterator>::value_type>>
    class BinarySearch {
    public:
        using T = typename std::iterator_traits<typename std::remove_reference_t<ContainerType>::const_iterator>::value_type;

        /**
         * Return a std::pair. The first element is a std::optional that contains a const_iterator
         * that points the element, if it is found. The second element is the index past the begin iterator;
         * which is equal to std::advance(container.begin(), index) or container.being() + index.
         * If element was not found, index is -1.
         * Searches elements using the specified binary predicate.
         * @param container
         * @param value
         * @return
         */
        static std::pair<std::optional<typename std::remove_reference_t<ContainerType>::const_iterator>, std::size_t>
        search(const ContainerType& container, const T& value) {
            std::size_t size = std::distance(container.begin(), container.end());
            if (size == 0) {
                return {std::nullopt, -1};
            }

            std::size_t low = 0;
            std::size_t high = size - 1;
            auto frontIterator = container.begin();
            Comparator comparator;

            while (high >= low) {
                std::size_t mid = (high + low) / 2;
                auto it = frontIterator;
                std::advance(it, mid);
                if (!comparator(*it, value) && !comparator(value, *it)) {
                    return {it, mid};
                } else if (comparator(*it, value)) {
                    low = mid + 1;
                } else {
                    high = mid - 1;
                }
            }
            return {std::nullopt, -1};
        }

    };

}

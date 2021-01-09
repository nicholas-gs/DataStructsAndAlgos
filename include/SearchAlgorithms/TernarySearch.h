//
// Created by Nicholas on 13/12/2020.
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
     *
     * @tparam ContainerType
     * @tparam T
     */
    template<typename ContainerType,
            typename Comparator = impl::search::StandardComparator<
                    typename std::iterator_traits<typename ContainerType::const_iterator>::value_type>,
            typename T = typename std::iterator_traits<typename ContainerType::const_iterator>::value_type>
    class TernarySearch {
    public:

        static std::pair<std::optional<typename ContainerType::const_iterator>, std::size_t>
        search(const ContainerType& container, const T& value) {
            std::size_t size = std::distance(container.begin(), container.end());
            if (size == 0) {
                return {std::nullopt, -1};
            }

            std::size_t low = 0;
            std::size_t high = size - 1;

            impl::search::ComparatorHelper<T, Comparator> comparator;

            while (high >= low) {
                std::size_t lowMid = ((2 * low) + high) / 3;
                std::size_t highMid = (low + (2 * high)) / 3;

                typename ContainerType::const_iterator lowMidIt = container.begin();
                typename ContainerType::const_iterator highMidIt = lowMidIt;
                std::advance(lowMidIt, lowMid);
                std::advance(highMidIt, highMid);

                const T& lowMidVal = *lowMidIt;
                const T& highMidVal = *highMidIt;

                if (comparator.equal(lowMidVal, value)) {
                    return {lowMidIt, lowMid};
                } else if (comparator.equal(highMidVal, value)) {
                    return {highMidIt, highMid};
                } else if (comparator(value, lowMidVal)) {
                    high = lowMid - 1;
                } else if (comparator(highMidVal, value)) {
                    low = highMid + 1;
                } else {
                    low = lowMid + 1;
                    high = highMid - 1;
                }
            }
            return {std::nullopt, -1};
        }
    };


}

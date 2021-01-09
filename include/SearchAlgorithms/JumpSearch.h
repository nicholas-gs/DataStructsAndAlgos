//
// Created by Nicholas on 12/12/2020.
//

#pragma once

#include "impl/SearchAlgorithmsImpl.h"
#include <type_traits>
#include <cstddef>
#include <optional>
#include <iterator>
#include <utility>
#include <cmath>

namespace wtl {

    /**
     * Jump search algorithm
     * @tparam ContainerType stl container type
     * @tparam Comparator Binary predicate
     * @tparam T
     */
    template<typename ContainerType,
            typename Comparator = impl::search::StandardComparator<
                    typename std::iterator_traits<typename ContainerType::const_iterator>::value_type>,
            typename T = typename std::iterator_traits<typename ContainerType::const_iterator>::value_type>
    class JumpSearch {
    public:

        static std::pair<std::optional<typename ContainerType::const_iterator>, std::size_t>
        search(const ContainerType& container, const T& value) {
            std::size_t size = std::distance(container.begin(), container.end());
            if (size == 0) {
                return {std::nullopt, -1};
            }
            std::size_t jump = sqrt(size);
            std::size_t pos = 0;
            impl::search::ComparatorHelper<T, Comparator> comparator;
            while (pos < size) {
                auto it = container.begin();
                std::advance(it, pos);
                const T& element = *it;
                if (comparator(element, value)) {
                    pos += jump;
                } else if (!comparator(value, element)) {
                    return {it, pos};
                } else {
                    break;
                }
            }
            for (std::size_t i = pos - jump + 1; (i < pos) && (i < size) && (i >= 0); i++) {
                auto it = container.begin();
                std::advance(it, i);
                const T& element = *it;
                if (comparator.equal(element, value)) {
                    return {it, i};
                }
            }
            return {std::nullopt, -1};
        }

    };

}

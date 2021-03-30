//
// Created by Nicholas on 15 Dec 2020.
//

#pragma once

namespace wtl::impl::ds {

    /**
     * Standard comparator functor, similar to std::less.
     * @tparam T
     */
    template<typename T>
    struct StandardComparator {
        bool operator()(const T& lhs, const T& rhs) const {
            return lhs < rhs;
        }
    };

    /**
     * Wrapper around a standard binary predicate.
     * @tparam T
     * @tparam Comparator
     */
    template <typename T, typename Comparator>
    struct ComparatorHelper{

        Comparator comparator;

        bool operator()(const T& lhs, const T& rhs) const {
            return comparator(lhs, rhs);
        }

        /**
          * Check if lhs is less than or equal to rhs
          * @param lhs
          * @param rhs
          * @return
          */
        [[nodiscard]] inline bool dualCompare(const T& lhs, const T& rhs) const noexcept {
            return comparator(lhs, rhs) || !comparator(rhs, lhs);
        }

        /**
         * Check if lhs and rhs are equal
         * @param lhs
         * @param rhs
         * @return
         */
        [[nodiscard]]  inline bool equal(const T& lhs, const T& rhs) const noexcept {
            return !comparator(lhs, rhs) && !comparator(rhs, lhs);
        }
    };

}

//
// Created by Nicholas on 21 Dec 2020.
//

#pragma once

#include <cstddef>
#include <map>
#include <functional>
#include <stdexcept>

namespace wtl {

    /**
     * Tag dispatch for left type of BiMap
     */
    struct BiMap_Left_Tag {
    } bimap_left_tag;

    /**
     * Tag dispatch for right type of BiMap
     */
    struct BiMap_Right_Tag {
    } bimap_right_tag;

    /**
     * A BiMap is a bidirectional map that allows for 2-way mapping between elements of types A and B.
     * Since both elements in the pair acts as the "key", they thus have to be both unique. That is,
     * no 2 elements of type A can be the same, and no 2 elements of type B can be the same.
     * BiMap prevents duplication.
     * Elements of type A and B are both immutable after they have been inserted.
     * @tparam A
     * @tparam B
     * @tparam Comparator_A
     * @tparam Comparator_B
     */
    template<typename A, typename B, typename Comparator_A = std::less<A>, typename Comparator_B = std::less<B>>
    class BiMap {
    private:

        using ReferenceA = std::reference_wrapper<const A>;
        using ReferenceB = std::reference_wrapper<const B>;
        using LeftMap = std::map<A, const B*, Comparator_A>;
        using RightMap = std::map<B, const A*, Comparator_B>;

        /// Map from A to B*
        LeftMap m_LeftMap;

        /// Map from B to A*
        RightMap m_RightMap;

    public:

        /**
         * Default constructor
         */
        BiMap() = default;

        /**
         * Copy constructor
         * @param other
         */
        BiMap(const BiMap& other) : m_LeftMap(other.m_LeftMap), m_RightMap(other.m_RightMap) {}

        /**
         * Copy assignment
         * @param other
         * @return
         */
        BiMap& operator=(const BiMap& other) {
            if (this != &other) {
                m_LeftMap = other.m_LeftMap;
                m_RightMap = other.m_RightMap;
            }
            return *this;
        }

        /**
         * Get the number of elements
         * @return
         */
        [[nodiscard]] std::size_t getSize() const noexcept {
            return m_LeftMap.size();
        }

        /**
         * Check if container is empty
         * @return
         */
        [[nodiscard]] bool empty() const noexcept {
            return m_LeftMap == 0;
        }

        /**
         * Insert A and B into Bimap. If either element a or b already exists in the BiMap, then no insertion happens.
         * @param a
         * @param b
         * @return True if insertion occurred, false if duplicate was found
         */
        bool insert(const A& a, const B& b) {
            auto[leftIt, leftResult] = m_LeftMap.insert({a, nullptr});
            // Duplicate element "a" was found
            if (!leftResult) {
                return false;
            }
            // Get pointer to the inserted element "a"
            const A* aPtr = &(leftIt->first);
            auto[rightIt, rightResult] = m_RightMap.insert({b, aPtr});
            // Duplicate element "b" was found
            if (!rightResult) {
                m_LeftMap.erase(a);
                return false;
            }
            leftIt->second = &(rightIt->first);
            return true;
        }

        /**
        * Insert A and B into Bimap. If either element a or b already exists in the BiMap, then no insertion happens.
        * @param a
        * @param b
        * @return True if insertion occurred, false if duplicate was found
        */
        bool insert(A&& a, B&& b) {
            auto[leftIt, leftResult] = m_LeftMap.insert({std::move(a), nullptr});
            // Duplicate element "a" was found
            if (!leftResult) {
                return false;
            }
            // Get pointer to the inserted element "a"
            const A* aPtr = &(leftIt->first);
            auto[rightIt, rightResult] = m_RightMap.insert({std::move(b), aPtr});
            // Duplicate element "b" was found
            if (!rightResult) {
                m_LeftMap.erase(a);
                return false;
            }
            leftIt->second = &(rightIt->first);
            return true;
        }

        /**
         * Get A from element of type B.
         * @param b
         * @return Const reference to the
         */
        [[nodiscard]] std::optional<ReferenceA> find(const B& b, BiMap_Right_Tag = bimap_right_tag) const {
            auto result = m_RightMap.find(b);
            if (result == m_RightMap.end()) {
                return std::nullopt;
            }
            return *(result->second);
        }

        /**
         * Get B from element of type A
         * @param a
         * @return
         */
        [[nodiscard]] std::optional<ReferenceB> find(const A& a, BiMap_Left_Tag = bimap_left_tag) const {
            auto result = m_LeftMap.find(a);
            if (result == m_LeftMap.end()) {
                return std::nullopt;
            }
            return *(result->second);
        }

        /**
         * Check if element of type A exists.
         * @param a
         * @return True if it exists, false if not
         */
        [[nodiscard]] bool contains(const A& a, BiMap_Left_Tag = bimap_left_tag) const {
            return find(a, bimap_left_tag) != std::nullopt;
        }

        /**
         * Check if element of type B exists.
         * @param b
         * @return True if it exists, false if not
         */
        [[nodiscard]] bool contains(const B& b, BiMap_Right_Tag = bimap_right_tag) const {
            return find(b, bimap_right_tag) != std::nullopt;
        }

        /**
         * Erase (A,B) pair using element of type A
         * @param a
         * @return True if pair was removed from BiMap.
         */
        bool erase(const A& a, BiMap_Left_Tag = bimap_left_tag) {
            auto result = m_LeftMap.find(a);
            if (result == m_LeftMap.end()) {
                return false;
            }
            m_RightMap.erase(*(result->second));
            m_LeftMap.erase(result);
            return true;
        }

        /**
         * Erase (A,B) pair using element of type B
         * @param b
         * @return True if pair was removed from BiMap.
         */
        bool erase(const B& b, BiMap_Right_Tag = bimap_right_tag) {
            auto result = m_RightMap.find(b);
            if (result == m_RightMap.end()) {
                return false;
            }
            m_LeftMap.erase(*(result->second));
            m_RightMap.erase(result);
            return true;
        }

        /**
         * Clear the BiMap.
         */
        void clear() noexcept {
            m_LeftMap.clear();
            m_RightMap.clear();
        }

        /**
         * Destructor
         */
        ~BiMap() = default;

    };

}

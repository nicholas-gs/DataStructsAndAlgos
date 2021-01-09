//
// Created by Nicholas on 20 Dec 2020.
//

#pragma once

#include <cstddef>
#include <functional>
#include <list>
#include <utility>

namespace wtl {

    /**
     * Hashmap for unique elements. Unlike a UnorderedMap, this data structure is for only elements of type K.
     * Implemented using a hashmap with closed addressing.
     * @tparam K
     * @tparam Hash
     */
    template<typename K, typename Hash = std::hash<K>>
    class UnorderedSet {
    private:

        static constexpr std::size_t MINIMUM_BUCKET_COUNT = 12;
        static constexpr double MAXIMUM_LOAD_FACTOR = 1;

        /// Number of elements
        std::size_t m_Size = 0;

        /// Number of buckets
        std::size_t m_BucketCount = 0;

        /// Buckets represented as a array of lists
        std::list<K>* m_Buckets = nullptr;

        /// Hashing algorithm
        Hash m_Hash;

        /**
        * Double the number of buckets if the load factor exceeds the threshold. Rehashes the key-value pairs.
        */
        void expand() noexcept {
            if (getLoadFactor() >= MAXIMUM_LOAD_FACTOR) {
                std::list<K>* newBuckets = new std::list<K>[m_BucketCount * 2];
                for (std::size_t i = 0; i < m_BucketCount; i++) {
                    for (const K& key : m_Buckets[i]) {
                        std::size_t index = m_Hash(key) % (m_BucketCount * 2);
                        newBuckets[index].push_back(key);
                    }
                }
                delete[] m_Buckets;
                m_BucketCount *= 2;
                m_Buckets = newBuckets;
            }
        }

    public:

        /**
         * Constructor
         */
        UnorderedSet() : m_BucketCount(MINIMUM_BUCKET_COUNT) {
            m_Buckets = new std::list<K>[m_BucketCount];
        }

        /**
         * Copy constructor
         * @param other
         */
        UnorderedSet(const UnorderedSet& other) {
            m_Size = other.m_Size;
            m_BucketCount = other.m_BucketCount;
            m_Buckets = new std::list<K>[m_BucketCount];
            for (std::size_t i = 0; i < m_BucketCount; ++i) {
                m_Buckets[i] = other.m_Buckets[i];
            }
        }

        /**
         * Copy assignment
         * @param other
         * @return
         */
        UnorderedSet& operator=(const UnorderedSet& other) noexcept {
            if (this != &other) {
                delete[] m_Buckets;
                m_Size = other.m_Size;
                m_BucketCount = other.m_BucketCount;
                m_Buckets = new std::list<K>[m_BucketCount];
                for (std::size_t i = 0; i < m_BucketCount; ++i) {
                    m_Buckets[i] = other.m_Buckets[i];
                }
            }
            return *this;
        }

        /**
         * Get the number of elements
         * @return
         */
        [[nodiscard]] std::size_t getSize() const noexcept {
            return m_Size;
        }

        /**
         * Check if container is empty
         * @return
         */
        [[nodiscard]] bool empty() const noexcept {
            return m_Size == 0;
        }

        /**
         * Get the number of buckets
         * @return
         */
        [[nodiscard]] std::size_t getBucketCount() const noexcept {
            return m_BucketCount;
        }

        /**
         * Get the average load factor.
         * @return
         */
        [[nodiscard]] double getLoadFactor() const noexcept {
            return static_cast<double>(m_Size) / static_cast<double>(m_BucketCount);
        }

        /**
         * Returns a const reference to the element with the specified key, wrapped in a std::optional.
         * If the element was not found, then std::nullopt is returned.
         * @param key
         * @return
         */
        [[nodiscard]] std::optional<std::reference_wrapper<const K>> find(const K& key) const noexcept {
            std::size_t index = m_Hash(key) % m_BucketCount;
            for (const K& entry : m_Buckets[index]) {
                if (entry == key) {
                    return entry;
                }
            }
            return std::nullopt;
        }

        /**
         * Check if a key exists in the container.
         * @param key
         * @return True if key exists, false if not.
         */
        [[nodiscard]] bool contains(const K& key) const noexcept {
            if (find(key) == std::nullopt) {
                return false;
            }
            return true;
        }

        /**
         * Insert element. If the element already exists inside the hashmap, then nothing happens.
         * If no insertion occurred, then the bool in the returned value is false. Else it is true if
         * insertion occurred.
         * @param key
         * @return
         */
        std::pair<std::reference_wrapper<const K>, bool> insert(const K& key) noexcept {
            expand();
            std::size_t index = m_Hash(key) % m_BucketCount;
            for (K& entry : m_Buckets[index]) {
                if (entry == key) {
                    return {entry, false};
                }
            }
            m_Buckets[index].push_back(key);
            ++m_Size;
            auto lastIterator = std::next(m_Buckets[index].end(), -1);
            return {*lastIterator, true};
        }

        /**
         * Insert element. If the element already exists inside the hashmap, then nothing happens.
         * If no insertion occurred, then the bool in the returned value is false. Else it is true if
         * insertion occurred.
         * @param key
         * @return
         */
        std::pair<std::reference_wrapper<const K>, bool> insert(K&& key) noexcept {
            expand();
            std::size_t index = m_Hash(key) % m_BucketCount;
            for (K& entry : m_Buckets[index]) {
                if (entry == key) {
                    return {entry, false};
                }
            }
            m_Buckets[index].push_back(std::move(key));
            ++m_Size;
            auto lastIterator = std::next(m_Buckets[index].end(), -1);
            return {*lastIterator, true};
        }

        /**
         * Construct element in-place. Similar to insert(...).
         * @see insert(...)
         * @tparam Ts
         * @param args
         * @return
         */
        template<typename ... Ts>
        std::pair<std::reference_wrapper<const K>, bool> emplace(Ts&& ... args) noexcept {
            expand();
            K key(std::forward<Ts>(args)...);
            std::size_t index = m_Hash(key) % m_BucketCount;
            for (K& entry : m_Buckets[index]) {
                if (entry == key) {
                    return {entry, false};
                }
            }
            m_Buckets[index].push_back(std::move(key));
            ++m_Size;
            auto lastIterator = std::next(m_Buckets[index].end(), -1);
            return {*lastIterator, true};
        }

        /**
         * Remove a key from the hashmap.
         * @param key
         * @return True if key was found and removed, false if key not found.
         */
        bool erase(const K& key) noexcept {
            std::size_t index = m_Hash(key) % m_BucketCount;
            auto iterator = m_Buckets[index].begin();
            for (; iterator != m_Buckets[index].end(); ++iterator) {
                if (*iterator == key) {
                    break;
                }
            }
            if (iterator != m_Buckets[index].end()) {
                m_Buckets[index].erase(iterator);
                --m_Size;
                return true;
            }
            return false;
        }

        /**
         * Shrink the container.
         * @return True if rehashing was done, false if not
         */
        bool shrinkToFit() noexcept {
            std::size_t s = MINIMUM_BUCKET_COUNT;
            while ((static_cast<double>(m_Size) / static_cast<double>(s)) > MAXIMUM_LOAD_FACTOR) {
                s *= 2;
            }
            if (s < m_BucketCount) {
                std::list<K>* newBuckets = new std::list<K>[s];
                for (std::size_t i = 0; i < m_BucketCount; i++) {
                    for (const K& key : m_Buckets[i]) {
                        std::size_t index = m_Hash(key) % s;
                        newBuckets[index].push_back(key);
                    }
                }
                delete[] m_Buckets;
                m_BucketCount = s;
                m_Buckets = newBuckets;
                return true;
            }
            return false;
        }

        /**
         * Remove all elements in the container. Sets size back to 0.
         */
        void clear() noexcept {
            for (std::size_t i = 0; i < m_BucketCount; ++i) {
                m_Buckets[i].clear();
            }
            m_Size = 0;
            if (m_BucketCount > MINIMUM_BUCKET_COUNT) {
                m_BucketCount = MINIMUM_BUCKET_COUNT;
                delete[] m_Buckets;
                m_Buckets = new std::list<K>[m_BucketCount];
            }
        }

        /**
         * Destructor
         */
        ~UnorderedSet() {
            delete[] m_Buckets;
        }

    };

}

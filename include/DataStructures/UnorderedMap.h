//
// Created by Nicholas on 17/12/2020.
//

#include <cstddef>
#include <functional>
#include <list>
#include <utility>

#pragma once

namespace wtl {

    namespace impl {

        /**
         * Element in list.
         * @tparam K
         * @tparam V
         */
        template<typename K, typename V>
        class UnorderedMapEntry {
        public:

            K m_Key;

            V m_Value;

            /**
             * Constructor
             * @param key
             * @param value
             */
            UnorderedMapEntry(const K& key, const V& value) : m_Key(key), m_Value(value) {}

            UnorderedMapEntry(const K& key, V&& value) : m_Key(key), m_Value(std::move(value)) {}

            template<typename ... Ts>
            explicit UnorderedMapEntry(const K& key, Ts&& ... args) : m_Key(key), m_Value(std::forward<Ts>(args)...) {}

            /**
             * Copy constructor
             * @param other
             */
            UnorderedMapEntry(const UnorderedMapEntry<K, V>& other) : m_Key(other.m_Key), m_Value(other.m_Value) {}

            /**
             * Move assignment
             * @param other
             * @return
             */
            UnorderedMapEntry& operator=(const UnorderedMapEntry& other) noexcept {
                if (this != &other) {
                    m_Key = other.m_Key;
                    m_Value = other.m_Value;
                }
                return *this;
            }

            /**
             * Move constructor
             * @param other
             */
            UnorderedMapEntry(UnorderedMapEntry<K, V>&& other) noexcept
                    : m_Key(std::move(other.m_Key)), m_Value(std::move(other.m_Value)) {}

            ~UnorderedMapEntry() = default;
        };

    }

    /**
     * An UnorderedMap is a hashmap using closed addressing. It is also known as open hashing. Elements are inserted
     * in a unsorted fashion into buckets. The hash of the key determines which bucket the value is inserted in.
     * Supports average O(1) time insertion, searching and removing.
     * Number of buckets automatically expands depending on the maximum load factor.
     * @tparam K
     * @tparam V
     * @tparam Hash
     */
    template<typename K, typename V, typename Hash = std::hash<K>>
    class UnorderedMap {
    private:

        typedef impl::UnorderedMapEntry<K, V> Entry;
        typedef std::pair<std::reference_wrapper<const K>, std::reference_wrapper<V>> KeyValuePairResult;

        static constexpr std::size_t MINIMUM_BUCKET_COUNT = 12;
        static constexpr double MAXIMUM_LOAD_FACTOR = 1;

        /// Number of elements
        std::size_t m_Size = 0;

        /// Number of buckets
        std::size_t m_BucketCount = 0;

        /// Buckets represented as a array of lists
        std::list<Entry>* m_Buckets = nullptr;

        /// Hashing algorithm
        Hash m_Hash;

        /**
         * Double the number of buckets if the load factor exceeds the threshold. Rehashes the key-value pairs.
         */
        void expand() noexcept {
            if (getLoadFactor() >= MAXIMUM_LOAD_FACTOR) {
                auto* newBuckets = new std::list<Entry>[m_BucketCount * 2];
                for (std::size_t i = 0; i < m_BucketCount; i++) {
                    for (const Entry& entry : m_Buckets[i]) {
                        std::size_t index = m_Hash(entry.m_Key) % (m_BucketCount * 2);
                        newBuckets[index].push_back(entry);
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
        UnorderedMap() : m_BucketCount(MINIMUM_BUCKET_COUNT) {
            m_Buckets = new std::list<Entry>[m_BucketCount];
        }

        /**
         * Copy constructor
         */
        UnorderedMap(const UnorderedMap& other) {
            m_Size = other.m_Size;
            m_BucketCount = other.m_BucketCount;
            m_Buckets = new std::list<Entry>[m_BucketCount];
            for (std::size_t i = 0; i < m_BucketCount; ++i) {
                m_Buckets[i] = other.m_Buckets[i];
            }
        }

        /**
         * Copy assignment
         */
        UnorderedMap& operator=(const UnorderedMap& other) noexcept {
            if (this != &other) {
                delete[] m_Buckets;
                m_Size = other.m_Size;
                m_BucketCount = other.m_BucketCount;
                m_Buckets = new std::list<Entry>[m_BucketCount];
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
         * Returns a reference to the value with the specified key, wrapped in a std::optional.
         * If the key was not found, then std::nullopt is returned.
         * @param key
         * @return
         */
        [[nodiscard]] std::optional<std::reference_wrapper<V>> at(const K& key) noexcept {
            std::size_t index = m_Hash(key) % m_BucketCount;
            for (Entry& entry : m_Buckets[index]) {
                if (entry.m_Key == key) {
                    return entry.m_Value;
                }
            }
            return std::nullopt;
        }

        /**
         * Returns a const reference to the value with the specified key, wrapped in a std::optional.
         * If the key was not found, then std::nullopt is returned.
         * @param key
         * @return
         */
        [[nodiscard]] std::optional<std::reference_wrapper<const V>> at(const K& key) const noexcept {
            std::size_t index = m_Hash(key) % m_BucketCount;
            for (const Entry& entry : m_Buckets[index]) {
                if (entry.m_Key == key) {
                    return entry.m_Value;
                }
            }
            return std::nullopt;
        }

        /**
         * Check if a key exists in the container.
         * @param key
         * @return True if key exists, false if not.
         */
        bool contains(const K& key) const noexcept {
            if (at(key) == std::nullopt) {
                return false;
            }
            return true;
        }

        /**
         * Insert key-value pair. If key already exists, then nothing happens. The return value is a pair which consists
         * of KeyValuePairResult and bool. The bool is true if insertion occurred, false if not.
         * If insertion occurred, then the KeyValuePairResult is a pair with const reference to the key and reference
         * to the inserted value.
         * If no insertion occurred, then the KeyValuePairResult is a pair with const reference to the key and reference
         * to the already existing element preventing the insertion.
         * @param key
         * @param value
         * @return stl pair consisting of KeyValuePairResult and bool.
         */
        std::pair<KeyValuePairResult, bool> insert(const K& key, const V& value) noexcept {
            expand();
            std::size_t index = m_Hash(key) % m_BucketCount;
            for (auto iterator = m_Buckets[index].begin(); iterator != m_Buckets[index].end(); ++iterator) {
                if (iterator->m_Key == key) {
                    return {{iterator->m_Key, iterator->m_Value}, false};
                }
            }
            m_Buckets[index].push_back(Entry(key, value));
            ++m_Size;
            auto lastIterator = std::next(m_Buckets[index].end(), -1);
            return {{lastIterator->m_Key, lastIterator->m_Value}, true};
        }

        /**
         * Insert key-value pair. If key already exists, then nothing happens. The return value is a pair which consists
         * of KeyValuePairResult and bool. The bool is true if insertion occurred, false if not.
         * If insertion occurred, then the KeyValuePairResult is a pair with const reference to the key and reference
         * to the inserted value.
         * If no insertion occurred, then the KeyValuePairResult is a pair with const reference to the key and reference
         * to the already existing element preventing the insertion.
         * @param key
         * @param value
         * @return stl pair consisting of KeyValuePairResult and bool.
         */
        std::pair<KeyValuePairResult, bool> insert(const K& key, V&& value) noexcept {
            expand();
            std::size_t index = m_Hash(key) % m_BucketCount;
            for (auto iterator = m_Buckets[index].begin(); iterator != m_Buckets[index].end(); ++iterator) {
                if (iterator->m_Key == key) {
                    return {{iterator->m_Key, iterator->m_Value}, false};
                }
            }
            m_Buckets[index].push_back(Entry(key, std::move(value)));
            ++m_Size;
            auto lastIterator = std::next(m_Buckets[index].end(), -1);
            return {{lastIterator->m_Key, lastIterator->m_Value}, true};
        }

        /**
         * Insert key-value pair. If key already exists, the existing value is replaced by the new value, else it is
         * simply inserted as per normal.
         * The return value is a pair of KeyValuePairResult and bool. The bool is true if insertion occurred, false if
         * replacement occurred. The KeyValuePairResult is a pair with const reference to the key and reference
         * to the value.
         * @param key
         * @param value
         * @return
         */
        std::pair<KeyValuePairResult, bool> insertOrReplace(const K& key, const V& value) noexcept {
            expand();
            std::size_t index = m_Hash(key) % m_BucketCount;
            auto iterator = m_Buckets[index].begin();
            for (; iterator != m_Buckets[index].end(); ++iterator) {
                if (iterator->m_Key == key) {
                    break;
                }
            }
            if (iterator == m_Buckets[index].end()) {
                m_Buckets[index].push_back(Entry(key, value));
                ++m_Size;
                iterator = m_Buckets[index].end() - 1;
                return {{iterator->m_Key, iterator->m_Value}, true};
            } else {
                iterator->m_Value = value;
                return {{iterator->m_Key, iterator->m_Value}, false};
            }
        }

        /**
         * Insert key-value pair. If key already exists, the existing value is replaced by the new value, else it is
         * simply inserted as per normal.
         * The return value is a pair of KeyValuePairResult and bool. The bool is true if insertion occurred, false if
         * replacement occurred. The KeyValuePairResult is a pair with const reference to the key and reference
         * to the value.
         * @param key
         * @param value
         * @return
         */
        std::pair<KeyValuePairResult, bool> insertOrReplace(const K& key, V&& value) noexcept {
            expand();
            std::size_t index = m_Hash(key) % m_BucketCount;
            auto iterator = m_Buckets[index].begin();
            for (; iterator != m_Buckets[index].end(); ++iterator) {
                if (iterator->m_Key == key) {
                    break;
                }
            }
            if (iterator == m_Buckets[index].end()) {
                m_Buckets[index].push_back(Entry(key, std::move(value)));
                ++m_Size;
                iterator = m_Buckets[index].end() - 1;
                return {{iterator->m_Key, iterator->m_Value}, true};
            } else {
                iterator->m_Value = std::move(value);
                return {{iterator->m_Key, iterator->m_Value}, false};
            }
        }

        /**
         * Construct a value in-place if the key does not already exist. If it does, nothing occurs. This method
         * is similar to insert(...).
         * @see insert(...)
         * @tparam Ts
         * @param key
         * @param args
         * @return
         */
        template<typename ... Ts>
        std::pair<KeyValuePairResult, bool> emplace(const K& key, Ts&& ... args) noexcept {
            expand();
            std::size_t index = m_Hash(key) % m_BucketCount;
            for (auto iterator = m_Buckets[index].begin(); iterator != m_Buckets[index].end(); ++iterator) {
                if (iterator->m_Key == key) {
                    return {{iterator->m_Key, iterator->m_Value}, false};
                }
            }
            m_Buckets[index].push_back(Entry(key, std::forward<Ts>(args)...));
            ++m_Size;
            auto lastIterator = std::next(m_Buckets[index].end(), -1);
            return {{lastIterator->m_Key, lastIterator->m_Value}, true};
        }

        /**
         * Construct a value in-place if the key does not already exist. If it does, the value is replaced. This method
         * is similar to insertOrReplace(...).
         * @see insertOrReplace(...)
         * @tparam Ts
         * @param key
         * @param args
         * @return
         */
        template<typename ... Ts>
        std::pair<KeyValuePairResult, bool> emplaceOrReplace(const K& key, Ts&& ... args) noexcept {
            expand();
            std::size_t index = m_Hash(key) % m_BucketCount;
            auto iterator = m_Buckets[index].begin();
            for (; iterator != m_Buckets[index].end(); ++iterator) {
                if (iterator->m_Key == key) {
                    break;
                }
            }
            if (iterator == m_Buckets[index].end()) {
                m_Buckets[index].push_back(Entry(key, std::forward<Ts>(args)...));
                ++m_Size;
                iterator = m_Buckets[index].end() - 1;
                return {{iterator->m_Key, iterator->m_Value}, true};
            } else {
                iterator->m_Value = V(std::forward<Ts>(args)...);
                return {{iterator->m_Key, iterator->m_Value}, false};
            }
        }

        /**
         * Erase key-value pair with specified key.
         * @param key
         * @return True if key was found and key-value pair was removed, false if not.
         */
        bool erase(const K& key) noexcept {
            std::size_t index = m_Hash(key) % m_BucketCount;
            auto iterator = m_Buckets[index].begin();
            for (; iterator != m_Buckets[index].end(); ++iterator) {
                if (iterator->m_Key == key) {
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
                std::list<Entry>* newBuckets = new std::list<Entry>[s];
                for (std::size_t i = 0; i < m_BucketCount; i++) {
                    for (const Entry& entry : m_Buckets[i]) {
                        std::size_t index = m_Hash(entry.m_Key) % s;
                        newBuckets[index].push_back(entry);
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
                m_Buckets = new std::list<Entry>[m_BucketCount];
            }
        }

        /**
         * Destructor
         */
        ~UnorderedMap() {
            delete[] m_Buckets;
        }
    };

}

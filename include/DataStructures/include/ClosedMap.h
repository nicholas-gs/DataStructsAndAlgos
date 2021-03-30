//
// Created by Nicholas on 19 Dec 2020.
//

#pragma once

#include <cstddef>
#include <functional>
#include <utility>
#include <vector>
#include <stdexcept>
#include <cmath>

namespace wtl {

    /**
     * A hashmap that uses closed hashing/open addressing. Quadratic probing is used.
     * Unlike the UnorderedMap, present of duplicate keys is a undefined behaviour.
     * @tparam K
     * @tparam V
     * @tparam Hash
     */
    template<typename K, typename V, typename Hash = std::hash<K>>
    class ClosedMap {
    private:

        /**
         * Status of a slot in the hashmap.
         */
        enum Status {
            FREE, OCCUPIED, DELETED
        };

        typedef std::pair<K, V> KeyValuePair;
        typedef std::pair<std::reference_wrapper<const K>, std::reference_wrapper<V>> KeyValuePairResult;

        std::vector<std::size_t> PRIME_BUCKET_SIZES = {11, 23, 47, 63};
        static constexpr double MAXIMUM_LOAD_FACTOR = 0.5;

        /// Number of elements
        std::size_t m_Size = 0;

        /// Number of buckets
        std::size_t m_BucketCount = 0;

        /// Keep track of bucket count
        std::size_t m_BucketCountIndex = 0;

        /// Buckets represented as a array of values
        KeyValuePair* m_Buckets = nullptr;

        /// Array of Status
        Status* m_Status = nullptr;

        /// Hashing algorithm
        Hash m_Hash;

        /**
         * Functor to calculate hash index using quadratic probing. The offset has alternative signs (+1,−4,+9,−16,...).
         * Together with prime number p congruent to 3 modulo 4 number of buckets, this ensures that all buckets will
         * be checked for an empty slot.
         */
        struct QuadraticProbing {
        private:
            std::size_t m_HashedIndex;
            std::size_t m_BucketCount;
            std::size_t iteration = 0;

            inline std::size_t modulo(int value, std::size_t m) {
                int mod = value % static_cast<int>(m);
                if (mod < 0) {
                    mod += m;
                }
                return mod;
            }

        public:
            QuadraticProbing(std::size_t hashedKey, std::size_t bucketCount)
                    : m_HashedIndex(hashedKey % bucketCount), m_BucketCount(bucketCount) {}

            std::size_t operator()() {
                std::size_t r = modulo(static_cast<int>(m_HashedIndex) +
                                       static_cast<int>(std::pow(-1, iteration + 1) * std::pow(iteration, 2)),
                                       m_BucketCount);
                ++iteration;
                return r;
            }
        };

        /**
        * Double the number of buckets if the load factor exceeds the threshold. Rehashes the key-value pairs.
        */
        void expand() {
            if (getLoadFactor() > MAXIMUM_LOAD_FACTOR) {
                if (m_BucketCountIndex == PRIME_BUCKET_SIZES.size() - 1) {
                    throw std::runtime_error("Closed Map is at maximum capacity, cannot expand anymore!");
                }
                std::size_t oldBucketCount = m_BucketCount;
                m_Size = 0;
                m_BucketCount = PRIME_BUCKET_SIZES[++m_BucketCountIndex];
                auto* buckets = (KeyValuePair*) ::operator new(m_BucketCount * sizeof(KeyValuePair));
                auto* status = new Status[m_BucketCount];
                for (std::size_t i = 0; i < m_BucketCount; i++) {
                    status[i] = FREE;
                }
                std::swap(buckets, m_Buckets);
                std::swap(status, m_Status);
                for (std::size_t i = 0; i < oldBucketCount; i++) {
                    if (status[i] == OCCUPIED) {
                        insert(buckets[i].first, std::move(buckets[i].second));
                        buckets[i].~KeyValuePair();
                    }
                }
                ::operator delete(buckets, oldBucketCount * sizeof(KeyValuePair));
                delete[] status;
            }
        }

    public:

        /**
         * Constructor
         */
        ClosedMap() : m_BucketCount(PRIME_BUCKET_SIZES[0]) {
            m_Status = new Status[m_BucketCount];
            for (std::size_t i = 0; i < m_BucketCount; i++) {
                m_Status[i] = FREE;
            }
            m_Buckets = (KeyValuePair*) ::operator new(m_BucketCount * sizeof(KeyValuePair));
            m_Size = 0;
        }

        /**
         * Copy constructor
         * @param other
         */
        ClosedMap(const ClosedMap& other) {
            m_Size = other.m_Size;
            m_BucketCount = other.m_BucketCount;
            m_BucketCountIndex = other.m_BucketCountIndex;
            m_Buckets = (KeyValuePair*) ::operator new(m_BucketCount * sizeof(KeyValuePair));
            m_Status = new Status[m_BucketCount];
            for (std::size_t i = 0; i < m_BucketCount; i++) {
                m_Status[i] = other.m_Status[i];
                if (m_Status[i] == OCCUPIED) {
                    new(&m_Buckets[i]) KeyValuePair(other.m_Buckets[i]);
                }
            }
        }

        /**
         * Copy assignment
         * @param other
         * @return
         */
        ClosedMap& operator=(const ClosedMap& other) noexcept {
            if (this != &other) {
                for (std::size_t i = 0; i < m_BucketCount; i++) {
                    if (m_Status[i] == OCCUPIED) {
                        m_Buckets[i].~KeyValuePair();
                    }
                }
                ::operator delete(m_Buckets, m_BucketCount * sizeof(KeyValuePair));
                m_Size = other.m_Size;
                m_BucketCount = other.m_BucketCount;
                m_BucketCountIndex = other.m_BucketCountIndex;
                m_Buckets = (KeyValuePair*) ::operator new(m_BucketCount * sizeof(KeyValuePair));
                m_Status = new Status[m_BucketCount];
                for (std::size_t i = 0; i < m_BucketCount; i++) {
                    m_Status[i] = other.m_Status[i];
                    if (m_Status[i] == OCCUPIED) {
                        new(&m_Buckets[i]) KeyValuePair(other.m_Buckets[i]);
                    }
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
      * If the key was not found, then std::nullopts is returned.
      * @param key
      * @return
      */
        [[nodiscard]] std::optional<std::reference_wrapper<V>> at(const K& key) noexcept {
            QuadraticProbing quadraticProbing(m_Hash(key), m_BucketCount);
            std::size_t index = quadraticProbing();
            while (m_Status[index] != FREE) {
                if (m_Status[index] == OCCUPIED) {
                    if (m_Buckets[index].first == key) {
                        return m_Buckets[index].second;
                    }
                }
                index = quadraticProbing();
            }
            return std::nullopt;
        }

        /**
         * Returns a const reference to the value with the specified key, wrapped in a std::optional.
         * If the key was not found, then std::nullopts is returned.
         * @param key
         * @return
         */
        [[nodiscard]] std::optional<std::reference_wrapper<const V>> at(const K& key) const noexcept {
            QuadraticProbing quadraticProbing(m_Hash(key), m_BucketCount);
            std::size_t index = quadraticProbing();
            while (m_Status[index] != FREE) {
                if (m_Status[index] == OCCUPIED) {
                    if (m_Buckets[index].first == key) {
                        return m_Buckets[index].second;
                    }
                }
                index = quadraticProbing();
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
         * Insert key-value pair. Insertion is guaranteed to occur.
         * The KeyValuePairResult is a pair with const reference to the key and reference
         * to the inserted value.
         * @param key
         * @param value
         * @return stl pair consisting of KeyValuePairResult
         */
        KeyValuePairResult insert(const K& key, const V& value) noexcept {
            expand();
            QuadraticProbing quadraticProbing(m_Hash(key), m_BucketCount);
            std::size_t index = quadraticProbing();
            while (true) {
                if (m_Status[index] == FREE || m_Status == DELETED) {
                    m_Status[index] = OCCUPIED;
                    ++m_Size;
                    new(&m_Buckets[index]) KeyValuePair(key, value);
                    return {m_Buckets[index].first, m_Buckets[index].second};
                }
                index = quadraticProbing();
            }
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
        KeyValuePairResult insert(const K& key, V&& value) noexcept {
            expand();
            QuadraticProbing quadraticProbing(m_Hash(key), m_BucketCount);
            std::size_t index = quadraticProbing();
            while (true) {
                if (m_Status[index] == FREE || m_Status[index] == DELETED) {
                    m_Status[index] = OCCUPIED;
                    ++m_Size;
                    new(&m_Buckets[index]) KeyValuePair(key, std::move(value));
                    return {m_Buckets[index].first, m_Buckets[index].second};
                }
                index = quadraticProbing();
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
        std::pair<KeyValuePairResult, bool> insertOrReplace(const K& key, const V& value) {
            throw std::logic_error("Function not implemented");
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
        std::pair<KeyValuePairResult, bool> insertOrReplace(const K& key, V&& value) {
            throw std::logic_error("Function not implemented");
        }

        /**
         * Erase key-value pair with specified key.
         * @param key
         * @return True if key was found and key-value pair was removed, false if not.
         */
        bool erase(const K& key) noexcept {
            QuadraticProbing quadraticProbing(m_Hash(key), m_BucketCount);
            std::size_t index = quadraticProbing();
            while (m_Status[index] != FREE) {
                if (m_Status[index] == OCCUPIED) {
                    if (m_Buckets[index].first == key) {
                        m_Status[index] = DELETED;
                        m_Buckets[index].~KeyValuePair();
                        --m_Size;
                        return true;
                    }
                }
                index = quadraticProbing();
            }
            return false;
        }

        /**
         * Shrink the container.
         * @return True if rehashing was done, false if not
         */
        bool shrinkToFit() noexcept {
            std::size_t i;
            for (i = 0; i < PRIME_BUCKET_SIZES.size(); i++) {
                if (static_cast<double>(m_Size) / static_cast<double>(PRIME_BUCKET_SIZES[i]) < MAXIMUM_LOAD_FACTOR) {
                    break;
                }
            }
            if (PRIME_BUCKET_SIZES[i] < m_BucketCount) {
                std::size_t oldBucketCount = m_BucketCount;
                m_BucketCountIndex = i;
                m_BucketCount = PRIME_BUCKET_SIZES[i];
                auto* buckets = (KeyValuePair*) ::operator new(m_BucketCount * sizeof(KeyValuePair));
                auto* status = new Status[m_BucketCount];
                for (std::size_t j = 0; j < m_BucketCount; j++) {
                    status[j] = FREE;
                }
                std::swap(m_Buckets, buckets);
                std::swap(m_Status, status);
                for (std::size_t j = 0; j < oldBucketCount; j++) {
                    if (status[j] == OCCUPIED) {
                        insert(buckets[j].first, std::move(buckets[j].second));
                        buckets[j].~KeyValuePair();
                    }
                }
                delete[] status;
                operator delete(buckets, oldBucketCount * sizeof(KeyValuePair));
                return true;
            }
            return false;
        }

        /**
         * Remove all elements in the container. Sets size back to 0.
         */
        void clear() noexcept {
            for (std::size_t i = 0; i < m_BucketCount; i++) {
                if (m_Status[i] == OCCUPIED) {
                    m_Buckets[i].~KeyValuePair();
                }
                m_Status[i] = FREE;
            }
            m_Size = 0;
            if (m_BucketCount > PRIME_BUCKET_SIZES[0]) {
                delete[] m_Status;
                ::operator delete(m_Buckets, m_BucketCount * sizeof(KeyValuePair));
                m_BucketCount = PRIME_BUCKET_SIZES[0];
                m_BucketCountIndex = 0;
                m_Status = new Status[m_BucketCount];
                m_Buckets = (KeyValuePair*) ::operator new(m_BucketCount * sizeof(KeyValuePair));
            }
        }

        /**
         * Destructor
         */
        ~ClosedMap() {
            for (std::size_t i = 0; i < m_BucketCount; i++) {
                if (m_Status[i] == OCCUPIED) {
                    m_Buckets[i].~KeyValuePair();
                }
            }
            delete[] m_Status;
            ::operator delete(m_Buckets, m_BucketCount * sizeof(KeyValuePair));
        }

    };

}

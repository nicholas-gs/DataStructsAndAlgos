//
// Created by Nicholas on 6/12/2020.
//

#pragma once

namespace wtl_test {

    /**
     * Entity class used to tests
     */
    class Entity {
    public:

        int m_Id = -1;

        /**
         * Default constructor
         */
        Entity() = default;

        /**
         * Constructor
         * @param id
         */
        explicit Entity(int id) : m_Id(id) {}

        /**
         * Copy constructor
         * @param other
         */
        Entity(const Entity& other) {
            m_Id = other.m_Id;
        }

        /**
         * Copy assignment
         * @param other
         * @return
         */
        Entity& operator=(const Entity& other) noexcept {
            if (this != &other) {
                m_Id = other.m_Id;
            }
            return *this;
        }

        /**
         * Move constructor
         * @param other
         */
        Entity(Entity&& other) noexcept {
            m_Id = other.m_Id;
        }

        /**
         * Move assignment
         * @param other
         * @return
         */
        Entity& operator=(Entity&& other) noexcept {
            if (this != &other) {
                m_Id = other.m_Id;
            }
            return *this;
        }

        /**
         * Equality operator overload
         * @param other
         * @return
         */
        bool operator==(const Entity& other) const noexcept {
            return m_Id == other.m_Id;
        }

        /**
         * Inequality operator overload
         * @param other
         * @return
         */
        bool operator!=(const Entity& other) const noexcept {
            return !(*this != other);
        }

        bool operator<(const Entity& other) const {
            return m_Id < other.m_Id;
        }

    };
}

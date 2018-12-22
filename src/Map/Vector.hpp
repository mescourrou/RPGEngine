#pragma once

// STD Lib
#include <cmath>

// Project
#include "general_config.hpp"
#include <BaseObject.hpp>

#ifdef RPG_BUILD_TEST
#include <gtest/gtest.h>
#endif

namespace map
{

#ifdef RPG_BUILD_TEST
class VectorTest;
#endif

/**
 * @class Vector
 * @brief Create a vector with the size given in template parameter.
 */
template <const unsigned int m_kSize>
class Vector : public BaseObject
{
#ifdef RPG_BUILD_TEST
    friend class map::VectorTest;
#endif
public:
    /**
     * @brief Create a zero-full vector
     */
    Vector() : m_vector(m_kSize, 0) {}

    /**
     * @brief Create a vector from an initialize list.
     *
     * The length of the list must be smaller or equal to the size of the Vector
     * @param list Initializer lit
     */
    Vector(const std::initializer_list<double> list)
    {
        assert(list.size() <= m_kSize);
        for (auto& element : list)
        {
            m_vector.push_back(element);
        }

    }

    /**
     * @brief Create a vector from an other
     * @param [in] copy Vector to copy
     */
    Vector(const Vector<m_kSize>& copy)
    {
        m_vector = copy.m_vector;
    }

    /**
     * @brief Move the content of the other Vector
     * @param [in] move Vector to move into the new one
     */
    Vector(Vector<m_kSize>&& move)
    {
        m_vector = std::move(move.m_vector);
    }

    /// @brief Destructor
    ~Vector() override = default;

    /// @brief Get and modify the number at the index asked
    double& at(unsigned int index) { return m_vector.at(index); }
    /// @brief Get the number at the index asked
    double at(unsigned int index) const { return m_vector.at(index); }
    unsigned int size() const noexcept { return m_kSize; }

    /// @brief Get and modify the x coordinate (0)
    template<typename = std::enable_if<(m_kSize >= 1)>> double& x() noexcept { return m_vector.at(0); }
    /// @brief Get and modify the y coordinate (1)
    template<typename = std::enable_if<(m_kSize >= 2)>> double& y() noexcept { return m_vector.at(1); }
    /// @brief Get and modify the z coordinate (2)
    template<typename = std::enable_if<(m_kSize >= 3)>> double& z() noexcept { return m_vector.at(2); }
    /// @brief Get the x coordinate (0)
    template<typename = std::enable_if<(m_kSize >= 1)>> double x() const noexcept{ return m_vector.at(0); }
    /// @brief Get the y coordinate (1)
    template<typename = std::enable_if<(m_kSize >= 2)>> double y() const noexcept { return m_vector.at(1); }
    /// @brief Get the z coordinate (2)
    template<typename = std::enable_if<(m_kSize >= 3)>> double z() const noexcept { return m_vector.at(2); }

    /// @brief Calculate the euclidien norm of the vector
    double norm() const
    {
        double norm = 0;
        for (auto& el : m_vector)
        {
            norm += el*el;
        }
        return sqrt(norm);
    }

    /**
     * @brief Copy the vector to this one
     * @param [in] vector Vector to copy
     * @return Return himself
     */
    Vector<m_kSize>& operator=(const Vector<m_kSize>& vector)
    {
        m_vector = vector.m_vector;
        return *this;
    }
    /**
     * @brief Move the vector to this one
     * @param [in] vector Vector to move
     * @return Return himself
     */
    Vector<m_kSize>& operator=(Vector<m_kSize>&& vector)
    {
        m_vector = std::move(vector.m_vector);
        return *this;
    }

    Vector<m_kSize> operator+(const Vector<m_kSize>& vector) const
    {
        Vector<m_kSize> ret;
        for (unsigned int i = 0; i < m_kSize; i++)
            ret.at(i) = m_vector.at(i) + vector.at(i);
        return ret;
    }
    Vector<m_kSize> operator-(const Vector<m_kSize>& vector) const
    {
        Vector<m_kSize> ret;
        for (unsigned int i = 0; i < m_kSize; i++)
            ret.at(i) = m_vector.at(i) - vector.at(i);
        return ret;
    }
    Vector<m_kSize>& operator+=(const Vector<m_kSize>& vector)
    {
        for (unsigned int i = 0; i < m_kSize; i++)
            m_vector.at(i) += vector.at(i);
        return *this;
    }
    Vector<m_kSize>& operator-=(const Vector<m_kSize>& vector)
    {
        for (unsigned int i = 0; i < m_kSize; i++)
            m_vector.at(i) -= vector.at(i);
        return *this;
    }
    /**
     * @brief Compare the coordinate one to one
     * @param [in] other Other vector to compare to
     */
    bool operator==(const Vector<m_kSize>& other) const {
        for (unsigned int i = 0 ; i < m_kSize; i++)
        {
            if (other.m_vector.at(i) != m_vector.at(i))
                return false;
        }
        return true;
    }
    /**
     * @brief Compare the coordinate one to one
     * @param [in] other Other vector to compare to
     */
    bool operator!=(const Vector<m_kSize>& other) const {
        for (unsigned int i = 0 ; i < m_kSize; i++)
        {
            if (other.m_vector.at(i) != m_vector.at(i))
                return true;
        }
        return false;
    }


    std::string className() const noexcept override { return "Vector<"+std::to_string(m_kSize)+">"; }

protected:
    std::vector<double> m_vector; ///< Vector
};

} // namespace map


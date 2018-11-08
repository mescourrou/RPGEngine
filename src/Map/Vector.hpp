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

template <const unsigned int m_kSize>
class Vector : public BaseObject
{
#ifdef RPG_BUILD_TEST
    friend class map::VectorTest;
#endif
public:
    Vector() : m_vector(m_kSize, 0) {}
    Vector(const std::initializer_list<double> list)
    {
        assert(list.size() <= m_kSize);
        for (auto& element : list)
        {
            m_vector.push_back(element);
        }

    }
    Vector(const Vector<m_kSize>& copy)
    {
        m_vector = copy.m_vector;
    }
    Vector(Vector<m_kSize>&& move)
    {
        m_vector = std::move(move.m_vector);
    }
    ~Vector() override = default;

    double& at(unsigned int index) { return m_vector.at(index); }
    double at(unsigned int index) const { return m_vector.at(index); }
    unsigned int size() const noexcept { return m_kSize; }

    template<typename = std::enable_if<(m_kSize >= 1)>> double& x() noexcept { return m_vector.at(0); }
    template<typename = std::enable_if<(m_kSize >= 2)>> double& y() noexcept { return m_vector.at(1); }
    template<typename = std::enable_if<(m_kSize >= 3)>> double& z() noexcept { return m_vector.at(2); }
    template<typename = std::enable_if<(m_kSize >= 1)>> double x() const noexcept{ return m_vector.at(0); }
    template<typename = std::enable_if<(m_kSize >= 2)>> double y() const noexcept { return m_vector.at(1); }
    template<typename = std::enable_if<(m_kSize >= 3)>> double z() const noexcept { return m_vector.at(2); }

    double norm() const
    {
        double norm = 0;
        for (auto& el : m_vector)
        {
            norm += el*el;
        }
        return sqrt(norm);
    }

    Vector<m_kSize>& operator=(const Vector<m_kSize>& vector)
    {
        m_vector = vector.m_vector;
        return *this;
    }
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

    bool operator==(const Vector<m_kSize>& other) const {
        for (unsigned int i = 0 ; i < m_kSize; i++)
        {
            if (other.m_vector.at(i) != m_vector.at(i))
                return false;
        }
        return true;
    }
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
    std::vector<double> m_vector;
};

} // namespace map


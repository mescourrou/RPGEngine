#pragma once

// Project
#include "general_config.hpp"
#include <BaseObject.hpp>

#ifdef RPG_BUILD_TEST
#include <gtest/gtest.h>
#endif

#ifdef RPG_BUILD_TEST
class CounterTest;
#endif

/**
 * @brief Not used
 */
class Counter
{
#ifdef RPG_BUILD_TEST
    friend class CounterTest;
#endif
public:
    constexpr Counter(unsigned int start = 0) noexcept : m_count(start) {}
    ~Counter() noexcept = default;

    constexpr unsigned int number() noexcept {
        unsigned int current = m_count;
        m_count++;
        return current;
    }
private:
    unsigned int m_count;
};


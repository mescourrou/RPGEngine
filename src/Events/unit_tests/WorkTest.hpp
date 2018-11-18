#pragma once

#include <gtest/gtest.h>
#include <Work.hpp>

namespace events {

class WorkTest : public testing::Test
{
public:

    static inline bool callEffectuated1 = false;
    static inline bool callEffectuated2 = false;
    static inline bool callEffectuated3 = false;
    static inline bool callEffectuated4 = false;
};

}


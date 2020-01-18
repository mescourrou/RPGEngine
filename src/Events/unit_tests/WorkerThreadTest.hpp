#pragma once

#include <gtest/gtest.h>
#include <WorkerThread.hpp>

namespace events
{

class WorkerThreadTest : public testing::Test
{
  public:
    std::mutex mutex; ///< Mutex to prevent 2 tests to be run at the same time
    ///< (because WorkerThread work with a singleton)
};

}


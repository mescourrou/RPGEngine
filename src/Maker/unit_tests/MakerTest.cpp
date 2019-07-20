#include "MakerTest.hpp"
#include <glog/logging.h>

#include <Database.hpp>

namespace maker {

}

int main(int argc, char **argv)
{
    ::google::InitGoogleLogging(argv[0]);
    ::google::LogToStderr();
    ::google::SetVLOGLevel("*", VERBOSE);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


#include "QuestTest.hpp"
#include <glog/logging.h>
namespace quest
{

}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    google::SetVLOGLevel("*", 10);
    google::LogToStderr();
    return RUN_ALL_TESTS();
}


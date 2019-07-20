#include <iostream>
#include "general_config.hpp"
#include <Maker.hpp>

using namespace std;

int main(int argc, char **argv)
{
#ifndef BUILD_USE_FILESYSTEM
    throw std::string("Can't use Maker without filesystem");
#endif
    maker::Maker maker(argc, argv);
    maker.initialize();

    return maker.run();
}

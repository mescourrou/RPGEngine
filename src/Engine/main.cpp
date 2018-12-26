#include <iostream>
#include "general_config.hpp"
#include <GameLauncher.hpp>

using namespace std;

int main(int argc, char **argv)
{
    game::GameLauncher launcher(argc, argv);
    return launcher.start();
}

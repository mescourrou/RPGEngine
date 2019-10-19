#include <iostream>
#include "general_config.hpp"
#include <GameLauncher.hpp>

int main(int argc, char** argv)
{
    game::GameLauncher launcher(argc, argv);
    return launcher.start();
}

#include <iostream>
#include "general_config.hpp"
#include <Maker.hpp>

using namespace std;

int main(int argc, char **argv)
{
    maker::Maker maker;
    maker.loadDatabase("game1.db");

    return maker.createDatabaseModel();
}

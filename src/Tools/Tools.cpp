#include "Tools.hpp"
#include <algorithm>

std::string Tools::snakeCase(std::string str)
{
    std::replace_if(str.begin(), str.end(), [](char c){ return c == ' ';}, '_');
    return str;
}

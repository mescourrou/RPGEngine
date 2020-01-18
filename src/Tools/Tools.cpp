#include "Tools.hpp"
#include <algorithm>

/**
 * @brief Get the snake_case version of the string
 * @param str String to convert
 * @return Converted string
 */
std::string Tools::snakeCase(std::string str)
{
    std::replace_if(str.begin(), str.end(), [](char c)
    {
        return c == ' ';
    }, '_');
    return str;
}

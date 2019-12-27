#pragma once

namespace epstl
{

/**
 * @brief Get the max of the two elements
 *
 * The > operator need to be implemented
 *
 * @param a First element
 * @param b Second element
 * @return Return the max between a and b
 */
template <typename T>
T max(T a, T b)
{
    return a > b ? a : b;
}

/**
 * @brief Get the max of a list of elements
 *
 * The > operator need to be implemented
 *
 * @param a First element
 * @param b Second element
 * @param others Others elements
 * @return Return the max all elements
 */
template <typename T, typename ...Args>
T max(T a, T b, Args... others)
{
    return max(a, max(b, others...));
}

} // namespace epstl

#pragma once

namespace epstl
{

/**
 * @brief Standard pair structure
 */
template <typename T1, typename T2 = T1>
struct pair
{
    /**
     * @brief Default constructor
     */
    pair() = default;
    /**
     * @brief Construct the pair with the given values
     * @param first First value
     * @param second Second value
     */
    pair(T1 first, T2 second) : first(first), second(second) {}

    T1 first;   ///< First value
    T2 second;  ///< Second value

};

} // namespace epstl

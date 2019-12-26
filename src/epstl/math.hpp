#pragma once

namespace epstl
{

template <typename T>
T max(T a, T b)
{
    return a > b ? a : b;
}

template <typename T, typename ...Args>
T max(T a, T b, Args... others)
{
    return max(a, max(b, others...));
}

} // namespace epstl

#pragma once

namespace epstl
{

template <typename T1, typename T2 = T1>
struct pair
{
    pair() {}
    pair(T1 first, T2 second) : first(first), second(second) {}

    T1 first;
    T2 second;
};

} // namespace epstl

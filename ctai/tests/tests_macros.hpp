#pragma once

#include <type_traits>

namespace std
{
    template <typename T1, typename T2>
    constexpr auto is_same_v = std::is_same<T1, T2>::value;
}

#define ASSERT(x) static_assert(x, "");
#define ASSERT_EQ(a, b) static_assert(a == b, "");
#define ASSERT_NOT_EQ(a, b) static_assert(a != b, "");
#define ASSERT_EQ_T(a, b) static_assert(std::is_same_v<a, b>, "");
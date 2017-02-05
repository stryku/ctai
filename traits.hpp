#pragma once

#include <type_traits>
#include <cstddef>

template <size_t val, size_t ...values>
constexpr auto is_in = !((val != values) && ...);

template <typename type, typename ...types>
constexpr auto is_in_t = !((std::is_same<type, types>::value) && ...);
#pragma once

#include <type_traits>
#include <cstddef>

template <auto val, auto ...values>
constexpr auto is_value_in_v = ((val == values) || ...);

template <typename type, typename ...types>
constexpr auto is_type_in_v = ((std::is_same<type, types>::value) || ...);
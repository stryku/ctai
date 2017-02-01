#pragma once

#include <type_traits>
#include <cstddef>

template <size_t val, size_t ...values>
constexpr auto is_in = !((val != values) && ...);
#pragma once

#include <cstdint>

namespace cai
{
    template <uint32_t value>
    struct reg
    {
        static constexpr auto l = value & 0xff;
        static constexpr auto h = value & 0xff00;
        static constexpr auto x = value & 0xffff;
        static constexpr auto ex = value;
    };
}

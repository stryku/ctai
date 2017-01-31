#pragma once

namespace cai
{
    template <bool cf, bool zf>
    struct flags
    {
        static constexpr auto CF = cf;
        static constexpr auto ZF = zf;
    };
}

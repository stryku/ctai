#pragma once

namespace cai
{
    template <bool cf, bool zf>
    struct flags
    {
        static constexpr auto CF = cf;
        static constexpr auto ZF = zf;
    };

    using startup_flags_state = flags<false, false>;

    namespace details
    {
        template <typename>
        struct to_flags_impl;


        template <bool cf, bool zf>
        struct to_flags_impl<
                flags<cf, zf>>
        {
            using type = flags<cf, zf>;
        };
    }

    template <typename flags_t>
    using to_flags = typename details::to_flags_impl<flags_t>::type;

}

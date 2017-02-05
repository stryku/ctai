#pragma once

namespace cai
{
    template <bool cf, bool zf, bool sf, bool of>
    struct flags
    {
        static constexpr auto CF = cf;
        static constexpr auto ZF = zf;
        static constexpr auto SF = sf;
        static constexpr auto OF = of;
    };

    using startup_flags_state = flags<false, false, false, false>;


    namespace details
    {
        template <typename>
        struct to_flags_impl;


        template <bool cf, bool zf, bool sf, bool of>
        struct to_flags_impl<
                flags<cf, zf, sf, of>>
        {
            using type = flags<cf, zf, sf, of>;
        };
    }

    template <typename flags_t>
    using to_flags = typename details::to_flags_impl<flags_t>::type;

    template <typename flags_t, bool value>
    using set_cf = flags<value, to_flags<flags_t>{}.ZF, to_flags<flags_t>{}.SF, to_flags<flags_t>{}.OF>;

    template <typename flags_t, bool value>
    using set_zf = flags<to_flags<flags_t>{}.CF, value, to_flags<flags_t>{}.SF, to_flags<flags_t>{}.OF>;

    template <typename flags_t, bool value>
    using set_sf = flags<to_flags<flags_t>{}.CF, to_flags<flags_t>{}.ZF, value, to_flags<flags_t>{}.OF>;

    template <typename flags_t, bool value>
    using set_of = flags<to_flags<flags_t>{}.CF, to_flags<flags_t>{}.ZF, to_flags<flags_t>{}.SF, value>;
}

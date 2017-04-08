#pragma once

#include <cstddef>

namespace ctai
{
    namespace thread
    {
        template <size_t id_v, typename registers_t, typename flags_t>
        struct thread
        {
            static constexpr auto id = id_v;
            using registers = registers_t;
            using flags = flags_t;
        };
    }
}

#pragma once

#include <cstddef>

namespace ctai
{
    namespace thread
    {
        template <bool finished_v,
                  size_t id_v,
                  size_t priority_v,
                  typename registers_t,
                  typename flags_t>
        struct thread
        {
            static constexpr auto id = id_v;
            static constexpr auto priority = priority_v;
            static constexpr auto finished = finished_v;
            using registers = registers_t;
            using flags = flags_t;
        };
    }
}

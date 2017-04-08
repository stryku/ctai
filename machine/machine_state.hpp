#pragma once

#include <cstddef>

namespace ctai
{
    namespace machine
    {
        template <typename memory_t, typename threads_t, size_t time_v>
        struct state
        {
            using memory = memory_t;
            using threads = threads_t;
            static constexpr auto time = time_v;
            static constexpr auto next_time = time_v + 1;
        };
    }
}

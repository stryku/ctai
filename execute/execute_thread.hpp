#pragma once

#include "thread/thread.hpp"

#include <cstddef>

namespace ctai
{
    namespace execute
    {
        namespace details
        {
            template <typename thread_t,
                      typename memory_t,
                      typename opcodes_t,
                      size_t executed_instructions_count = 0>
            struct execute_thread_impl;

            template <size_t executed_instructions_count,
                      bool finished,
                      size_t id,
                      typename registers_t,
                      typename flags_t,
                      typename memory_t,
                      typename opcodes_t>
            struct execute_thread_impl<thread::thread<finished,
                                                      id,
                                                      executed_instructions_count, // reached thread max instruction execution count per turn, aka priority
                                                      registers_t,
                                                      flags_t>,
                                       memory_t,
                                       opcodes_t,
                                       executed_instructions_count>
            {
                using result_thread = thread::thread<finished,
                                                     id,
                                                     executed_instructions_count,
                                                     registers_t,
                                                     flags_t>;

                using result_memory = memory_t;
            };
        }
    }
}

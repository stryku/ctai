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


            //thread already finished
            template <size_t executed_instructions_count,
                      size_t id,
                      size_t priority,
                      typename registers_t,
                      typename flags_t,
                      typename memory_t,
                      typename opcodes_t>
            struct execute_thread_impl<thread::thread<true, // thread already finished
                                                      id,
                                                      priority,
                                                      registers_t,
                                                      flags_t>,
                                       memory_t,
                                       opcodes_t,
                                       executed_instructions_count>
            {
                using result_thread = thread::thread<true,
                                                     id,
                                                     priority,
                                                     registers_t,
                                                     flags_t>;

                using result_memory = memory_t;
            };


            //normal execution
            template <size_t executed_instructions_count,
                     size_t id,
                     size_t priority,
                     typename registers_t,
                     typename flags_t,
                     typename memory_t,
                     typename opcodes_t>
            struct execute_thread_impl<thread::thread<false,
                                                      id,
                                                      priority,
                                                      registers_t,
                                                      flags_t>,
                                       memory_t,
                                       opcodes_t,
                                       executed_instructions_count>
            {
                using current_thread_type = thread::thread<false,
                                                           id,
                                                           priority,
                                                           registers_t,
                                                           flags_t>;

                using thread_instruction_opcodes = thread::get_next_opcodes<current_thread_type, opcodes_t>;

                using next_thread =

                using execution_result = execute_thread_impl<current_thread_type,
                                                             memory_t,
                                                             opcodes_t>
            };
        }
    }
}

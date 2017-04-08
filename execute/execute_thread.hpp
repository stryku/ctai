#pragma once

#include "thread/thread.hpp"
#include "execute/instructions/execute_instruction.hpp"

#include <cstddef>

namespace ctai
{
    namespace execute2
    {
        namespace details
        {
            template <typename thread_t,
                      typename memory_t,
                      typename opcodes_t,
                      size_t executed_instructions_count = 0>
            struct execute_thread_impl;

            template <size_t executed_instructions_count_v,
                      bool finished,
                      size_t id,
                      typename registers_t,
                      typename flags_t,
                      typename memory_t,
                      typename opcodes_t>
            struct execute_thread_impl<thread::thread<finished,
                                                      id,
                                                      executed_instructions_count_v, // reached thread max instruction execution count per turn, aka priority
                                                      registers_t,
                                                      flags_t>,
                                       memory_t,
                                       opcodes_t,
                                       executed_instructions_count_v>
            {
                using result_thread = thread::thread<finished,
                                                     id,
                                                     executed_instructions_count_v,
                                                     registers_t,
                                                     flags_t>;

                using result_memory = memory_t;
                static constexpr auto executed_instructions_count = executed_instructions_count_v;

                using execution_result = execute_thread_impl;
            };


            //thread already finished
            template <size_t executed_instructions_count_v,
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
                                       executed_instructions_count_v>
            {
                using result_thread = thread::thread<true,
                                                     id,
                                                     priority,
                                                     registers_t,
                                                     flags_t>;

                using result_memory = memory_t;
                static constexpr auto executed_instructions_count = executed_instructions_count_v;

                using execution_result = execute_thread_impl;
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

                using ex_instruction_result = execute2::execute_instruction<current_thread_type,
                                                                            memory_t,
                                                                            thread_instruction_opcodes>;

                using execution_result = typename execute_thread_impl<typename ex_instruction_result::result_thread,
                                                                      typename ex_instruction_result::result_memory,
                                                                      opcodes_t,
                                                                      executed_instructions_count + 1>::execution_result;
            };
        }

        template <typename thread_t,
                  typename memory_t,
                  typename opcodes_t>
        using execute_thread = typename details::execute_thread_impl<thread_t,
                                                                     memory_t,
                                                                     opcodes_t>::execution_result;
    }
}

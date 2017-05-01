#pragma once

#include "instructions/ids_vaules.hpp"
#include "register.hpp"
#include "memory/memory.hpp"
#include "memory/memory_alloc.hpp"
#include "values/values.hpp"
#include "execute/eip_adjust.hpp"
#include "thread/thread.hpp"
#include "containers/queue.hpp"

#include <cstddef>
#include "machine/machine_state.hpp"

namespace ctai
{
    namespace execute2
    {
        namespace details
        {
            constexpr auto new_stack_size = 128;

            template <typename memory_t, typename threads_queue, size_t ret_val_v>
            struct ex_sys_create_thread_result
            {
                static constexpr auto ret_val = ret_val_v;
                using result_memory = memory_t;
                using result_threads_queue = threads_queue;
            };

            template <typename memory_t, typename threads_queue>
            using ex_sys_create_thread_impl_no_enough_memory = ex_sys_create_thread_result<memory_t, threads_queue, 0>;

            template <typename thread_t, typename memory_t, typename threads_queue, size_t allocated_stack_ptr,  size_t id>
            struct ex_sys_create_thread_after_alloc
            {
                using registers = typename thread_t::registers;

                static constexpr auto starting_point = get_reg<registers, regs::id_t::EBX>;
                static constexpr auto priority = get_reg<registers, regs::id_t::ECX>;
                static constexpr auto args = get_reg<registers, regs::id_t::EDX>;

                static constexpr auto new_thread_stack_start = allocated_stack_ptr + new_stack_size - 4; //-4 because there will be pointer to args

                using new_thread = thread::create<priority,
                                                  id,
                                                  starting_point, //eip
                                                  new_thread_stack_start>; //esp

                using new_threads_queue = queue::push<threads_queue, new_thread>;
                using memory_after_args_set = memory::set_32<memory_t, new_thread_stack_start, args>;

                using result = ex_sys_create_thread_result<memory_after_args_set, new_threads_queue, id>;
            };

            template <typename thread_t, typename memory_t, typename threads_queue, size_t id>
            struct ex_sys_create_thread_impl
            {
                using alloc_result = memory::alloc<memory_t, new_stack_size>;

                using result = typename std::conditional_t<alloc_result::result_ptr == 0,
                                                  ex_sys_create_thread_impl_no_enough_memory<memory_t, threads_queue>,
                                                  ex_sys_create_thread_after_alloc<thread_t,
                                                                                   typename alloc_result::result_memory,
                                                                                   threads_queue,
                                                                                   alloc_result::result_ptr,
                                                                                   id>>::result;
            };
        }

        //create_thread
        template <typename thread_t, typename machine_state_t, size_t ...rest_of_opcodes>
        struct ex_instruction<thread_t,
                              machine_state_t,
                              inst::to_size<inst::id_t::SYS_CREATE_THREAD>,
                              rest_of_opcodes...>
        {
            using create_result = typename details::ex_sys_create_thread_impl<thread_t,
                                                                              typename machine_state_t::memory,
                                                                              typename machine_state_t::threads,
                                                                              machine_state_t::last_thread_id + 1>::result;

            using registers_after_ret_val_set = set_reg<typename thread_t::registers,
                                                        regs::id_t::EAX,
                                                        create_result::ret_val>;

            using final_registers = adjust_eip<registers_after_ret_val_set, inst::id_t::SYS_CREATE_THREAD>;

            using result_thread = thread::set_registers<thread_t, final_registers>;
            using result_machine_state = machine::state<typename create_result::result_memory,
                                                        typename machine_state_t::opcodes,
                                                        typename create_result::result_threads_queue,
                                                        typename machine_state_t::output,
                                                        typename machine_state_t::input,
                                                        machine_state_t::time,
                                                        machine_state_t::last_thread_id + 1>;
        };
    }
}

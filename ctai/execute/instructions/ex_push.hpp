#pragma once

#include "ctai/instructions/ids_vaules.hpp"
#include "ctai/register.hpp"
#include "ctai/memory/memory.hpp"
#include "values/values.hpp"
#include "ctai/execute/eip_adjust.hpp"
#include "ctai/thread/thread.hpp"

#include <cstddef>
#include "ctai/machine/machine_state.hpp"

namespace ctai
{
    namespace execute2
    {
        namespace details
        {
            template <typename thread_t,
                      typename memory_t,
                      auto value>
            struct ex_push_impl
            {
                static constexpr auto current_esp = get_reg<typename thread_t::registers, regs::id_t::ESP>;
                static constexpr auto next_esp = current_esp - sizeof(value);

                using next_registers = set_reg<typename thread_t::registers, regs::id_t::ESP, next_esp>;
                using final_registers = adjust_eip<next_registers, inst::id_t::PUSH_REG>;

                using next_thread = thread::thread<thread_t::finished,
                                                   thread_t::id,
                                                   thread_t::priority,
                                                   final_registers,
                                                   typename thread_t::flags>;

                using splitted_value = values::split_to_byte_values_container<value>;
                using next_memory_block = values_container_n::set_from_container<memory_t, next_esp, splitted_value>;
            };
        }


        template <typename thread_t, typename mechine_state_t, size_t reg, size_t ...rest_of_opcodes>
        struct ex_instruction<thread_t,
                mechine_state_t,
                              inst::to_size<inst::id_t::PUSH_REG>,
                              reg,
                              rest_of_opcodes...>
        {
            static constexpr auto reg_val = get_reg<typename thread_t::registers, regs::to_id<reg>>;

            using push_result = details::ex_push_impl<thread_t,
                                                      typename mechine_state_t::memory::memory_block_t,
                                                      reg_val>;

            using next_memory = memory::set_memory_block<typename mechine_state_t::memory,
                                                         typename push_result::next_memory_block>;
            using result_machine_state = machine::set_memory<mechine_state_t, next_memory>;

            using result_thread = typename push_result::next_thread;
        };
    }
}

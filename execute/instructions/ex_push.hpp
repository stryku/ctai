#pragma once

#include "instructions/ids_vaules.hpp"
#include "register.hpp"
#include "memory/memory.hpp"
#include "values/values.hpp"
#include "execute/eip_adjuster.hpp"

#include <cstddef>
#include <thread/thread.hpp>

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
                using next_memory = values_container_n::set_from_container<memory_t, next_esp, splitted_value>;
            };
        }


        template <typename thread_t, typename memory_t, size_t reg, size_t ...rest_of_opcodes>
        struct ex_instruction<thread_t,
                              memory_t,
                              inst::to_size<inst::id_t::PUSH_REG>,
                              reg,
                              rest_of_opcodes...>
        {
            static constexpr auto reg_val = get_reg<typename thread_t::registers, regs::to_id<reg>>;

            using push_result = details::ex_push_impl<thread_t,
                                                      memory_t,
                                                      reg_val>;

            using result_thread = typename push_result::next_thread;
            using result_memory = typename push_result::next_thread;
        };
    }
}

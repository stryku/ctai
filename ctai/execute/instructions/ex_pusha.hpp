#pragma once

#include "ctai/instructions/ids_vaules.hpp"
#include "ctai/register.hpp"
#include "ctai/memory/memory.hpp"
#include "values/values.hpp"
#include "ctai/execute/eip_adjust.hpp"
#include "ctai/thread/thread.hpp"

#include <cstddef>

namespace ctai
{
    namespace execute2
    {
        namespace details
        {
            template <typename memory_t,
                      size_t esp,
                      auto value>
            struct push_reg
            {
                static constexpr auto next_esp = esp - sizeof(value);
                using next_memory = memory::set_32<memory_t, next_esp, value>;
            };

            template <typename thread_t,
                      typename memory_t>
            struct ex_pusha_impl
            {
                static constexpr auto current_esp = get_reg<typename thread_t::registers, regs::id_t::ESP>;

                using registers = typename thread_t::registers;
                using push_eax_result = push_reg<memory_t, current_esp, get_reg<registers, regs::id_t::EAX>>;
                using push_ecx_result = push_reg<typename push_eax_result::next_memory,
                                                 push_eax_result::next_esp,
                                                 get_reg<registers, regs::id_t::ECX>>;
                using push_edx_result = push_reg<typename push_ecx_result::next_memory,
                                                 push_ecx_result::next_esp,
                                                 get_reg<registers, regs::id_t::EDX>>;
                using push_ebx_result = push_reg<typename push_edx_result::next_memory,
                                                 push_edx_result::next_esp,
                                                 get_reg<registers, regs::id_t::EBX>>;
                using push_esp_result = push_reg<typename push_ebx_result::next_memory,
                                                 push_ebx_result::next_esp,
                                                 get_reg<registers, regs::id_t::ESP>>;
                using push_ebp_result = push_reg<typename push_esp_result::next_memory,
                                                 push_esp_result::next_esp,
                                                 get_reg<registers, regs::id_t::EBP>>;
                using push_esi_result = push_reg<typename push_ebp_result::next_memory,
                                                 push_ebp_result::next_esp,
                                                 get_reg<registers, regs::id_t::ESI>>;
                using push_edi_result = push_reg<typename push_esi_result::next_memory,
                                                 push_esi_result::next_esp,
                                                 get_reg<registers, regs::id_t::EDI>>;


                using next_registers = set_reg<registers, regs::id_t::ESP, push_edi_result::next_esp>;
                using final_registers = adjust_eip<next_registers, inst::id_t::PUSHA>;

                using next_thread = thread::thread<thread_t::finished,
                                                   thread_t::id,
                                                   thread_t::priority,
                                                   final_registers,
                                                   typename thread_t::flags>;


                using next_memory = typename push_edi_result::next_memory;
            };
        }


        template <typename thread_t, typename mechine_state_t, size_t ...rest_of_opcodes>
        struct ex_instruction<thread_t,
                mechine_state_t,
                              inst::to_size<inst::id_t::PUSHA>,
                              rest_of_opcodes...>
        {
            using pusha_result = details::ex_pusha_impl<thread_t,
                    typename mechine_state_t::memory>;

            using result_machine_state = machine::set_memory<mechine_state_t, typename pusha_result::next_memory>;

            using result_thread = typename pusha_result::next_thread;
        };
    }
}

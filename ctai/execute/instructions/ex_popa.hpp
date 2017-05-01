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
                      size_t esp>
            struct pop_reg
            {
                static constexpr auto next_esp = esp + 4;
                static constexpr auto result_value = memory::get_32<memory_t, esp>;
            };

            template <typename thread_t,
                      typename memory_t>
            struct ex_popa_impl
            {
                static constexpr auto current_esp = get_reg<typename thread_t::registers, regs::id_t::ESP>;

                using registers = typename thread_t::registers;

                using pop_edi_result = pop_reg<memory_t, current_esp>;
                using pop_esi_result = pop_reg<memory_t, pop_edi_result::next_esp>;
                using pop_ebp_result = pop_reg<memory_t, pop_esi_result::next_esp>;
                using pop_esp_result = pop_reg<memory_t, pop_ebp_result::next_esp>;
                using pop_ebx_result = pop_reg<memory_t, pop_esp_result::next_esp>;
                using pop_edx_result = pop_reg<memory_t, pop_ebx_result::next_esp>;
                using pop_ecx_result = pop_reg<memory_t, pop_edx_result::next_esp>;
                using pop_eax_result = pop_reg<memory_t, pop_ecx_result::next_esp>;

                using registers_after_eax = set_reg<registers, regs::id_t::EAX, pop_eax_result::result_value>;
                using registers_after_ebx = set_reg<registers_after_eax, regs::id_t::EBX, pop_ebx_result::result_value>;
                using registers_after_ecx = set_reg<registers_after_ebx, regs::id_t::ECX, pop_ecx_result::result_value>;
                using registers_after_edx = set_reg<registers_after_ecx, regs::id_t::EDX, pop_edx_result::result_value>;
                using registers_after_esp = set_reg<registers_after_edx, regs::id_t::ESP, pop_esp_result::result_value>;
                using registers_after_ebp = set_reg<registers_after_esp, regs::id_t::EBP, pop_ebp_result::result_value>;
                using registers_after_esi = set_reg<registers_after_ebp, regs::id_t::ESI, pop_esi_result::result_value>;
                using registers_after_edi = set_reg<registers_after_esi, regs::id_t::EDI, pop_edi_result::result_value>;

                using final_registers = adjust_eip<registers_after_edi, inst::id_t::PUSHA>;

                using next_thread = thread::thread<thread_t::finished,
                                                   thread_t::id,
                                                   thread_t::priority,
                                                   final_registers,
                                                   typename thread_t::flags>;


                using next_memory = memory_t;
            };
        }


        template <typename thread_t, typename machine_state_t, size_t ...rest_of_opcodes>
        struct ex_instruction<thread_t,
                machine_state_t,
                              inst::to_size<inst::id_t::POPA>,
                              rest_of_opcodes...>
        {
            using popa_result = details::ex_popa_impl<thread_t,
                                                      typename machine_state_t::memory>;

            using result_thread = typename popa_result::next_thread;
            using result_machine_state = machine::set_memory<machine_state_t, typename popa_result::next_memory>;
        };
    }
}

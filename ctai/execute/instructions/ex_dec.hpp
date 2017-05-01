#pragma once

#include "ctai/thread/thread.hpp"
#include "ctai/execute/eip_adjust.hpp"

namespace ctai
{
    namespace execute2
    {
        //dec register
        template<typename thread_t,
                 typename machine_state_t, size_t reg1, size_t ...rest_of_opcodes>
        struct ex_instruction<thread_t, machine_state_t, inst::to_size<inst::id_t::DEC>, reg1, rest_of_opcodes...>
        {
            using registers = typename thread_t::registers;

            static constexpr auto val1 = get_reg<registers, regs::to_id<reg1>>;

            using new_regs_state = set_reg<registers, regs::to_id<reg1>, static_cast<uint32_t>(val1 - 1)>;

            using final_regs_state = adjust_eip<new_regs_state, inst::id_t::DEC>;

            using result_thread = thread::set_registers<thread_t, final_regs_state>;
            using result_machine_state = machine_state_t;
        };
    }
}

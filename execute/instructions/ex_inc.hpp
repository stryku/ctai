#pragma once

#include "thread/thread.hpp"
#include "execute/eip_adjuster.hpp"

namespace ctai
{
    namespace execute2
    {
        //inc register
        template<typename thread_t,
                typename machine_state_t, size_t reg1, size_t ...rest_of_opcodes>
        struct ex_instruction<thread_t, machine_state_t, inst::to_size<inst::id_t::INC>, reg1, rest_of_opcodes...>
        {
            using registers = typename thread_t::registers;

            static constexpr auto val1 = get_reg<registers, regs::to_id<reg1>>;

            using new_regs_state = set_reg<registers, regs::to_id<reg1>, static_cast<uint32_t>(val1 + 1)>;

            using final_regs_state = adjust_eip<new_regs_state, inst::id_t::INC>;

            using result_thread = thread::set_registers<thread_t, final_regs_state>;
            using result_machine_state = machine_state_t;
        };
    }
}

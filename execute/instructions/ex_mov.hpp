#pragma once

#include "instructions/ids_vaules.hpp"
#include "register.hpp"
#include "memory/memory.hpp"
#include "values/values.hpp"
#include "execute/eip_adjuster.hpp"
#include "thread/thread.hpp"

#include <cstddef>

namespace ctai
{
    namespace execute2
    {
        //mov reg, val
        template <typename thread_t, typename machine_state_t, size_t reg, size_t value, size_t ...rest_of_opcodes>
        struct ex_instruction<thread_t,
                machine_state_t,
                inst::to_size<inst::id_t::MOV_REG_VAL>,
                reg,
                value,
                rest_of_opcodes...>
        {
            using registers_after_mov = set_reg<typename thread_t::registers,
                    regs::to_id<reg>,
                    value>;

            using final_registers = adjust_eip<registers_after_mov, inst::id_t::MOV_REG_VAL>;

            using result_thread = thread::set_registers<thread_t, final_registers>;
            using result_machine_state = machine_state_t;
        };
    }
}

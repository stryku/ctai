#pragma once

#include "ctai/instructions/ids_vaules.hpp"
#include "ctai/register.hpp"
#include "ctai/memory/memory.hpp"
#include "values/values.hpp"
#include "ctai/execute/eip_adjust.hpp"
#include "ctai/thread/thread.hpp"

#include <cstddef>
#include "ctai/machine/machine_state.hpp"
#include "ctai/utils/mem_reg_operations.hpp"

namespace ctai
{
    namespace execute2
    {
        template <typename thread_t, typename machine_state_t, size_t reg, size_t ...rest_of_opcodes>
        struct ex_instruction<thread_t,
                machine_state_t,
                inst::to_size<inst::id_t::POP_REG>,
                reg,
                rest_of_opcodes...>
        {
            using registers = typename thread_t::registers;

            static constexpr auto esp_val = get_reg<registers, regs::id_t::ESP>;

            using pop_result = utils::pop_reg_32<typename machine_state_t::memory,
                    registers,regs::to_id<reg>>;

            using final_registers = adjust_eip<pop_result, inst::id_t::POP_REG>;

            using result_thread = thread::set_registers<thread_t, final_registers>;

            using result_machine_state = machine_state_t;
        };
    }
}

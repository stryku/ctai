#pragma once

#include "instructions/ids_vaules.hpp"
#include "register.hpp"
#include "memory/memory.hpp"
#include "execute/eip_adjuster.hpp"
#include "thread/thread.hpp"
#include "machine/machine_state.hpp"
#include "io/output.hpp"

#include <cstddef>
#include <io/io.hpp>

namespace ctai
{
    namespace execute2
    {
        template <typename thread_t, typename machine_state_t, size_t ...rest_of_opcodes>
        struct ex_instruction<thread_t,
                machine_state_t,
                inst::to_size<inst::id_t::SYS_WRITE>,
                rest_of_opcodes...>
        {
            using final_registers = adjust_eip<typename thread_t::registers, inst::id_t::SYS_WRITE>;
            using result_thread = thread::set_registers<thread_t, final_registers>;

            static constexpr auto character = static_cast<char>(get_reg<final_registers, regs::id_t::AL>);

            using next_output = io::write<typename machine_state_t::output,
                    character>;

            using result_machine_state = machine::set_output<machine_state_t, next_output>;
        };
    }
}

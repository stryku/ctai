#pragma once

#include "instructions/ids_vaules.hpp"
#include "register.hpp"
#include "memory/memory.hpp"
#include "execute/eip_adjust.hpp"
#include "thread/thread.hpp"
#include "machine/machine_state.hpp"
#include "io/input.hpp"

#include <cstddef>
#include <io/io.hpp>

namespace ctai
{
    namespace execute2
    {
        template <typename thread_t, typename machine_state_t, size_t ...rest_of_opcodes>
        struct ex_instruction<thread_t,
                machine_state_t,
                inst::to_size<inst::id_t::SYS_READ>,
                rest_of_opcodes...>
        {
            using current_input = typename machine_state_t::input;
            static constexpr auto read_char = io::input::front<current_input>;

            using registers_after_al = set_reg<typename thread_t::registers, regs::id_t::AL, read_char>;

            using final_registers = adjust_eip<registers_after_al, inst::id_t::SYS_READ>;
            using result_thread = thread::set_registers<thread_t, final_registers>;

            using next_input = io::input::pop<current_input>;
            using result_machine_state = machine::set_input<machine_state_t, next_input>;
        };
    }
}

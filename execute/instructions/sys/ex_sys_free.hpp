#pragma once

#include "instructions/ids_vaules.hpp"
#include "register.hpp"
#include "memory/memory.hpp"
#include "execute/eip_adjust.hpp"
#include "thread/thread.hpp"
#include "machine/machine_state.hpp"
#include "memory/memory_free.hpp"

#include <cstddef>
#include <io/io.hpp>

namespace ctai
{
    namespace execute2
    {
        template <typename thread_t, typename machine_state_t, size_t ...rest_of_opcodes>
        struct ex_instruction<thread_t,
                              machine_state_t,
                              inst::to_size<inst::id_t::SYS_FREE>,
                              rest_of_opcodes...>
        {
            static constexpr auto ptr_to_free = get_reg<typename thread_t::registers, regs::id_t::EAX>;
            using free_result = memory::free<typename machine_state_t::memory, ptr_to_free>;
            using memory_result = memory::set_metadata<typename machine_state_t::memory, free_result>;

            using final_registers = adjust_eip<typename thread_t::registers, inst::id_t::SYS_FREE>;
            using result_thread = thread::set_registers<thread_t, final_registers>;

            using result_machine_state = machine::set_memory<machine_state_t, memory_result>;
        };
    }
}

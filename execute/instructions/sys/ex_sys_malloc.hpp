#pragma once

#include "instructions/ids_vaules.hpp"
#include "register.hpp"
#include "memory/memory.hpp"
#include "execute/eip_adjuster.hpp"
#include "thread/thread.hpp"
#include "machine/machine_state.hpp"
#include "memory/memory_alloc.hpp"

#include <cstddef>
#include <io/io.hpp>

namespace ctai
{
    namespace execute2
    {
        template <typename thread_t, typename machine_state_t, size_t ...rest_of_opcodes>
        struct ex_instruction<thread_t,
                              machine_state_t,
                              inst::to_size<inst::id_t::SYS_MALLOC>,
                              rest_of_opcodes...>
        {
            static constexpr auto size_to_alloc = get_reg<typename thread_t::registers, regs::id_t::EAX>;
            using alloc_result = memory::alloc<typename machine_state_t::memory, size_to_alloc>;

            using registers_after_eax = set_reg<typename thread_t::registers, regs::id_t::EAX, alloc_result::result_ptr>;

            using final_registers = adjust_eip<registers_after_eax, inst::id_t::SYS_MALLOC>;
            using result_thread = thread::set_registers<thread_t, final_registers>;

            using result_machine_state = machine::set_memory <machine_state_t, typename alloc_result::result_memory>;
        };
    }
}

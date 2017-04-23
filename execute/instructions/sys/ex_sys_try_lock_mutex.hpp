#pragma once

#include "instructions/ids_vaules.hpp"
#include "register.hpp"
#include "memory/memory.hpp"
#include "execute/eip_adjuster.hpp"
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
                              inst::to_size<inst::id_t::SYS_TRY_LOCK_MUTEX>,
                              rest_of_opcodes...>
        {
            static constexpr auto ebx_value = get_reg<typename thread_t::registers, regs::id_t::EBX>;
            static constexpr auto mutex_value = memory::get_8<typename machine_state_t::memory_t, ebx_value>;
            static constexpr auto set_to_eax = mutex_value ? 0 : 1; //if mutex already has value 1, it's already locked - return 0

            using result_memory = memory::set_8<typename machine_state_t::memory_t, ebx_value, 1>;

            using registers_after_eax = set_reg<typename thread_t::registers, regs::id_t::EAX, set_to_eax>;
            using final_registers = adjust_eip<registers_after_eax, inst::id_t::SYS_TRY_LOCK_MUTEX>;
            using result_thread = thread::set_registers<thread_t, final_registers>;

            using result_machine_state = machine::set_memory<machine_state_t, result_memory>;
        };
    }
}

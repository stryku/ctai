#pragma once

#include "ctai/instructions/ids_vaules.hpp"
#include "ctai/execute/eip_adjust.hpp"
#include "ctai/thread/thread.hpp"

#include <type_traits>

namespace ctai
{
    namespace execute2
    {
        template<typename thread_t, typename machine_state_t, size_t ip, size_t ...rest_of_opcodes>
        struct ex_instruction<thread_t, machine_state_t, inst::to_size<inst::id_t::JG>, ip, rest_of_opcodes...>
        {
            static constexpr auto flags_v = typename thread_t::flags{};

            static constexpr bool should_jmp = (flags_v.ZF == false && flags_v.CF == false);
            using final_regs_state = std::conditional_t<
                    should_jmp,
                    set_reg<typename thread_t::registers, regs::id_t::EIP, static_cast<uint32_t>(ip)>,
                    adjust_eip<typename thread_t::registers, inst::id_t::JG>>;

            using result_thread = thread::set_registers<thread_t, final_regs_state>;
            using result_machine_state = machine_state_t;
        };
    }
}
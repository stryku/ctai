#pragma once

#include <machine_state.hpp>
#include "instructions/instruction.hpp"
#include "execute/ex_instruction.hpp"
#include "instructions/ids_vaules.hpp"

namespace cai
{
    namespace execute
    {
        template <typename state_t, size_t reg>
        struct ex_instruction<state_t, inst::to_size<inst::id_t::PUSH_REG>, reg>
        {
            using state = to_machine_state<state_t>;
            using type = machine_state<
                    stack_push_8<get_reg<typename state::registers_state_t, regs::to_id<reg>>, typename state::stack_t>,
typename                    state::flags_t,
                    typename state::registers_state_t>;
        };


        namespace tests
        {
            //push al, and check if stack have correct value on the top
            //each register in startup_machine_state has 0xaabbccdd value
            static_assert(stack_top_8<to_machine_state<typename ex_instruction<startup_machine_state,
                                                                               inst::to_size<inst::id_t::PUSH_REG>,
                                                                               regs::to_size<regs::id_t::AL>>::type>::stack_t> == 0xdd, "");

            static_assert(stack_top_8<to_machine_state<typename ex_instruction<startup_machine_state,
                                                                               inst::to_size<inst::id_t::PUSH_REG>,
                                                                               regs::to_size<regs::id_t::AH>>::type>::stack_t> == 0xcc, "");
        }
    }
}

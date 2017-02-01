#pragma once

#include "machine_state.hpp"
#include "instructions/instruction.hpp"
#include "execute/ex_instruction.hpp"
#include "instructions/ids_vaules.hpp"

namespace cai
{
    namespace execute
    {
        template<typename state_t, size_t reg>
        struct ex_instruction<state_t, inst::to_size<inst::id_t::POP_REG>, reg>
        {
            using state = to_machine_state<state_t>;

            static constexpr auto val = stack_top<regs::reg_size_type<regs::to_id<reg>>, typename state::stack_t>; // get value from stack
            using new_stack = stack_pop<regs::reg_size_type<regs::to_id<reg>>, typename state::stack_t>; // pop from stack
            using new_regs_state = set_reg<typename state::registers_state_t, regs::to_id<reg>, static_cast<uint32_t>(val)>; //move value to register

            using type = machine_state<new_stack, typename state::flags_t, new_regs_state>;
        };

        namespace tests
        {
            static_assert(get_reg<
                    ex_instruction<
                            startup_machine_state,
                            inst::to_size<inst::id_t::POP_REG>,
                            regs::to_size<regs::id_t::AL>
                    >::type::registers_state_t,
                    regs::id_t::AL
            > == 0x00 ,"");

            static_assert(get_reg<
                    ex_instruction<
                            startup_machine_state,
                            inst::to_size<inst::id_t::POP_REG>,
                            regs::to_size<regs::id_t::AX>
                    >::type::registers_state_t,
                    regs::id_t::AX
            > == 0x0011 ,"");

            static_assert(get_reg<
                    ex_instruction<
                            startup_machine_state,
                            inst::to_size<inst::id_t::POP_REG>,
                            regs::to_size<regs::id_t::EAX>
                    >::type::registers_state_t,
                    regs::id_t::EAX
            > == 0x00112233 ,"");
        }
    }
}

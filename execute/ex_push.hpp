#pragma once

#include <machine_state.hpp>
#include "instructions/instruction.hpp"
#include "execute/ex_instruction.hpp"
#include "instructions/ids_vaules.hpp"

namespace cai
{
    namespace execute
    {
        template <typename state_t, size_t reg, size_t ...rest_of_opcodes>
        struct ex_instruction<state_t, inst::to_size<inst::id_t::PUSH_REG>, reg, rest_of_opcodes...>
        {
            using state = to_machine_state<state_t>;

            static constexpr auto reg_val = get_reg<typename state::registers_state_t, regs::to_id<reg>>;
            using new_stack = stack_push<regs::reg_size_type<regs::to_id<reg>>, // type to get val size
                        reg_val,
                        typename state::stack_t>;

            static constexpr auto eip_val = get_reg<typename state::registers_state_t, regs::id_t::EIP> + 2;
            using new_regs_state = set_reg<typename state::registers_state_t, regs::id_t::EIP, eip_val>;

            using type = machine_state<new_stack,
                    typename state::flags_t,
                    new_regs_state>;
        };


        namespace tests
        {
            //push a register, and check if stack have correct value on the top
            //each 32b register in startup_machine_state has 0xaabbccdd value
            static_assert(stack_top<regs::reg_size_type<regs::id_t::AL>,
                                    to_machine_state<typename ex_instruction<startup_machine_state,
                                                                             inst::to_size<inst::id_t::PUSH_REG>,
                                                                             regs::to_size<regs::id_t::AL>>::type>::stack_t> == 0xdd, "");

            static_assert(stack_top<regs::reg_size_type<regs::id_t::AH>,
                                    to_machine_state<typename ex_instruction<startup_machine_state,
                                                                             inst::to_size<inst::id_t::PUSH_REG>,
                                                                             regs::to_size<regs::id_t::AH>>::type>::stack_t> == 0xcc, "");



            static_assert(stack_top<regs::reg_size_type<regs::id_t::AH>,
                                    to_machine_state<typename ex_instruction<startup_machine_state,
                                                                             inst::to_size<inst::id_t::PUSH_REG>,
                                                                             regs::to_size<regs::id_t::AX>>::type>::stack_t> == 0xcc, "");

            static_assert(stack_top<regs::reg_size_type<regs::id_t::AX>,
                                    to_machine_state<typename ex_instruction<startup_machine_state,
                                                                             inst::to_size<inst::id_t::PUSH_REG>,
                                                                             regs::to_size<regs::id_t::AX>>::type>::stack_t> == 0xccdd, "");

            static_assert(stack_top<regs::reg_size_type<regs::id_t::AL>,
                                    to_machine_state<typename ex_instruction<startup_machine_state,
                                                                             inst::to_size<inst::id_t::PUSH_REG>,
                                                                             regs::to_size<regs::id_t::EAX>>::type>::stack_t> == 0xaa, "");

            static_assert(stack_top<regs::reg_size_type<regs::id_t::AX>,
                                    to_machine_state<typename ex_instruction<startup_machine_state,
                                                                             inst::to_size<inst::id_t::PUSH_REG>,
                                                                             regs::to_size<regs::id_t::EAX>>::type>::stack_t> == 0xaabb, "");

            static_assert(stack_top<regs::reg_size_type<regs::id_t::EAX>,
                                    to_machine_state<typename ex_instruction<startup_machine_state,
                                                                             inst::to_size<inst::id_t::PUSH_REG>,
                                                                             regs::to_size<regs::id_t::EAX>>::type>::stack_t> == 0xaabbccdd, "");
        }
    }
}

#pragma once

#include "machine_state.hpp"
#include "instructions/instruction.hpp"
#include "execute/ex_instruction.hpp"
#include "instructions/ids_vaules.hpp"

namespace cai
{
    namespace execute
    {
        template<typename state_t, size_t reg1, size_t reg2>
        struct ex_instruction<state_t, inst::to_size<inst::id_t::MOV_REG_REG>, reg1, reg2>
        {
            using state = to_machine_state<state_t>;

            static constexpr auto val2 = get_reg<typename state::registers_state_t, regs::to_id<reg2>>; // get reg2 value

            using new_regs_state = set_reg<typename state::registers_state_t, regs::to_id<reg1>, static_cast<uint32_t>(val2)>; //move register to register

            using type = machine_state<typename state::stack_t, typename state::flags_t, new_regs_state>;
        };

        namespace tests
        {
            static_assert(get_reg<ex_instruction<startup_machine_state,
                                  inst::to_size<inst::id_t::MOV_REG_REG>,
                                  regs::to_size<regs::id_t::AL>,
                                  regs::to_size<regs::id_t::AH>>::type::registers_state_t,
                                  regs::id_t::AL> == static_cast<uint8_t>(0xcc)
                    ,"");

            static_assert(get_reg<ex_instruction<startup_machine_state,
                          inst::to_size<inst::id_t::MOV_REG_REG>,
                          regs::to_size<regs::id_t::AL>,
                          regs::to_size<regs::id_t::AX>>::type::registers_state_t,
                          regs::id_t::AL> == static_cast<uint8_t>(0xdd)
                    ,"");

            static_assert(get_reg<ex_instruction<startup_machine_state,
                          inst::to_size<inst::id_t::MOV_REG_REG>,
                          regs::to_size<regs::id_t::AL>,
                          regs::to_size<regs::id_t::EAX>>::type::registers_state_t,
                          regs::id_t::AL> == static_cast<uint8_t>(0xdd)
                    ,"");

            static_assert(get_reg<ex_instruction<startup_machine_state,
                          inst::to_size<inst::id_t::MOV_REG_REG>,
                          regs::to_size<regs::id_t::AL>,
                          regs::to_size<regs::id_t::AH>>::type::registers_state_t,
                          regs::id_t::EAX> == static_cast<uint32_t>(0xaabbcccc)
                    ,"");

            static_assert(get_reg<ex_instruction<startup_machine_state,
                          inst::to_size<inst::id_t::MOV_REG_REG>,
                          regs::to_size<regs::id_t::AX>,
                          regs::to_size<regs::id_t::AL>>::type::registers_state_t,
                          regs::id_t::AX> == static_cast<uint32_t>(0xdd)
                    ,"");
        }
    }
}
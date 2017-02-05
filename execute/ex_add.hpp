#pragma once

#include "machine_state.hpp"
#include "instructions/instruction.hpp"
#include "execute/ex_instruction.hpp"
#include "instructions/ids_vaules.hpp"
#include "execute/eip_adjuster.hpp"

namespace cai
{
    namespace execute
    {
        template<typename state_t, size_t reg1, size_t reg2, size_t ...rest_of_opcodes>
        struct ex_instruction<state_t, inst::to_size<inst::id_t::ADD_REG_REG>, reg1, reg2, rest_of_opcodes...>
        {
            using state = to_machine_state<state_t>;

            static constexpr auto val1 = get_reg<typename state::registers_state_t, regs::to_id<reg1>>; // get reg1 value
            static constexpr auto val2 = get_reg<typename state::registers_state_t, regs::to_id<reg2>>; // get reg1 value

            using new_regs_state = set_reg<typename state::registers_state_t, regs::to_id<reg1>, static_cast<uint32_t>(val1 + val2)>; //move value to register

            using final_regs_state = adjust_eip<new_regs_state, inst::id_t::ADD_REG_REG>;


        using type = machine_state<typename state::stack_t, typename state::flags_t, final_regs_state>;
        };

        namespace tests
        {
            static_assert(get_reg<ex_instruction<startup_machine_state,
                                                 inst::to_size<inst::id_t::ADD_REG_REG>,
                                                 regs::to_size<regs::id_t::AL>,
                                                 regs::to_size<regs::id_t::AH>>::type::registers_state_t,
                                  regs::id_t::AL> == static_cast<uint8_t>(0xcc + 0xdd)
                    ,"");

            static_assert(get_reg<ex_instruction<startup_machine_state,
                          inst::to_size<inst::id_t::ADD_REG_REG>,
                          regs::to_size<regs::id_t::AH>,
                          regs::to_size<regs::id_t::AH>>::type::registers_state_t,
                          regs::id_t::AH> == static_cast<uint8_t>(0xcc + 0xcc)
                    ,"");

            static_assert(get_reg<ex_instruction<startup_machine_state,
                          inst::to_size<inst::id_t::ADD_REG_REG>,
                          regs::to_size<regs::id_t::AX>,
                          regs::to_size<regs::id_t::BX>>::type::registers_state_t,
                          regs::id_t::AX> == static_cast<uint16_t>(0xccdd + static_cast<uint16_t>(0xbada55))
                    ,"");

            static_assert(get_reg<ex_instruction<startup_machine_state,
                          inst::to_size<inst::id_t::ADD_REG_REG>,
                          regs::to_size<regs::id_t::EAX>,
                          regs::to_size<regs::id_t::EBX>>::type::registers_state_t,
                          regs::id_t::EAX> == static_cast<uint32_t>(0xaabbccdd + 0xbada55)
                    ,"");
        }
    }
}
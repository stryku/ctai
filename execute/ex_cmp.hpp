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
        struct ex_instruction<state_t, inst::to_size<inst::id_t::CMP_REG_REG>, reg1, reg2>
        {
            using state = to_machine_state<state_t>;

            static constexpr auto val1 = get_reg<typename state::registers_state_t, regs::to_id<reg1>>; // get reg1 value
            static constexpr auto val2 = get_reg<typename state::registers_state_t, regs::to_id<reg2>>; // get reg1 value

            static constexpr bool zf = (val1 == val2);
            static constexpr bool cf = (static_cast<uint64_t>(val1) < static_cast<uint64_t>(val2)); //set if borrow

            using new_flags_state = flags<cf, zf>;

            using type = machine_state<typename state::stack_t, new_flags_state, typename state::registers_state_t>;
        };

        namespace tests
        {
            using cmp_test_machine_state = machine_state<startup_stack, startup_flags_state, registers_state<0x0,0xaaaabbbb, 0xaabbccdd, 0,0,0,0,0, 0>>;

            static_assert(
                    ex_instruction<
                            cmp_test_machine_state,
                            inst::to_size<inst::id_t::CMP_REG_REG>,
                            regs::to_size<regs::id_t::AL>,
                            regs::to_size<regs::id_t::AH>>::type::flags.CF == false
                    ,"");

            static_assert(
                    ex_instruction<
                    cmp_test_machine_state,
                    inst::to_size<inst::id_t::CMP_REG_REG>,
                    regs::to_size<regs::id_t::AL>,
                    regs::to_size<regs::id_t::AH>>::type::flags.ZF == true
                    ,"");

            static_assert(
                    ex_instruction<
                    cmp_test_machine_state,
                    inst::to_size<inst::id_t::CMP_REG_REG>,
                    regs::to_size<regs::id_t::BX>,
                    regs::to_size<regs::id_t::AX>>::type::flags.CF == false
                    ,"");

            static_assert(
                    ex_instruction<
                    cmp_test_machine_state,
                    inst::to_size<inst::id_t::CMP_REG_REG>,
                    regs::to_size<regs::id_t::AX>,
                    regs::to_size<regs::id_t::BX>>::type::flags.CF == true
                    ,"");
        }
    }
}
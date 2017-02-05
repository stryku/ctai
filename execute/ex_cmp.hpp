#pragma once

#include "machine_state.hpp"
#include "instructions/instruction.hpp"
#include "execute/ex_instruction.hpp"
#include "instructions/ids_vaules.hpp"

#include <numeric>
#include <limits>

namespace cai
{
    namespace execute
    {
        template<typename state_t, size_t reg1, size_t reg2, size_t ...rest_of_opcodes>
        struct ex_instruction<state_t, inst::to_size<inst::id_t::CMP_REG_REG>, reg1, reg2, rest_of_opcodes...>
        {
            using state = to_machine_state<state_t>;

            static constexpr auto val1 = get_reg<typename state::registers_state_t, regs::to_id<reg1>>; // get reg1 value
            static constexpr auto val2 = get_reg<typename state::registers_state_t, regs::to_id<reg2>>; // get reg1 value
            static constexpr int64_t result = static_cast<int64_t>(val1) - static_cast<int64_t>(val2);

            static constexpr bool zf = (val1 == val2);
            static constexpr bool cf = (static_cast<uint64_t>(val1) < static_cast<uint64_t>(val2)); //set if borrow
            static constexpr bool sf = (result < 0);
            static constexpr bool of = (result < static_cast<int64_t>(std::numeric_limits<int32_t>::min()) ||
                                        result > static_cast<int64_t>(std::numeric_limits<int32_t>::max()));

            using new_flags_state = flags<cf, zf, sf, of>;

        using final_regs_state = adjust_eip<typename state::registers_state_t, inst::id_t::CMP_REG_REG>;


        using type = machine_state<typename state::stack_t, new_flags_state, final_regs_state>;
        };

        namespace tests
        {
            using cmp_test_machine_state = machine_state<startup_stack, startup_flags_state, registers_state<0x0,0xaaaabbbb, 4, 5,0,0,0,0, 0>>;

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

            static_assert(
                    ex_instruction<
                    cmp_test_machine_state,
                    inst::to_size<inst::id_t::CMP_REG_REG>,
                    regs::to_size<regs::id_t::EAX>,
                    regs::to_size<regs::id_t::EBX>>::type::flags.CF == true
                    ,"");

            static_assert(
                    ex_instruction<
                    cmp_test_machine_state,
                    inst::to_size<inst::id_t::CMP_REG_REG>,
                    regs::to_size<regs::id_t::ECX>,
                    regs::to_size<regs::id_t::EDX>>::type::flags.SF == true
                    ,"");
        }
    }
}
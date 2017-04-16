#pragma once

#include "memory/memory.hpp"
#include "execute/instructions/ex_popa.hpp"
#include "instructions/ids_vaules.hpp"
#include "tokenize/tokens.hpp"
#include "tuple.hpp"
#include "tests/tests_macros.hpp"
#include "machine/machine_state.hpp"

namespace ctai::tests::ex_popa
{
    template <auto ...values>
    using vc = ctai::values_container_n::values_container<values...>;

    using test_memory = ctai::memory::memory<33,
                                             vc<0, //reserved
                                                8, 8, 8, 8,
                                                7, 7, 7, 7,
                                                6, 6, 6, 6,
                                                0, 0, 0, 33, //esp
                                                4, 4, 4, 4, //ebx
                                                3, 3, 3, 3, //edx
                                                2, 2, 2, 2, //ecx
                                                1, 1, 1, 1>>;//eax

    using test_registers = ctai::registers_state<0, //eax
                                                     0,   //ebx
                                                     0, //ecx
                                                     0, //edx
                                                     1, //esp
                                                     0,
                                                     0,
                                                     0,
                                                     0>;//eip

    using test_thread = ctai::thread::thread<false,
                                             0,
                                             0,
                                             test_registers,
                                             ctai::startup_flags_state>;

    using test_machine_state = machine::state<test_memory,
            vc<>,
            ctai::tuple_n::tuple<>,
            ctai::io::output::buffer<>,
            0, 0>;

    namespace test_ex_pusha
    {
        using expected_registers = ctai::registers_state<0x01010101, //eax
                                                         0x04040404,   //ebx
                                                         0x02020202, //ecx
                                                         0x03030303, //edx
                                                         33, //esp
                                                         0x06060606,
                                                         0x07070707,
                                                         0x08080808,
                                                         1>;//eip

        using execute_result = ctai::execute2::ex_instruction<test_thread,
                test_machine_state,
                                                              ctai::inst::to_size<ctai::inst::id_t::POPA>>;

        using result_registers = typename execute_result::result_thread::registers;

        ASSERT_EQ_T(result_registers, expected_registers);
    }
}

#pragma once

#include "memory/memory.hpp"
#include "execute/instructions/ex_pusha.hpp"
#include "instructions/ids_vaules.hpp"
#include "tokenize/tokens.hpp"
#include "tuple.hpp"
#include "tests/tests_macros.hpp"
#include "machine/machine_state.hpp"
#include "tuple/tuple.hpp"


namespace ctai::tests::ex_pusha
{
    template <auto ...values>
    using vc = ctai::values_container_n::values_container<values...>;

    using test_memory = ctai::memory::memory_create<33>;

    using test_registers = ctai::registers_state<0x01010101, //eax
                                                 0x04040404,   //ebx
                                                 0x02020202, //ecx
                                                 0x03030303, //edx
                                                 33, //esp
                                                 0x06060606,
                                                 0x07070707,
                                                 0x08080808,
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
            ctai::io::input::buffer<>,
            0,
                    0>;


    namespace test_ex_pusha
    {
        using expected_memory = ctai::memory::memory<33,
                                                     vc<static_cast<uint8_t>(0), //reserved
                                                        8, 8, 8, 8,
                                                        7, 7, 7, 7,
                                                        6, 6, 6, 6,
                                                        0, 0, 0, 33, //esp
                                                        4, 4, 4, 4, //ebx
                                                        3, 3, 3, 3, //edx
                                                        2, 2, 2, 2, //ecx
                                                        1, 1, 1, 1>>;//eax

        using expected_mem_block =  ctai::values_container_n::cast_values_to<typename expected_memory::memory_block_t, uint32_t>;

        constexpr auto expected_esp = 1;

        using execute_result = ctai::execute2::ex_instruction<test_thread,
                test_machine_state,
                                                              ctai::inst::to_size<ctai::inst::id_t::PUSHA>>;

        using result_machine_state = typename execute_result::result_machine_state;
        using result_mem_block = ctai::values_container_n::cast_values_to<typename result_machine_state::memory::memory_block_t, uint32_t>;

        using result_registers = typename execute_result::result_thread::registers;
        constexpr auto result_esp = ctai::get_reg<result_registers, ctai::regs::id_t::ESP>;

        ASSERT_EQ_T(expected_mem_block, result_mem_block);
        ASSERT_EQ(expected_esp, result_esp);
    }
}

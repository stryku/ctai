#pragma once

#include "containers/values_container.hpp"
#include "instructions/matchers/sys_exit_thread_matcher.hpp"
#include "instructions/ids_vaules.hpp"
#include "tokenize/syscalls_tokens.hpp"
#include "tuple.hpp"

namespace ctai::tests::sys_exit_thread_matcher
{
    template <auto ...values>
    using vc = ctai::values_container_n::values_container<values...>;

    namespace test_sys_exit_thread_matcher_opcodes
    {
        using expected_opcodes = vc<inst::to_size<inst::id_t::SYS_EXIT_THREAD>>;
        using test_result = instruction_match<ctai::tuple<tokens::tok_sys_exit_thread, string<>, string<>>>::instruction;

        ASSERT_EQ_T(test_result, expected_opcodes);
    }

    namespace test_sys_exit_thread_matcher_tokens
    {
        using expected_tokens = ctai::tuple<tokens::tok_sys_exit_thread>;
        using test_result = instruction_match<ctai::tuple<tokens::tok_sys_exit_thread, string<>, string<>>>::instruction_tokens;

        ASSERT_EQ_T(test_result, expected_tokens);
    }
}

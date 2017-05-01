#pragma once

#include "containers/values_container.hpp"
#include "instructions/matchers/call_matcher.hpp"
#include "instructions/matchers/instructions_matchers.hpp"
#include "instructions/ids_vaules.hpp"
#include "tuple.hpp"

namespace ctai::tests::call_matcher
{
    template <auto ...values>
    using vc = ctai::values_container_n::values_container<values...>;

    namespace test_call_matcher_opcodes
    {
        using expected_opcodes = vc<inst::to_size<inst::id_t::CALL_VAL>, static_cast<size_t>(22)>;
        using test_result = instruction_match<ctai::tuple<tokens::tok_call, string<'2','2'>, string<>, string<>>>::instruction;

        ASSERT_EQ_T(test_result, expected_opcodes);
    }

    namespace test_call_matcher_tokens
    {
        using expected_tokens = ctai::tuple<tokens::tok_call, string<'2','2'>>;
        using test_result = instruction_match<ctai::tuple<tokens::tok_call, string<'2','2'>, string<>, string<>>>::instruction_tokens;

        ASSERT_EQ_T(test_result, expected_tokens);
    }
}

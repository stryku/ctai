#pragma once

#include "ctai/containers/values_container.hpp"
#include "ctai/instructions/matchers/call_matcher.hpp"
#include "ctai/instructions/matchers/instructions_matchers.hpp"
#include "ctai/instructions/ids_vaules.hpp"
#include "ctai/tuple.hpp"

namespace ctai::tests::pusha_matcher
{
    template <auto ...values>
    using vc = ctai::values_container_n::values_container<values...>;

    namespace test_pusha_matcher_opcodes
    {
        using expected_opcodes = vc<inst::to_size<inst::id_t::PUSHA>>;
        using test_result = instruction_match<ctai::tuple<tokens::tok_pusha, string<'2','2'>, string<>, string<>>>::instruction;

        ASSERT_EQ_T(test_result, expected_opcodes);
    }

    namespace test_pusha_matcher_tokens
    {
        using expected_tokens = ctai::tuple<tokens::tok_pusha>;
        using test_result = instruction_match<ctai::tuple<tokens::tok_pusha, string<'2','2'>, string<>, string<>>>::instruction_tokens;

        ASSERT_EQ_T(test_result, expected_tokens);
    }
}

#pragma once

#include "tuple.hpp"
#include "tokenize/tokens.hpp"
#include "instructions/ids_vaules.hpp"
#include "instructions/matchers/operand_decoder.hpp"
#include "containers/values_container.hpp"
#include "eip_change.hpp"

namespace ctai
{
    namespace details
    {
        // jl instruction pointer / label
        template <typename operand, typename ...rest_of_tokens>
        struct matcher_impl<tuple<
                tokens::tok_jl,
                operand,
                rest_of_tokens...>>
        {
            static constexpr auto instruction_type = inst::id_t::JL;
            static constexpr auto decoded = is_label_token<operand> ? 0 : operand_decoder<operand>;

            using instruction = values_container_n::values_container<
                    inst::to_size<instruction_type>,
                    decoded>;

            static constexpr auto eip_change = get_eip_change<instruction_type>;

            using rest_of_tokens_t = tuple<rest_of_tokens...>;
        };
    }

    template <typename tokens>
    using instruction_match = typename details::matcher_impl<tokens>;
}
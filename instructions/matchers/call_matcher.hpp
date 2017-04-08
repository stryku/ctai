#pragma once

#include "tuple.hpp"
#include "tokenize/tokens.hpp"
#include "instructions/ids_vaules.hpp"
#include "instructions/matchers/operand_decoder.hpp"
#include "eip_change.hpp"
#include "string.hpp"
#include "containers/values_container.hpp"

namespace ctai
{
    namespace details
    {
        // call .label
        template <char ...label_chars, typename ...rest_of_tokens>
        struct matcher_impl<tuple<
                            tokens::tok_call,
                            string<'.', label_chars...>,
                            rest_of_tokens...>>
        {
            static constexpr auto instruction_type = inst::id_t::CALL_VAL;

            using instruction = values_container_n::values_container<
                inst::to_size<instruction_type>,
                0>;

            static constexpr auto eip_change = get_eip_change<instruction_type>;
            using instruction_tokens = tuple<
                    tokens::tok_call,
                    string<'.', label_chars...>>;

            using rest_of_tokens_t = tuple<rest_of_tokens...>;
        };

        // call instruction_pointer
        template <typename operand, typename ...rest_of_tokens>
        struct matcher_impl<tuple<
                tokens::tok_call,
                operand,
                rest_of_tokens...>>
        {
            static constexpr auto instruction_type = inst::id_t::CALL_VAL;

            using instruction = values_container_n::values_container<
                    inst::to_size<instruction_type>,
                    operand_decoder<operand>>;

            static constexpr auto eip_change = get_eip_change<instruction_type>;
            using instruction_tokens = tuple<
                    tokens::tok_call,
                    operand>;

            using rest_of_tokens_t = tuple<rest_of_tokens...>;
        };
    }
}
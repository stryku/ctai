#pragma once

#include "tuple.hpp"
#include "tokenize/tokens.hpp"
#include "instructions/ids_vaules.hpp"
#include "instructions/matchers/operand_decoder.hpp"

namespace cai
{
    namespace details
    {
        // add reg, reg/val
        template <typename reg_token, typename operand, typename ...rest_of_tokens>
        struct matcher_impl<tuple<
                tokens::tok_mov,
                reg_token,
                tokens::tok_comma,
                operand,
                rest_of_tokens...>>
        {
            static constexpr auto instruction_type = is_reg_token<operand> ? inst::id_t::ADD_REG_REG : inst::id_t::ADD_REG_VAL;

            using instruction = values_container<
                    inst::to_size<instruction_type>,
                    token_to_reg_val<reg_token>,
                    operand_decoder<operand>>;

            using rest_of_tokens_t = tuple<rest_of_tokens...>;
        };
    }

    template <typename tokens>
    using instruction_match = typename details::matcher_impl<tokens>;

    namespace tests
    {
        static_assert(std::is_same<instruction_match<tuple<tokens::tok_pop, tokens::tok_eax, string<>, string<>>>::instruction,
                values_container<inst::to_size<inst::id_t::POP_REG>, regs::to_size<regs::id_t::EAX>>>::value, "");
    }
}
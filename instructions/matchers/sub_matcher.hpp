#pragma once

#include "tuple.hpp"
#include "tokenize/tokens.hpp"
#include "instructions/ids_vaules.hpp"
#include "instructions/matchers/operand_decoder.hpp"
#include "containers/values_container.hpp"

namespace ctai
{
    namespace details
    {
        // sub reg, reg/val
        template <typename reg_token, typename operand, typename ...rest_of_tokens>
        struct matcher_impl<tuple<tokens::tok_sub,
                                  reg_token,
                                  tokens::tok_comma,
                                  operand,
                                  rest_of_tokens...>>
        {
            static constexpr auto instruction_type = is_reg_token<operand> ? inst::id_t::SUB_REG_REG : inst::id_t::SUB_REG_VAL;

            using instruction = values_container_n::values_container<inst::to_size<instruction_type>,
                                                                     token_to_reg_opcode<reg_token>,
                                                                     operand_decoder<operand>>;

            static constexpr auto eip_change = get_eip_change<instruction_type>;
            using instruction_tokens = tuple<tokens::tok_sub,
                                             reg_token,
                                             tokens::tok_comma,
                                             operand>;

            using rest_of_tokens_t = tuple<rest_of_tokens...>;
        };
    }
}
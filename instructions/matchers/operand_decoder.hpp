#pragma once

#include "tuple.hpp"
#include "tokenize/tokens.hpp"
#include "instructions/ids_vaules.hpp"


// We assume that operand can be reg or const value
namespace cai
{
    namespace details
    {

        template <typename>
        struct operand_decoder_impl;

        template <typename str>
        struct operand_decoder_impl
        {
            static constexpr size_t value =
                    is_reg_token<str>
                        ? token_to_reg_val<str>
                        : string_to_int<str>;
        };
    }

    template <typename token>
    constexpr auto operand_decoder = details::operand_decoder_impl<token>::value;

}

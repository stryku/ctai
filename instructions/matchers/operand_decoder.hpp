#pragma once

#include "tuple.hpp"
#include "tokenize/tokens.hpp"
#include "instructions/ids_vaules.hpp"

#include <type_traits>

// We assume that operand can be reg or const value
namespace ctai
{
    namespace details
    {
        template <typename token>
        struct reg_token_decoder
        {
            static constexpr size_t value = token_to_reg_opcode<token>;
        };

        template <typename token>
        struct const_val_token_decoder
        {
            static constexpr size_t value = string_to_int<token>;
        };

        template <typename>
        struct operand_decoder_impl;

        template <typename str>
        struct operand_decoder_impl
        {
            static constexpr size_t value =
                    std::conditional_t<is_reg_token<str>,
                                reg_token_decoder<str>,
                                const_val_token_decoder<str>>::value;
        };

        template <char ...rest_of_chars>
        struct operand_decoder_impl<ctai::string<'.', rest_of_chars...>>
        {
            static constexpr size_t value = 0;
        };
    }

    template <typename token>
    constexpr auto operand_decoder = details::operand_decoder_impl<token>::value;

    namespace tests
    {
        static_assert(operand_decoder<decltype("eax"_s)> == regs::to_size<regs::id_t::EAX>,"");
        static_assert(operand_decoder<decltype("2"_s)> == 2,"");
        static_assert(operand_decoder<decltype("-2"_s)> == -2,"");
    }
}

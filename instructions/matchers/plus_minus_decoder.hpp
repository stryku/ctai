#pragma once

#include "tokenize/tokens.hpp"

#include <type_traits>

namespace cai
{
    namespace details
    {

        template <typename>
        struct plus_minus_decoder_impl;

        template <typename str>
        struct plus_minus_decoder_impl
        {
            static constexpr bool value = std::is_same<str, tokens::tok_plus>::value;
        };
    }

    template <typename token>
    constexpr auto is_plus = details::plus_minus_decoder_impl<token>::value;
}

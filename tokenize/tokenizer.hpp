#pragma once

#include <type_traits>

#include "string.hpp"

namespace cai
{
    //
    // get_token
    //
    namespace details
    {
        template <typename str, typename curr_token>
        struct get_token_impl;

        template <typename current_token>
        struct get_token_impl<string<>, current_token>
        {
            using result_token = current_token;
            using result_string = string<>;
        };

        template <char ...str_chars, char ...token_chars>
        struct get_token_impl<string<' ', str_chars...>, string<token_chars...>>
        {
            using str = string<str_chars...>;
            using token = string<token_chars...>;

            using result_token = token;
            using result_string = str;
        };

        template <char curr_char, char ...str_chars, char ...token_chars>
        struct get_token_impl<string<curr_char, str_chars...>, string<token_chars...>>
        {
            using str = string<curr_char, str_chars...>;
            using token = string<token_chars...>;

            //static constexpr auto curr_char = string_front<str>;
            using popped_str = string<str_chars...>;

            using result_token = typename get_token_impl<popped_str, string_append<token, curr_char>>::result_token;
            using result_string = typename get_token_impl<popped_str, string_append<token, curr_char>>::result_string;
        };
    }

    template <typename s>
    using get_token = details::get_token_impl<s, string<>>;

    template <typename str, typename current_tokens>
    struct tokenizer
    {

    };

    namespace tests
    {
        static_assert(std::is_same<typename get_token<decltype("abc def"_s)>::result_token,
                                   decltype("abc"_s)>::value, "");

        static_assert(std::is_same<typename get_token<decltype("abc def"_s)>::result_string,
                                   decltype("def"_s)>::value, "");
    }
}
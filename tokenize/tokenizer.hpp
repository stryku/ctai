#pragma once

#include "string.hpp"
#include "tuple.hpp"

#include <type_traits>


namespace cai
{
    //
    // get_token
    //
    namespace details
    {
        template <typename str, typename curr_token = string<>>
        struct get_token_impl;

        template <typename current_token>
        struct get_token_impl<string<>, current_token>
        {
            using result_token = current_token;
            using rest_of_string = string<>;
        };

        template <char ...str_chars, typename current_token>
        struct get_token_impl<string<' ', str_chars...>, current_token>
        {
            using result_token = current_token;
            using rest_of_string = string<str_chars...>;
        };

        template <char curr_char, char ...str_chars, typename current_token>
        struct get_token_impl<string<curr_char, str_chars...>, current_token>
        {
            using string_without_token_char = string<str_chars...>;

            using result_t = get_token_impl<string_without_token_char, string_append<current_token, curr_char>>;

            using result_token = typename result_t::result_token;
            using rest_of_string = typename result_t::rest_of_string;
        };
    }

    template <typename s>
    using get_token = details::get_token_impl<s>;

    namespace details
    {
        template <typename s, typename current_tokens = tuple<>>
        struct tokenize_impl;

        template <typename current_tokens>
        struct tokenize_impl<string<>, current_tokens>
        {
            using tokens = current_tokens;
        };

        template <char ...str_chars, typename current_tokens>
        struct tokenize_impl<string<str_chars...>, current_tokens>
        {
            using str = string<str_chars...>;
            using get_token_t = get_token<str>;

            using next_tokens = tuple_append<current_tokens, typename get_token_t::result_token>;
            using next_string = typename get_token_t::rest_of_string;

            using tokens = typename tokenize_impl<next_string, next_tokens>::tokens;
        };
    }

    template <typename s>
    using tokenize = typename details::tokenize_impl<s>::tokens;

    namespace tests
    {
        static_assert(std::is_same<typename get_token<decltype("abc def"_s)>::result_token,
                                   decltype("abc"_s)>::value, "");

        static_assert(std::is_same<typename get_token<decltype("abc def"_s)>::rest_of_string,
                                   decltype("def"_s)>::value, "");

        static_assert(std::is_same<tokenize<decltype("abc def"_s)>,
                                   tuple<decltype("abc"_s), decltype("def"_s)>>::value, "");
    }
}
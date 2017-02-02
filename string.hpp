#pragma once

namespace cai
{
    template <char ...chars>
    struct string
    {
        static constexpr auto size = sizeof...(chars);
    };

    namespace details
    {
        template <typename str>
        struct to_string_impl;

        template <char ...chars>
        struct to_string_impl<string<chars...>>
        {
            using type = string<chars...>;
        };
    }

    template <typename s>
    using to_string = typename details::to_string_impl<s>::type;

    namespace details
    {
        template <typename, typename, bool eq_size>
        struct string_eq_impl;

        // different size
        template <char ...lchars, char ...rchars>
        struct string_eq_impl<string<lchars...>, string<rchars...>, false>
        {
            static constexpr auto value = false;
        };

        template <>
        struct string_eq_impl<string<>, string<>, true>
        {
            static constexpr auto value = true;
        };

        template <char eq_first, char ...lchars, char ...rchars>
        struct string_eq_impl<string<eq_first, lchars...>, string<eq_first, rchars...>, true>
        {
            static constexpr auto value = string_eq_impl<string<lchars...>, string<rchars...>, true>::value;
        };

        template <char lfirst, char rfirst, char ...lchars, char ...rchars>
        struct string_eq_impl<string<lfirst, lchars...>, string<rfirst, rchars...>, true>
        {
            static constexpr auto value = false;
        };
    }

    template <typename ls, typename rs>
    constexpr auto string_eq = details::string_eq_impl<ls, rs, to_string<ls>::size == to_string<rs>::size>::value;

    namespace tests
    {
        static_assert(
                string_eq<string<'a','b'>, string<'a','b'>>
                ,"");

        static_assert(
                !string_eq<string<'a','b'>, string<'A','b'>>
                ,"");

        static_assert(
                !string_eq<string<'a','b','c'>, string<'a','b'>>
                ,"");

        static_assert(
                !string_eq<string<'a','b'>, string<'A','b'>>
                ,"");

        static_assert(
                !string_eq<string<'a','b'>, string<'a','b','c'>>
                ,"");
    }
}
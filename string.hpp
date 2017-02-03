#pragma once

namespace cai
{
    template <char ...chars>
    struct string
    {
        static constexpr auto size = sizeof...(chars);
    };


    //
    // to_string
    //
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

    //
    //string_eq
    //
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

    //
    //string_front
    //
    namespace details
    {
        template <typename>
        struct string_front_impl;

        template <char front, char ...chars>
        struct string_front_impl<string<front, chars...>>
        {
            static constexpr auto value = front;
        };
    }

    template <typename s>
    constexpr auto string_front = details::string_front_impl<s>::value;

    //
    //string_append
    //
    namespace details
    {
        template <typename, char>
        struct string_append_impl;

        template <char ...chars, char value>
        struct string_append_impl<string<chars...>, value>
        {
            using type = string<chars..., value>;
        };
    }

    template <typename s, char value>
    using string_append = typename details::string_append_impl<s, value>::type;

    //
    //string_pop_front
    //
    namespace details
    {
        template <typename>
        struct string_pop_front_impl;

        template <char front, char ...chars>
        struct string_pop_front_impl<string<front, chars...>>
        {
            using type = string<chars...>;
        };
    }

    template <typename s>
    using string_pop_front = typename details::string_pop_front_impl<s>::type;



}

template <typename T, T... chars>
constexpr auto operator""_s() {
    return cai::string<chars...>{};
}

namespace cai
{
    namespace tests
    {
        static_assert(string_eq<string<'a','b'>, string<'a','b'>>, "");
        static_assert(string_eq<string<'a','b'>, decltype("ab"_s)>, "");
        static_assert(!string_eq<string<'a','b'>, string<'A','b'>>, "");
        static_assert(!string_eq<string<'a','b','c'>, string<'a','b'>>, "");
        static_assert(!string_eq<string<'A','b'>, string<'a','b'>>, "");
        static_assert(!string_eq<string<'a','b'>, string<'a','b','c'>>, "");

        static_assert(string_front<decltype("abc"_s)> == 'a', "");
        static_assert(string_front<decltype(" abc"_s)> == ' ', "");
    }
}
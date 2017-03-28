#pragma once

namespace ctai
{
    namespace values_container
    {
        template<auto ...values>
        struct values_container
        {
            static constexpr auto size = sizeof...(values);
        };

        //
        //front
        //
        namespace details
        {
            template<typename container>
            struct front_impl;

            template<auto front, auto ...chars>
            struct front_impl<values_container<front, chars...>>
            {
                static constexpr auto value = front;
            };
        }

        template<typename container>
        constexpr auto front = details::front_impl<container>::value;


        //
        //append
        //
        namespace details
        {
            template <typename container, auto value>
            struct append_impl;

            template <auto ...chars, auto value>
            struct append_impl<values_container<chars...>, value>
            {
                using result = values_container<chars..., value>;
            };
        }

        template <typename container, auto value>
        using append = typename details::append_impl<container, value>::result;

        //
        //string_merge
        //
        namespace details
        {
            template <typename container1, typename container2>
            struct merge_impl;

            template <auto ...values1, auto ...values2>
            struct merge_impl<values_container<values1...>, values_container<values2...>>
            {
                using result = values_container<values1..., values2...>;
            };
        }

        template <typename container1, typename container2>
        using merge = typename details::merge_impl<container1, container2>::result;

        //
        //pop_front
        //
        namespace details
        {
            template <typename container>
            struct pop_front_impl;

            template <auto front, auto ...values>
            struct pop_front_impl<values_container<front, values...>>
            {
                using result = values_container<values...>;
            };
        }

        template <typename container>
        using pop_front = typename details::pop_front_impl<container>::result;
    }
}
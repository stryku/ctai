#pragma once

#include "utils/empty_type.hpp"
#include "utils/bad_value.hpp"
#include "utils/predicate.hpp"

namespace ctai
{
    namespace tuple_n
    {
        template<typename ...args>
        struct tuple
        {};

        //
        //append
        //
        namespace details
        {
            template<typename,
                     typename>
            struct append_impl;

            template<typename ...args,
                     typename tail>
            struct append_impl<tuple<args...>, tail>
            {
                using type = tuple<args..., tail>;
            };
        }

        template<typename tup,
                 typename tail>
        using append = typename details::append_impl<tup, tail>::type;

        //
        // merge
        //
        namespace details
        {
            template<typename left_tuple,
                     typename right_tuple>
            struct merge_impl;

            template<typename ...ltypes,
                     typename ...rtypes>
            struct merge_impl<tuple<ltypes...>,
                                    tuple<rtypes...>>
            {
                using type = tuple<ltypes...,
                                   rtypes...>;
            };
        }

        template<typename l, typename r>
        using merge = typename details::merge_impl<l, r>::type;

        //
        // take
        //
        namespace details
        {
            template <size_t count, bool end, typename rest, typename taken = tuple<>>
            struct take_impl;

            template <typename ...rest_of_types, typename ...taken_types>
            struct take_impl<0, true, tuple<rest_of_types...>, tuple<taken_types...>>
            {
                using type = take_impl;

                using taken = tuple<taken_types...>;
                using rest = tuple<rest_of_types...>;
            };

            template <size_t count, typename to_take, typename ...rest_of_types, typename ...taken_types>
            struct take_impl<count, false, tuple<to_take, rest_of_types...>, tuple<taken_types...>>
            {
                using type = take_impl<count-1,
                                       count == 1,
                                       tuple<rest_of_types...>,
                                       tuple<taken_types..., to_take>>;

                using taken = typename type::taken;
                using rest = typename type::rest;
            };
        }

        template <size_t count, typename tuple_t>
        using take = typename details::take_impl<count, count==0, tuple_t>::type;

        //
        //find_if
        //
        namespace details
        {
            template <typename tuple_t, typename predicate>
            struct find_if_impl;

            template <typename predicate>
            struct find_if_impl<tuple<>, predicate>
            {
                using result = utils::empty_type;
            };

            template <typename current_type,
                      typename ...rest_of_types,
                      typename predicate>
            struct find_if_impl<tuple<current_type, rest_of_types...>,
                                predicate>
            {
                using result = std::conditional_t<utils::predicate_v<predicate, current_type>,
                                                  current_type,
                                                  typename find_if_impl<tuple<rest_of_types...>,
                                                                        predicate>::result>;

            };
        }

        template <typename tuple_t, typename predicate>
        using find_if = typename details::find_if_impl<tuple_t, predicate>::result;

        //
        //find_if_it
        //
        namespace details
        {
            template <typename tuple_t, typename predicate, size_t iterator = 0>
            struct find_if_it_impl;

            template <typename predicate, size_t iterator>
            struct find_if_it_impl<tuple<>, predicate, iterator>
            {
                static constexpr auto result = utils::bad_value;
            };

            template <typename current_type,
                      typename ...rest_of_types,
                      typename predicate,
                      size_t iterator>
            struct find_if_it_impl<tuple<current_type, rest_of_types...>,
                                   predicate,
                                   iterator>
            {
                static constexpr auto result = utils::predicate_v<predicate, current_type> ? iterator
                                                                                           : find_if_it_impl<tuple<rest_of_types...>,
                                                                                                             predicate,
                                                                                                             iterator + 1>::result;
            };
        }

        template <typename tuple_t, typename predicate>
        constexpr auto find_if_it = details::find_if_it_impl<tuple_t, predicate>::result;
    }
}
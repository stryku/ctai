#pragma once

#include "utils/empty_type.hpp"
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

        template<typename l,
                 typename r>
        using merge = typename details::merge_impl<l, r>::type;


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
    }
}
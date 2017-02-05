#pragma once

#include "labels.hpp"
#include "string.hpp"
#include "instructions/matchers/instructions_matchers.hpp"

namespace cai
{
    //
    // substitute_labels
    //
    namespace details
    {
        template <typename tokens, typename labels, typename final_tokens = tuple<>>
        struct substitute_labels_impl;

        template <typename labels, typename final_tokens>
        struct substitute_labels_impl<tuple<>, labels, final_tokens>
        {
            using tokens = final_tokens;
        };

        //label match
        template <char ...label_chars, typename ...rest_of_tokens, typename labels, typename ...current_tokens>
        struct substitute_labels_impl<
                tuple<string<'.', label_chars...>, rest_of_tokens...>,
                labels,
                tuple<current_tokens...>>
        {
            static constexpr auto ip = labels_get_ip<labels, string<'.', label_chars...>>;
            using str_ip = string_from_int<ip>;

            using substitued = substitute_labels_impl<
                    tuple<rest_of_tokens...>,
                    labels,
                    tuple<current_tokens..., str_ip>>;

            using tokens = typename substitued::tokens;
        };

        //normal token
        template <typename current_token, typename ...rest_of_tokens, typename labels, typename ...current_tokens>
        struct substitute_labels_impl<
                tuple<current_token, rest_of_tokens...>,
                labels,
                tuple<current_tokens...>>
        {
            using substitued = substitute_labels_impl<
                    tuple<rest_of_tokens...>,
                    labels,
                    tuple<current_tokens..., current_token>>;

            using tokens = typename substitued::tokens;
        };
    }

    template <typename tokens, typename labels>
    using substutite_labels = typename details::substitute_labels_impl<tokens, labels>::tokens;

    namespace tests
    {
        static_assert(
                std::is_same<
                        substutite_labels<tuple<decltype("abc"_s), decltype("def"_s)>, tuple<>>,
                        tuple<decltype("abc"_s), decltype("def"_s)>
                >::value
                ,"");

        static_assert(
                std::is_same<
                        substutite_labels<tuple<decltype(".label"_s), decltype("abc"_s), decltype("def"_s)>, tuple<label_metadata<decltype(".label"_s), 12>>>,
                        tuple<decltype("12"_s), decltype("abc"_s), decltype("def"_s)>
                >::value
                ,"");
    }
}
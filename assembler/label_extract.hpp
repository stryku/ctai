#pragma once

#include "labels.hpp"
#include "string.hpp"
#include "instructions/matchers/instructions_matchers.hpp"

namespace cai
{
    //
    // extract_labels
    //
    namespace details
    {
        template <typename, size_t current_ip = 0,  typename final_tokens = tuple<>, typename final_labels = tuple<>>
        struct extract_labels_impl;

        template <size_t ip, typename final_tokens, typename final_labels>
        struct extract_labels_impl<tuple<>, ip, final_tokens, final_labels>
        {
            using tokens = final_tokens;
            using labels = final_labels;
        };

        //label match
        template <size_t current_ip, char ...label_chars, typename ...rest_of_tokens, typename ...current_tokens, typename ...current_labels>
        struct extract_labels_impl<
                tuple<string<':', label_chars...>, rest_of_tokens...>,
                current_ip,
                tuple<current_tokens...>,
                tuple<current_labels...>>
        {
            using extracted = extract_labels_impl<
                    tuple<rest_of_tokens...>,
                    current_ip,
                    tuple<current_tokens...>,
                    tuple<current_labels..., label_metadata<string<'.', label_chars...>, current_ip>>>;

            using tokens = typename extracted::tokens;
            using labels = typename extracted::labels;
        };

        //normal instruction
        template <size_t current_ip, typename current_basic_token, typename ...basic_tokens, typename ...current_tokens, typename current_labels>
        struct extract_labels_impl<
                tuple<current_basic_token, basic_tokens...>,
                current_ip,
                tuple<current_tokens...>,
                current_labels>
        {
            using instruction = instruction_match<tuple<current_basic_token, basic_tokens...>>;
            using next_tokens = tuple_merge<tuple<current_tokens...>, typename instruction::instruction_tokens>;
            static constexpr auto nex_ip = current_ip + instruction::eip_change;

            using extracted = extract_labels_impl<
                    typename instruction::rest_of_tokens_t,
                    nex_ip,
                    next_tokens,
                    current_labels>;

            using tokens = typename extracted::tokens;
            using labels = typename extracted::labels;
        };
    }

    template <typename tokens>
    using extract_labels = details::extract_labels_impl<tokens>;

    namespace tests
    {
        static_assert(
                std::is_same<
                        extract_labels<tuple<decltype(":label0"_s), decltype("push"_s), decltype("eax"_s)>>::tokens,
                tuple<decltype("push"_s), decltype("eax"_s)>
                                          >::value
                ,"");

        static_assert(
                std::is_same<
                        extract_labels<tuple<decltype(":label0"_s), decltype("push"_s), decltype("eax"_s), decltype(":label1"_s)>>::labels,
                tuple<label_metadata<decltype(".label0"_s), 0>,
                label_metadata<decltype(".label1"_s), 2>>
                                          >::value
                ,"");
    }
}
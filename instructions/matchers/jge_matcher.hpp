#pragma once

#include "tuple.hpp"
#include "tokenize/tokens.hpp"
#include "instructions/ids_vaules.hpp"
#include "instructions/matchers/operand_decoder.hpp"

namespace cai
{
    namespace details
    {
        // jmp instruction_pointer
        template <typename ip, typename ...rest_of_tokens>
        struct matcher_impl<tuple<
                tokens::tok_jge,
                ip,
                rest_of_tokens...>>
        {
            static constexpr auto instruction_type = inst::id_t::JGE;

            using instruction = values_container<
                    inst::to_size<instruction_type>,
                    operand_decoder<ip>>;

            static constexpr auto eip_change = get_eip_change<instruction_type>;
            using instruction_tokens = tuple<
                    tokens::tok_add,
                    ip>;

            using rest_of_tokens_t = tuple<rest_of_tokens...>;
        };

        // jmp .label_name
        template <char ...label_chars, typename ...rest_of_tokens>
        struct matcher_impl<tuple<
                            tokens::tok_jge,
                string<'.', label_chars...>,
                rest_of_tokens...>>
        {
            static constexpr auto instruction_type = inst::id_t::JGE;

            using instruction = values_container<
            inst::to_size<instruction_type>, 0>;

            static constexpr auto eip_change = get_eip_change<instruction_type>;
            using instruction_tokens = tuple<
                    tokens::tok_jge,
                    string<'.', label_chars...>>;

            using rest_of_tokens_t = tuple<rest_of_tokens...>;
        };
    }

    template <typename tokens>
    using instruction_match = typename details::matcher_impl<tokens>;

    namespace tests
    {
        static_assert(std::is_same<instruction_match<tuple<tokens::tok_jge, decltype("22"_s), string<>, string<>>>::instruction,
                values_container<
                        inst::to_size<inst::id_t::JGE>,
                        22
                >>::value, "");
    }
}
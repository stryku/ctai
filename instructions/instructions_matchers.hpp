#pragma once

#include "tuple.hpp"
#include "tokenize/tokens.hpp"
#include "ids_vaules.hpp"

namespace cai
{
    namespace details
    {
        template <typename tokens>
        struct matcher_impl;

        template <typename ...rest_of_tokens>
        struct matcher_impl<tuple<tokens::tok_push, tokens::tok_eax, rest_of_tokens...>>
        {
            using instruction = values_container<
                    inst::to_size<inst::id_t::PUSH_REG>,
                    regs::to_size<regs::id_t::EAX>>;

            using rest_of_tokens_t = tuple<rest_of_tokens...>;
        };

        template <typename ...rest_of_tokens>
        struct matcher_impl<tuple<tokens::tok_pop, tokens::tok_ebx, rest_of_tokens...>>
        {
            using instruction = values_container<
                    inst::to_size<inst::id_t::POP_REG>,
                    regs::to_size<regs::id_t::EBX>>;

            using rest_of_tokens_t = tuple<rest_of_tokens...>;
        };
    }

    template <typename tokens>
    using instruction_match = typename details::matcher_impl<tokens>;

    namespace tests
    {
        static_assert(std::is_same<instruction_match<tuple<tokens::tok_push, tokens::tok_eax, string<>, string<>>>::instruction,
                                   values_container<inst::to_size<inst::id_t::PUSH_REG>, regs::to_size<regs::id_t::EAX>>>::value, "");
    }
}
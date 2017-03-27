#pragma once

#include "tuple.hpp"
#include "tokenize/tokens.hpp"
#include "instructions/ids_vaules.hpp"
#include "eip_change.hpp"

namespace ctai
{
    namespace details
    {
        template <typename tokens>
        struct matcher_impl;

        template <typename ...rest_of_tokens>
        struct matcher_impl<tuple<tokens::tok_exit, rest_of_tokens...>>
        {
            using instruction = values_container<inst::to_size<inst::id_t::EXIT>>;
            static constexpr auto eip_change = get_eip_change<inst::id_t::EXIT>;

            using instruction_tokens = tuple<tokens::tok_exit>;
            using rest_of_tokens_t = tuple<rest_of_tokens...>;
        };
    }

    template <typename tokens>
    using instruction_match = typename details::matcher_impl<tokens>;
}

#include "push_matcher.hpp"
#include "pop_matcher.hpp"
#include "mov_matcher.hpp"
#include "add_matcher.hpp"
#include "jmp_matcher.hpp"
#include "jge_matcher.hpp"
#include "cmp_matcher.hpp"

namespace ctai
{
    namespace tests
    {
        static_assert(std::is_same<instruction_match<tuple<tokens::tok_push, tokens::tok_eax, string<>, string<>>>::instruction,
                values_container<inst::to_size<inst::id_t::PUSH_REG>, regs::to_size<regs::id_t::EAX>>>::value, "");
    }
}
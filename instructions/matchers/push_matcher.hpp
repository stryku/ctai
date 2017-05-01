#pragma once

#include "tuple.hpp"
#include "tokenize/tokens.hpp"
#include "instructions/ids_vaules.hpp"
#include "instructions/matchers/operand_decoder.hpp"
#include "containers/values_container.hpp"

namespace ctai
{
    namespace details
    {
        //push op
        template <typename operand, typename ...rest_of_tokens>
        struct matcher_impl<tuple<tokens::tok_push, operand, rest_of_tokens...>>
        {
            using instruction = values_container_n::values_container<
                    inst::to_size<inst::id_t::PUSH_REG>,
                    operand_decoder<operand>>;

            static constexpr auto eip_change = get_eip_change<inst::id_t::PUSH_REG>;
            using instruction_tokens = tuple<tokens::tok_push, operand>;

            using rest_of_tokens_t = tuple<rest_of_tokens...>;
        };
    }

    template <typename tokens>
    using instruction_match = typename details::matcher_impl<tokens>;

    namespace tests
    {
        static_assert(std::is_same<instruction_match<tuple<tokens::tok_push, tokens::tok_eax, string<>, string<>>>::instruction,
                                   values_container_n::values_container<inst::to_size<inst::id_t::PUSH_REG>, regs::to_size<regs::id_t::EAX>>>::value, "");
    }
}
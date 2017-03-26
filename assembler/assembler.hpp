#pragma once

#include "instructions/matchers/instructions_matchers.hpp"
#include "instructions/matchers/push_matcher.hpp"
#include "tuple.hpp"

namespace cai
{

    //
    // get_instruction
    //
    namespace details
    {
        template <typename>
        struct get_instruction_impl;

        template <typename ...tokens>
        struct get_instruction_impl<tuple<tokens...>>
        {
            using match = instruction_match<tuple<tokens...>>;
            using instruction = typename match::instruction;
            using res_of_tokens = typename match::rest_of_tokens_t;
        };
    }

    template <typename tokens>
    using get_instruction = details::get_instruction_impl<tokens>;

    namespace details
    {
        template <typename tokens, typename instructions_t = values_container<>>
        struct assemble_impl;

        template <typename curr_instructions>
        struct assemble_impl<tuple<>, curr_instructions>
        {
            using instructions_result = curr_instructions;
        };

        template <typename tokens, typename curr_instructions>
        struct assemble_impl
        {
            using instruction = get_instruction<tokens>;

            using next_instructions = values_merge<curr_instructions, typename instruction::instruction>;
            using next_tokens = typename instruction::res_of_tokens;

            using instructions_result = typename assemble_impl<next_tokens, next_instructions>::instructions_result;
        };
    }

    template <typename tokens>
    using assemble = typename details::assemble_impl<tokens>::instructions_result;

    namespace tests
    {
        //push eax
        static_assert(std::is_same<assemble<tuple<tokens::tok_push, tokens::tok_eax>>,
                                   values_container<inst::to_size<inst::id_t::PUSH_REG>, regs::to_size<regs::id_t::EAX>>>::value,"");

        //push eax
        //pop ebx
        static_assert(std::is_same<assemble<tuple<tokens::tok_push, tokens::tok_eax, tokens::tok_pop, tokens::tok_ebx>>,
                                   values_container<inst::to_size<inst::id_t::PUSH_REG>, regs::to_size<regs::id_t::EAX>,
                                                    inst::to_size<inst::id_t::POP_REG>, regs::to_size<regs::id_t::EBX>>>::value,"");
    }
}
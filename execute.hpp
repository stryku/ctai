#pragma once

#include "instructions/ids_vaules.hpp"
#include "register.hpp"
#include "tuple.hpp"
#include "machine_state.hpp"
#include "execute/ex_instruction.hpp"
#include "execute/ex_push.hpp"
#include "tokenize/tokenizer.hpp"
#include "assembler/assembler.hpp"
#include "assembler/label_substitute.hpp"
#include "assembler/label_extract.hpp"

#include <cstddef>

namespace ctai
{
    namespace details
    {
        template <typename, typename, typename>
        struct execute_impl;

        template <typename curr_machine_state, typename all_opcodes, size_t ...rest_of_instructions>
        struct execute_impl<curr_machine_state, all_opcodes, values_container<inst::to_size<inst::id_t::EXIT>, rest_of_instructions...>>
        {
            using regs_state = typename to_machine_state<curr_machine_state>::registers_state_t;
            static constexpr auto ret_val = get_reg<regs_state, regs::id_t::EAX>;
        };

        template <typename curr_machine_state, typename all_opcodes, typename opcodes = all_opcodes>
        struct execute_impl
        {
            using next_machine_state = execute_instruction<curr_machine_state, opcodes>;
            using next_opcodes = get_next_instruction<next_machine_state, all_opcodes>;
            static constexpr auto ret_val = execute_impl<next_machine_state, all_opcodes, next_opcodes>::ret_val;
        };
    }

    namespace details
    {
        template <typename code>
        struct prepare_and_execute
        {
            using tokens = tokenize<code>;
            using extract_labels_result = extract_labels<tokens>;
            using tokens_without_labels = typename extract_labels_result::tokens;
            using labels_metadata = typename extract_labels_result::labels;
            using tokens_after_labels_substitution = substutite_labels<tokens_without_labels, labels_metadata>;
            using opcodes = assemble<tokens_after_labels_substitution>;
            using machine_state = startup_machine_state;

            static constexpr auto ret_val = execute_impl<machine_state, opcodes>::ret_val;
        };
    }

    //todo change execute_code -> execute
    template <typename code>
    constexpr auto execute_code = details::prepare_and_execute<code>::ret_val;
}
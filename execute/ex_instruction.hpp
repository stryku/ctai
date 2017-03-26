#pragma once

#include "tuple.hpp"

#include <cstddef>

namespace cai {
    namespace execute {
        template<typename state, size_t ...opcodes>
        struct ex_instruction;
    }
}


#include "execute/ex_pop.hpp"
#include "execute/ex_push.hpp"
#include "execute/ex_add.hpp"
#include "execute/ex_sub.hpp"
#include "execute/ex_cmp.hpp"
#include "execute/ex_mov.hpp"
#include "execute/ex_mul.hpp"
#include "execute/ex_jmp.hpp"
#include "execute/ex_jge.hpp"

namespace cai
{
    namespace details
    {
        template <typename, typename>
        struct execute_instruction_impl;

        template <typename state, size_t ...opcodes>
        struct execute_instruction_impl<state, values_container<opcodes...>>
        {
            using next_machine_state = typename execute::ex_instruction<state, opcodes...>::next_machine_state;
        };
    }

    template <typename state, typename opcodes>
    using execute_instruction = typename details::execute_instruction_impl<state, opcodes>::next_machine_state;
}

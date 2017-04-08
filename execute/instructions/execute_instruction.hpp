#pragma once

#include "containers/values_container.hpp"

namespace ctai
{
    namespace execute2
    {
        template<typename thread_t, typename memory_t, auto ...opcodes>
        struct ex_instruction;
    }
}

#include "execute/instructions/ex_push.hpp"

namespace ctai
{
    namespace execute2
    {
        namespace details
        {
            template<typename thread_t,
                     typename memory_t,
                     typename opcodes>
            struct execute_instruction_impl;

            template <typename thread_t,
                      typename memory_t,
                      auto ...opcodes>
            struct execute_instruction_impl<thread_t,
                                            memory_t,
                                            values_container_n::values_container<opcodes...>>
            {
                using result = ex_instruction<thread_t,
                                              memory_t,
                                              opcodes...>;
            };
        }

        template<typename thread_t,
                 typename memory_t,
                 typename opcodes>
        using execute_instruction = typename details::execute_instruction_impl<thread_t,
                                                                               memory_t,
                                                                               opcodes>::result;
    }
}

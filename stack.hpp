#pragma once

#include <cstdint>
#include <stdlib.h>

namespace cai
{
    template <uint8_t top_val, uint8_t ...values>
    struct stack
    {
        static constexpr uint8_t top = top_val;
    };

    static_assert(stack<0, 1, 2>::top == 0, "");

    namespace details
    {
        template <typename v>
        struct to_stack_impl;

        template <uint8_t ...values>
        struct to_stack_impl<stack<values...>>
        {
            using type = stack<values...>;
        };
    }

    template <typename s>
    using to_stack = typename details::to_stack_impl<s>::type;

    namespace operations
    {
        namespace details
        {
            template <size_t count, uint8_t ...Args>
            struct pop_front_impl;

            template <size_t count>
            struct pop_front_impl<count>
            {
                using type = stack<0>;
            };

            template <uint8_t ...Args>
            struct pop_front_impl<0, Args...>
            {
                using type = stack<Args...>;
            };

            template <size_t count, uint8_t ...Args>
            struct pop_front_impl
            {
                using type = typename pop_front_impl<count - 1, Args...>::type;
            };
        }

        template <size_t s, typename v>
        struct stack_pop_impl;

        template <size_t pop_size, uint8_t ...values>
        struct stack_pop_impl<pop_size, stack<values...>>
        {
            using new_stack = typename details::pop_front_impl<pop_size,
                    values...>::type;
        };

        template <typename pop_dest_t, typename stack_t>
        struct stack_pop
        {
            using new_stack = typename stack_pop_impl<
                    sizeof(pop_dest_t),
                    to_stack<stack_t>
            >::new_stack;
        };

        static_assert(stack_pop<uint8_t, stack<0, 1>>::new_stack::top == 1, "");
    }
}

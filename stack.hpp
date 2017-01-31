#pragma once

#include <cstdint>
#include <stdlib.h>
#include <type_traits>

namespace cai
{
    template <uint8_t top_val, uint8_t ...values>
    struct stack
    {
        static constexpr uint8_t top = top_val;
    };

    using startup_stack = stack<0>;

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

    static_assert(std::is_same<stack<0,1>, to_stack<stack<0,1>>>::value, "");

    namespace operations
    {
        namespace details
        {
            template <size_t N>
            struct count_wrapper
            {
                static constexpr auto val = N;
                using dec = count_wrapper<N-1>;
            };

            template <size_t count, bool end, uint8_t ...values>
            struct pop_front_impl;

            template <size_t count, bool end>
            struct pop_front_impl<count, end>
            {
                using type = stack<0>;
            };

            template <size_t count, uint8_t top, uint8_t ...values>
            struct pop_front_impl<count, true, top, values...>
            {
                using type = stack<top, values...>;
            };

            template <size_t count, uint8_t top, uint8_t ...values>
            struct pop_front_impl<count,false,  top, values... >
            {
                using type = typename pop_front_impl<count-1, count == 1, values...>::type;
            };

            template <size_t s, typename v>
            struct stack_pop_impl;

            template <size_t pop_size, uint8_t ...values>
            struct stack_pop_impl<pop_size, stack<values...>>
            {
                static_assert(sizeof...(values) > pop_size, "");

                using new_stack = typename details::pop_front_impl<
                        pop_size,
                        false,
                        values...>::type;
            };
        }

        template <typename pop_dest_t, typename stack_t>
        struct stack_pop
        {
            using new_stack = typename details::stack_pop_impl<
                    sizeof(pop_dest_t),
                    to_stack<stack_t>
            >::new_stack;
        };

        static_assert(stack_pop<uint8_t,  stack<0, 1, 2, 3, 4, 5, 6>>::new_stack::top == 1, "");
        static_assert(stack_pop<uint16_t, stack<0, 1, 2, 3, 4, 5, 6>>::new_stack::top == 2, "");
        static_assert(stack_pop<uint32_t, stack<0, 1, 2, 3, 4, 5, 6>>::new_stack::top == 4, "");
    }
}

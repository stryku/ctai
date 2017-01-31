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

    //
    //stack pop implementation
    //

    namespace details
    {
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

            using new_stack = typename details::pop_front_impl<pop_size,
                                                               false,
                                                               values...>::type;
        };


    }

    template <typename pop_dest_t, typename stack_t>
    using stack_pop = typename details::stack_pop_impl<sizeof(pop_dest_t), stack_t>::new_stack;

    //
    //stack merge implementation
    //

    namespace details
    {
        template<typename l, typename r>
        struct stack_merge_impl;

        template<uint8_t ...lvalues, uint8_t ...rvalues>
        struct stack_merge_impl<stack<lvalues...>, stack<rvalues...>>
        {
            using new_stack = stack<lvalues..., rvalues...>;
        };
    }

    template<typename l, typename r>
    using stack_merge = typename details::stack_merge_impl<l, r>::new_stack;


    //
    //creating stack from integer value. usable to merge this stack later
    //

    template <typename val_type, uint32_t value>
    struct stack_from_value
    {
        using new_stack = typename std::conditional_t<sizeof(val_type) == 1,
                                                      stack<value & 0xff>,
                                                      typename std::conditional_t<sizeof(val_type) == 2,
                                                                                  stack<((value & 0xff00) >> 8), value & 0xff>,
                                                                                  stack<((value & 0xff000000) >> 24), ((value & 0xff0000) >> 16), ((value & 0xff00) >> 8), value & 0xff>>>;
    };


    //
    // stack push implementation
    //

    template <typename push_t, uint32_t push_v, typename stack_t>
    struct stack_push_impl
    {
        using new_stack = stack_merge<typename stack_from_value<push_t, push_v>::new_stack,
                                      stack_t>;
    };


    template <uint32_t push_v, typename stack_t>
    using stack_push_8 = typename stack_push_impl<uint8_t, push_v, stack_t>::new_stack;

    template <uint32_t push_v, typename stack_t>
    using stack_push_16 = typename stack_push_impl<uint16_t, push_v, stack_t>::new_stack;

    template <uint32_t push_v, typename stack_t>
    using stack_push_32 = typename stack_push_impl<uint32_t, push_v, stack_t>::new_stack;


    namespace tests
    {
        using test_stack = stack<0, 1, 2, 3, 4, 5, 6>;

        static_assert(stack_pop<uint8_t, test_stack>::top == 1, "");
        static_assert(stack_pop<uint16_t, test_stack>::top == 2, "");
        static_assert(stack_pop<uint32_t, test_stack>::top == 4, "");

        static_assert(stack_push_8<0xdd, test_stack>::top == 0xdd, "");
        static_assert(stack_push_16<0xccdd, test_stack>::top == 0xcc, "");
        static_assert(stack_push_32<0xaabbccdd, test_stack>::top == 0xaa, "");
    }
}

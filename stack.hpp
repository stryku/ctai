#pragma once

#include <cstdint>
#include <stdlib.h>
#include <type_traits>

namespace cai
{
    template <uint8_t ...values>
    struct stack
    {
        static constexpr auto size = sizeof...(values);
    };

    //using startup_stack = stack<0, 0, 0 ,0>;
    using startup_stack = stack<0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff>;

    namespace details
    {
        template <typename>
        struct to_stack_impl;


        template <uint8_t ...values>
        struct to_stack_impl<stack<values...>>
        {
            using type = stack<values...>;
        };
    }

    template <typename stack_t>
    using to_stack = typename details::to_stack_impl<stack_t>::type;
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
            using type = startup_stack;
        };

        template <size_t count, uint8_t top, uint8_t ...values>
        struct pop_front_impl<count, true, top, values...>
        {
            using type = stack<top, values...>;
        };

        template <size_t count, uint8_t top, uint8_t ...values>
        struct pop_front_impl<count, false,  top, values... >
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

    template <typename stack_t>
    using stack_pop_8 = stack_pop<uint8_t, stack_t>;

    template <typename stack_t>
    using stack_pop_16 = stack_pop<uint16_t, stack_t>;

    template <typename stack_t>
    using stack_pop_32 = stack_pop<uint32_t, stack_t>;


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



    namespace details
    {
        template <typename, uint32_t, typename>
        struct stack_push_impl;

        template<uint8_t val_to_push, uint8_t ...values>
        struct stack_push_impl<uint8_t, val_to_push, stack<values...>>
        {
            using type = stack_push_8<val_to_push, stack<values...>>;
        };

        template<uint16_t val_to_push, uint8_t ...values>
        struct stack_push_impl<uint16_t, val_to_push, stack<values...>>
        {
            using type = stack_push_16<val_to_push, stack<values...>>;
        };

        template<uint32_t val_to_push, uint8_t ...values>
        struct stack_push_impl<uint32_t, val_to_push, stack<values...>>
        {
            using type = stack_push_32<val_to_push, stack<values...>>;
        };
    }

    template <typename size_type, uint32_t val, typename stack_t>
    using stack_push = typename details::stack_push_impl<size_type, val, stack_t>::type;

    //
    // get stack top
    //

    namespace details
    {
        template<typename>
        struct top_getter_8;

        template<typename>
        struct top_getter_16;

        template<typename>
        struct top_getter_32;

        template<uint8_t top, uint8_t ...values>
        struct top_getter_8<stack<top, values...>>
        {
            static constexpr uint8_t value = top;
        };

        template<uint8_t top_1, uint8_t top_0, uint8_t ...values>
        struct top_getter_16<stack<top_1, top_0, values...>>
        {
            static constexpr uint16_t value = (static_cast<uint16_t>(top_1) << 8) + static_cast<uint16_t>(top_0);
        };

        template<uint8_t top_3, uint8_t top_2, uint8_t top_1, uint8_t top_0, uint8_t ...values>
        struct top_getter_32<stack<top_3, top_2, top_1, top_0, values...>>
        {
            static constexpr uint32_t value = (static_cast<uint16_t>(top_3) << 24) + (static_cast<uint16_t>(top_2) << 16) + (static_cast<uint16_t>(top_1) << 8) + static_cast<uint16_t>(top_0);
        };
    }

    template <typename stack_t>
    constexpr uint8_t stack_top_8 = details::top_getter_8<stack_t>::value;

    template <typename stack_t>
    constexpr uint16_t stack_top_16 = details::top_getter_16<stack_t>::value;

    template <typename stack_t>
    constexpr uint32_t stack_top_32 = details::top_getter_32<stack_t>::value;



    namespace details
    {
        template <typename, typename>
        struct stack_top_impl;

        template<uint8_t ...values>
        struct stack_top_impl<uint8_t, stack<values...>>
        {
            static constexpr uint8_t val = stack_top_8<stack<values...>>;
        };

        template<uint8_t ...values>
        struct stack_top_impl<uint16_t, stack<values...>>
        {
            static constexpr uint16_t val = stack_top_16<stack<values...>>;
        };

        template<uint8_t ...values>
        struct stack_top_impl<uint32_t, stack<values...>>
        {
            static constexpr uint32_t val = stack_top_32<stack<values...>>;
        };
    }

    template <typename size_type, typename stack_t>
    constexpr auto stack_top = details::stack_top_impl<size_type, stack_t>::val;

    //
    // stack_take
    //

    namespace details
    {
        template <size_t, bool, typename, typename>
        struct stack_take_impl;

        template <uint8_t ...taken, uint8_t ...left>
        struct stack_take_impl<0, true, stack<taken...>, stack<left...>>
        {
            using type = stack_take_impl;

            using taken_stack = stack<taken...>;
            using left_stack = stack<left...>;
        };

        template <size_t count, uint8_t to_take, uint8_t ...taken, uint8_t ...left>
        struct stack_take_impl<count, false, stack<taken...>, stack<to_take, left...>>
        {
            using type = stack_take_impl<count-1,
                                         count == 1,
                                         stack<taken..., to_take>,
                                         stack<left...>>;

            using taken_stack = typename type::taken_stack;
            using left_stack = typename type::left_stack;
        };
    }

    template <size_t count, typename stack_t>
    using stack_take = typename details::stack_take_impl<count, count==0, stack<>, stack_t>::type;

    //
    // stack_set
    //

    namespace details
    {
        template <typename, uint32_t, size_t, typename>
        struct stack_set_impl;

        template <uint32_t val_to_set, size_t ptr, uint8_t ...values>
        struct stack_set_impl<uint8_t, val_to_set, ptr, stack<values...>>
        {
            using stack_t = stack<values...>;
            static constexpr auto to_take = stack_t::size - ptr - 1;

            using stack_take_t = stack_take<to_take, stack_t>;
            using stack_after_pop = stack_pop_8<typename stack_take_t::type::left_stack>;

            using stack_after_push = stack_push_8<static_cast<uint8_t>(val_to_set), stack_after_pop>;
            using result_stack = stack_merge<typename stack_take_t::taken_stack, stack_after_push>;

        };

        template <uint32_t val_to_set, size_t ptr, uint8_t ...values>
        struct stack_set_impl<uint16_t, val_to_set, ptr, stack<values...>>
        {
            using stack_t = stack<values...>;
            static constexpr auto to_take = stack_t::size - ptr - 1;

            using stack_take_t = stack_take<to_take, stack_t>;
            using stack_after_pop = stack_pop_16<typename stack_take_t::type::left_stack>;
            using stack_after_push = stack_push_16<static_cast<uint16_t>(val_to_set), stack_after_pop>;
            using result_stack = stack_merge<typename stack_take_t::taken_stack, stack_after_push>;
        };

        template <uint32_t val_to_set, size_t ptr, uint8_t ...values>
        struct stack_set_impl<uint32_t, val_to_set, ptr, stack<values...>>
        {
            using stack_t = stack<values...>;
            static constexpr auto to_take = stack_t::size - ptr - 1;

            using stack_take_t = stack_take<to_take, stack_t>;
            using stack_after_pop = stack_pop_32<typename stack_take_t::type::left_stack>;
            using stack_after_push = stack_push_32<val_to_set, stack_after_pop>;
            using result_stack = stack_merge<typename stack_take_t::taken_stack, stack_after_push>;
        };
    }

    //WARNING: you can't set value at 0 ptr
    template <typename val_size_type, uint32_t val, size_t ptr, typename stack_t>
    using stack_set = typename details::stack_set_impl<val_size_type, val, ptr, stack_t>::result_stack;


    //
    // stack get
    //

    namespace details
    {
        template <typename, size_t, typename>
        struct stack_get_impl;

        template <size_t ptr, uint8_t ...values>
        struct stack_get_impl<uint8_t, ptr, stack<values...>>
        {
            using stack_t = stack<values...>;
            static constexpr auto to_take = stack_t::size - ptr - 1;

            using stack_take_t = stack_take<to_take, stack_t>;
            static constexpr auto value = stack_top_8<typename stack_take_t::type::left_stack>;

        };

        template <size_t ptr, uint8_t ...values>
        struct stack_get_impl<uint16_t, ptr, stack<values...>>
        {
            using stack_t = stack<values...>;
            static constexpr auto to_take = stack_t::size - ptr - 1;

            using stack_take_t = stack_take<to_take, stack_t>;
            static constexpr auto value = stack_top_16<typename stack_take_t::type::left_stack>;
        };

        template <size_t ptr, uint8_t ...values>
        struct stack_get_impl<uint32_t, ptr, stack<values...>>
        {
            using stack_t = stack<values...>;
            static constexpr auto to_take = stack_t::size - ptr - 1;

            using stack_take_t = stack_take<to_take, stack_t>;
            static constexpr auto value = stack_top_32<typename stack_take_t::type::left_stack>;
        };
    }

    //WARNING: you can't set value at 0 ptr
    template <typename val_size_type, size_t ptr, typename stack_t>
    constexpr auto stack_get = details::stack_get_impl<val_size_type, ptr, stack_t>::value;

    //
    // basic tests
    //

    namespace tests
    {
        using test_stack = stack<0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77>;

        static_assert(stack_top_8<test_stack> == 0x00, "");
        static_assert(stack_top_16<test_stack> == 0x0011, "");
        static_assert(stack_top_32<test_stack> == 0x00112233, "");

        static_assert(stack_top_8<stack_pop_8<test_stack>> == 0x11, "");
        static_assert(stack_top_16<stack_pop_16<test_stack>> == 0x2233, "");
        static_assert(stack_top_32<stack_pop_32<test_stack>> == 0x44556677, "");

        static_assert(stack_top_8<stack_push_8<0xaa, test_stack>> == 0xaa, "");
        static_assert(stack_top_16<stack_push_16<0xaabb, test_stack>> == 0xaabb, "");
        static_assert(stack_top_32<stack_push_32<0xaabbccdd, test_stack>> == 0xaabbccdd, "");

        static_assert(
                std::is_same<stack_take<2, test_stack>::type::left_stack,
                             stack<0x22, 0x33, 0x44, 0x55, 0x66, 0x77>>::value
                , "");

        static_assert(
                std::is_same<stack_take<2, test_stack>::type::taken_stack ,
                             stack<0x00, 0x11>>::value
                , "");

        static_assert(
                std::is_same<stack_take<0, test_stack>::type::left_stack,
                        test_stack>::value
                , "");

        static_assert(
                std::is_same<stack_set<uint8_t, 0xff, 1, test_stack>,
                        stack<0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0xff, 0x77>>::value
                , "");

    }
}

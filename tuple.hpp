#pragma once

namespace cai
{
    template <typename ...args>
    struct tuple
    {};

    //
    //tuple_append
    //
    namespace details
    {
        template <typename, typename>
        struct tuple_append_impl;

        template <typename ...args, typename tail>
        struct tuple_append_impl<tuple<args...>, tail>
        {
            using type = tuple<args..., tail>;
        };
    }

    template <typename tup, typename tail>
    using tuple_append = typename details::tuple_append_impl<tup, tail>::type;

    //
    // tuple_merge
    //
    namespace details
    {
        template <typename, typename>
        struct tuple_merge_impl;

        template <typename ...ltypes, typename ...rtypes>
        struct tuple_merge_impl<tuple<ltypes...>, tuple<rtypes...>>
        {
            using type = tuple<ltypes..., rtypes...>;
        };
    }

    template <typename l, typename r>
    using tuple_merge = typename details::tuple_merge_impl<l, r>::type;


    //
    // values_container
    //

    template <size_t ...values>
    struct values_container
    {};

    //
    //values_append
    //
    namespace details
    {
        template <typename, size_t>
        struct values_append_impl;

        template <size_t ...args, size_t tail>
        struct values_append_impl<values_container<args...>, tail>
        {
            using type = values_container<args..., tail>;
        };
    }

    template <typename tup, size_t tail>
    using values_append = typename details::values_append_impl<tup, tail>::type;

    //
    //values_merge
    //
    namespace details
    {
        template <typename, typename>
        struct values_merge_impl;

        template <size_t ...ltypes, size_t ...rtypes>
        struct values_merge_impl<values_container<ltypes...>, values_container<rtypes...>>
        {
            using type = values_container<ltypes..., rtypes...>;
        };
    }

    template <typename l, typename r>
    using values_merge = typename details::values_merge_impl<l, r>::type;
}
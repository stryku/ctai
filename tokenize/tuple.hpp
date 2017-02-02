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
}
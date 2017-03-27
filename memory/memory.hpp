#pragma once

#include <cstdint>
#include <cstdlib>
#include <regex>

namespace ctai
{
    template <uint8_t ...values>
    struct memory
    {
        static constexpr auto size = sizeof...(values);
    };

    namespace details
    {
        template <typename Is>
        struct create_memory_impl;

        template <size_t ...Ints>
        struct create_memory_impl<std::index_sequence<Ints...>>
        {
            using memory_t = memory<static_cast<uint8_t>(Ints)...>;
        };
    }

    template <size_t size>
    using create_memory = details::create_memory_impl<std::make_index_sequence<size>>::memory_t;
}

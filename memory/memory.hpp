#pragma once

#include <cstdint>
#include <cstddef>
#include "containers/values_container.hpp"

namespace ctai
{
    namespace memory
    {
        template <uint8_t ...values>
        using memory_block = values_container_n::values_container<values...>;

        template <size_t size>
        struct memory_metadata
        {
            using bytes_metadata = values_container_n::create<bool, size>;
        };

        template <size_t size>
        struct memory
        {
            using mem = values_container_n::create<uint8_t, size>;
            using metadata_t = memory_metadata<size>;
        };

        template <size_t size>
        using memory_create = memory<size>;


        //
        //get_8
        //
        template <typename memory_t, size_t ptr>
        constexpr auto get_8 = values_container_n::get_1<typename memory_t::mem, ptr>;

        //
        //get_16
        //
        namespace details
        {
            template <typename memory_t, size_t ptr>
            struct get_16_impl
            {
                using bytes = values_container_n::get_2<typename memory_t::mem, ptr>;
                static constexpr auto result = values_container_n::bytes_to_value_2<bytes>;
            };
        }

        template <typename memory_t, size_t ptr>
        constexpr auto get_16_impl = details::get_16_impl<memory_t, ptr>::result;

        //
        //get_32
        //
        namespace details
        {
            template <typename memory_t, size_t ptr>
            struct get_32_impl
            {
                using bytes = values_container_n::get_4<typename memory_t::mem, ptr>;
                static constexpr auto result = values_container_n::bytes_to_value_4<bytes>;
            };
        }

        template <typename memory_t, size_t ptr>
        constexpr auto get_32 = details::get_32_impl<memory_t, ptr>::result;
    }
}

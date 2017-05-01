#pragma once

#include "containers/values_container.hpp"
#include "memory/memory_metadata.hpp"
#include "values/values.hpp"

#include <cstdint>
#include <cstddef>

namespace ctai
{
    namespace memory
    {
        template <uint8_t ...values>
        using memory_block = values_container_n::values_container<values...>;

        //template <size_t size>
      //  struct memory_metadata
      //  {
      //      using bytes_metadata = values_container_n::create<bool, size>;
      //  };

        template <size_t size_v,
                  typename memory_block_type = values_container_n::create<uint8_t, size_v>,
                  typename metadata_type = metadata::create<size_v>>
        struct memory
        {
            static constexpr auto size = size_v;
            using memory_block_t = memory_block_type;
            using metadata_t = metadata_type;
        };

        template <size_t size>
        using memory_create = memory<size>;

        template <typename memory_t, typename memory_block>
        using set_memory_block = memory<memory_t::size,
                                        memory_block,
                                        typename memory_t::metadata_t>;

        template <typename memory_t, typename metadata_t>
        using set_metadata = memory<memory_t::size,
                                        typename memory_t::memory_block_t,
                                        metadata_t>;

        //
        //get_8
        //
        template <typename memory_t, size_t ptr>
        constexpr auto get_8 = values_container_n::get_1<typename memory_t::memory_block_t, ptr>;

        //
        //get_16
        //
        namespace details
        {
            template <typename memory_t, size_t ptr>
            struct get_16_impl
            {
                using bytes = values_container_n::get_2<typename memory_t::memory_block_t, ptr>;
                static constexpr auto result = values_container_n::bytes_to_value_2<bytes>;
            };
        }

        template <typename memory_t, size_t ptr>
        constexpr auto get_16 = details::get_16_impl<memory_t, ptr>::result;

        //
        //get_32
        //
        namespace details
        {
            template <typename memory_t, size_t ptr>
            struct get_32_impl
            {
                using bytes = values_container_n::get_4<typename memory_t::memory_block_t, ptr>;
                static constexpr auto result = values_container_n::bytes_to_value_4<bytes>;
            };
        }

        template <typename memory_t, size_t ptr>
        constexpr auto get_32 = details::get_32_impl<memory_t, ptr>::result;


        //
        //get
        //
        namespace details
        {
            template <typename memory_t, typename get_type_size, size_t ptr>
            struct get_impl;

            template <typename memory_t, size_t ptr>
            struct get_impl<memory_t, uint8_t, ptr>
            {
                static constexpr auto result = get_8<memory_t, ptr>;
            };

            template <typename memory_t, size_t ptr>
            struct get_impl<memory_t, uint16_t, ptr>
            {
                static constexpr auto result = get_16<memory_t, ptr>;
            };

            template <typename memory_t, size_t ptr>
            struct get_impl<memory_t, uint32_t, ptr>
            {
                static constexpr auto result = get_32<memory_t, ptr>;
            };
        }

        template <typename memory_t, typename get_type_size, size_t ptr>
        constexpr auto get = details::get_impl<memory_t, get_type_size, ptr>::result;


        namespace details
        {
            template <typename memory_t, size_t ptr, uint32_t value, typename value_type>
            struct set_impl
            {
                using splitted_value = values::split_to_byte_values_container<static_cast<value_type>(value)>;
                using new_block = values_container_n::set_from_container<typename memory_t::memory_block_t, ptr, splitted_value>;

                using result = memory<memory_t::size,
                                      new_block,
                                      typename memory_t::metadata_t>;
            };
        }

        template <typename memory_t, size_t ptr, uint32_t value>
        using set_8 = typename details::set_impl<memory_t, ptr, value, uint8_t>::result;

        template <typename memory_t, size_t ptr, uint32_t value>
        using set_16 = typename details::set_impl<memory_t, ptr, value, uint16_t>::result;

        template <typename memory_t, size_t ptr, uint32_t value>
        using set_32 = typename details::set_impl<memory_t, ptr, value, uint32_t>::result;

        template <typename memory_t, typename get_type_size, size_t ptr, auto value>
        using set = typename details::set_impl<memory_t, ptr, value, get_type_size>::result;
    }
}

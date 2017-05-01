#pragma once

#include "memory/memory.hpp"
#include "memory/memory_metadata.hpp"
#include "ctai/traits.hpp"

#include <cstddef>

namespace ctai
{
    namespace memory
    {
        namespace details
        {
            template <typename memory_t, size_t ptr, size_t size>
            struct reserve_block
            {
                using result_metadata = metadata::reserve_block<typename memory_t::metadata_t, ptr, size>;
                using result_memory = ctai::memory::set_metadata<memory_t,
                                                                 result_metadata>;
            };

            template <typename memory_t, size_t size>
            struct reserve_block<memory_t, utils::bad_value, size>
            {
                using result_memory = memory_t;
            };

            template<typename memory_t, size_t size>
            struct alloc_impl
            {
                static constexpr auto found_block_ptr = metadata::find_unused_block_of_size<typename memory_t::metadata_t, size>;

                using result_memory = typename reserve_block<memory_t,
                                                             found_block_ptr,
                                                             size>::result_memory;

                static constexpr auto result_ptr = found_block_ptr != utils::bad_value ? found_block_ptr
                                                                                       : 0;
            };
        }

        template <typename memory_t, size_t size>
        using alloc = typename details::alloc_impl<memory_t, size>;
    }
}

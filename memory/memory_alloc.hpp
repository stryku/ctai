#pragma once

#include "memory/memory.hpp"
#include "memory/memory_metadata.hpp"
#include "traits.hpp"

#include <cstddef>

namespace ctai
{
    namespace memory
    {
        namespace details
        {
            template <typename metadata_t, size_t ptr, size_t size>
            struct reserve_block
            {
                using result = metadata::reserve_block<metadata_t, ptr, size>;
            };

            template <typename metadata_t, size_t size>
            struct reserve_block<metadata_t, utils::bad_value, size>
            {
                using result = metadata_t;
            };

            template<typename metadata_t, size_t size>
            struct alloc_impl
            {
                static constexpr auto found_block_ptr = metadata::find_unused_block_of_size<metadata_t, size>;

                using result_metadata = typename reserve_block<metadata_t, found_block_ptr, size>::result;

                static constexpr auto result_ptr = found_block_ptr != utils::bad_value ? found_block_ptr
                                                                                       : 0;
            };
        }

        template <typename metadata_t, size_t size>
        using alloc = details::alloc_impl<metadata_t, size>;
    }
}

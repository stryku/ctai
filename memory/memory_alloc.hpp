#pragma once

#include "memory/memory.hpp"
#include "memory/memory_metadata.hpp"

#include <cstddef>

namespace ctai
{
    namespace memory
    {
        namespace details
        {
            template<typename metadata_t, size_t size>
            struct memory_alloc_impl
            {
                static constexpr auto found_block_ptr = metadata::find_unused_block_of_size<metadata_t, size>;

                using result_metadata = std::conditional_t<found_block_ptr == utils::bad_value,
                                                           metadata_t,
                                                           metadata::reserve_block<metadata_t, found_block_ptr, size>>;

                static constexpr auto result_ptr = found_block_ptr != utils::bad_value ? found_block_ptr
                                                                                       : 0;
            };
        }
    }
}

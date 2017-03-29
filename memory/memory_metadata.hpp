#pragma once

#include "containers/values_container.hpp"
#include "tuple/tuple.hpp"

#include <cstddef>

namespace ctai
{
    namespace memory
    {
        namespace metadata
        {
            template<bool ...values>
            using memory_used_bytes = values_container_n::values_container<values...>;

            template<size_t ptr_v, size_t size_v>
            struct allocated_block
            {
                constexpr auto ptr = ptr_v;
                constexpr auto size = size_v;
            };

            template<typename ...blocks>
            using allocated_blocks = tuple<blocks...>;

            template<typename used_bytes, typename allocated_blocks>
            struct memory_metadata
            {};

            //
            //find_allocated_block
            //
            namespace details
            {
                template <size_t desired_ptr>
                struct find_allocated_block_predicate
                {
                    template <typename block>
                    static constexpr auto value = (block::ptr == desired_ptr);
                };

                template <typename metadata, size_t ptr>
                struct find_allocated_block_impl;

                template <typename used_bytes,
                          typename allocated_blocks,
                          size_t ptr>
                struct find_allocated_block_impl<memory_metadata<used_bytes,
                                                                 allocated_blocks>
                                                 ptr>
                {
                    using result = tuple::find_if<allocated_blocks,
                                                  find_allocated_block_predicate<ptr>>;
                };
            }

            template <typename metadata, size_t ptr>
            using find_allocated_block = typename details::find_allocated_block_impl<metadata, ptr>::result;
        }
    }
}

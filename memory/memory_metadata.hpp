#pragma once

#include "containers/values_container.hpp"
#include "utils/bad_value.hpp"
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
                static constexpr auto ptr = ptr_v;
                static constexpr auto size = size_v;
            };

            template<typename ...blocks>
            using allocated_blocks = tuple_n::tuple<blocks...>;

            template<typename used_bytes, typename allocated_blocks>
            struct memory_metadata
            {};

            template <size_t size>
            using create = memory_metadata<values_container_n::create_with_val<size, false>,
                                           tuple_n::tuple<>>;


            namespace details
            {
                template <size_t desired_ptr>
                struct find_allocated_block_predicate
                {
                    template <typename block>
                    static constexpr auto value = (block::ptr == desired_ptr);
                };
            }

            //
            //find_allocated_block
            //
            namespace details
            {
                template <typename metadata, size_t ptr>
                struct find_allocated_block_impl;

                template <typename used_bytes,
                          typename allocated_blocks,
                          size_t ptr>
                struct find_allocated_block_impl<memory_metadata<used_bytes,
                                                                 allocated_blocks>,
                                                 ptr>
                {
                    using result = tuple_n::find_if<allocated_blocks,
                                                    find_allocated_block_predicate<ptr>>;
                };
            }

            template <typename metadata, size_t ptr>
            using find_allocated_block = typename details::find_allocated_block_impl<metadata, ptr>::result;

            //
            //reserve_block
            //
            namespace details
            {
                template <typename metadata, size_t ptr, size_t size>
                struct reserve_block_impl;

                template <typename used_bytes,
                          typename allocated_blocks,
                          size_t ptr,
                          size_t size>
                struct reserve_block_impl<memory_metadata<used_bytes,
                                                          allocated_blocks>,
                                          ptr,
                                          size>
                {
                    using new_used_bytes = values_container_n::set_val<used_bytes, ptr,size, true>;
                    using new_allocated_blocks = std::conditional_t<(size > 0),
                                                                    tuple_n::append<allocated_blocks, allocated_block<ptr, size>>,
                                                                    allocated_blocks>;

                    using result = memory_metadata<new_used_bytes, new_allocated_blocks>;
                };
            }

            template <typename metadata, size_t ptr, size_t size>
            using reserve_block = typename details::reserve_block_impl<metadata, ptr, size>::result;

            //
            //free_block
            //
            namespace details
            {
                template <typename used_bytes, typename allocated_blocks, size_t iterator>
                struct free_and_erase_block
                {
                    using block_metadata = tuple_n::get<allocated_blocks, iterator>;
                    using new_used_bytes = values_container_n::set_val<used_bytes,
                                                                       block_metadata::ptr,
                                                                       block_metadata::size,
                                                                       false>;
                    using new_allocated_blocks = tuple_n::erase<allocated_blocks, iterator>;

                    using result = memory_metadata<new_used_bytes, new_allocated_blocks>;
                };

                template <typename used_bytes, typename allocated_blocks>
                struct free_and_erase_block<used_bytes, allocated_blocks, utils::bad_value>
                {
                    using result = memory_metadata<used_bytes, allocated_blocks>;
                };


                template <typename metadata, size_t ptr>
                struct free_block_impl;

                template <typename used_bytes,
                          typename allocated_blocks,
                          size_t ptr>
                struct free_block_impl<memory_metadata<used_bytes,
                                                       allocated_blocks>,
                                       ptr>
                {
                    static constexpr auto found_it = tuple_n::find_if_it<allocated_blocks, find_allocated_block_predicate<ptr>>;

                    using result = std::conditional_t<found_it == utils::bad_value,
                                                      memory_metadata<used_bytes, allocated_blocks>,
                                                      typename free_and_erase_block<used_bytes, allocated_blocks, found_it>::result>;
                };
            }

            template <typename metadata, size_t ptr>
            using free_block = typename details::free_block_impl<metadata, ptr>::result;
        }
    }
}

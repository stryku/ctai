#pragma once

#include "memory/memory_metadata.hpp"
#include "containers/values_container.hpp"
#include "tuple/tuple.hpp"

#include "tests/tests_macros.hpp"

namespace ctai::tests::memory_metadata
{
    namespace test_create
    {
        namespace create_empty
        {
            using expected = ctai::memory::metadata::memory_metadata<ctai::values_container_n::values_container<>,
                                                                     ctai::tuple_n::tuple<>>;
            using test_result = ctai::memory::metadata::create<0>;
            ASSERT_EQ_T(expected, test_result);
        }

        namespace create_one_size
        {
            using expected = ctai::memory::metadata::memory_metadata<ctai::values_container_n::values_container<false>,
                                                                     ctai::tuple_n::tuple<>>;
            using test_result = ctai::memory::metadata::create<1>;
            ASSERT_EQ_T(expected, test_result);
        }

        namespace create_some_size
        {
            using expected = ctai::memory::metadata::memory_metadata<ctai::values_container_n::values_container<false, false, false>,
                                                                     ctai::tuple_n::tuple<>>;
            using test_result = ctai::memory::metadata::create<3>;
            ASSERT_EQ_T(expected, test_result);
        }
    }

    namespace test_reserve_block
    {
        using test_mem_metadata = ctai::memory::metadata::create<5>;

        namespace reserve_block_zero_size
        {
            using expected = ctai::memory::metadata::memory_metadata<ctai::values_container_n::values_container<false, false, false, false, false>,
                                                                     ctai::tuple_n::tuple<>>;
            using test_result = ctai::memory::metadata::reserve_block<test_mem_metadata, 0, 0>;
            ASSERT_EQ_T(expected, test_result);
        }

        namespace reserve_block_one_size
        {
            namespace t1
            {
                using expected_used_bytes = ctai::values_container_n::values_container<true, false, false, false, false>;
                using expected_allocated_blocks = ctai::tuple_n::tuple<ctai::memory::metadata::allocated_block<0, 1>>;
                using expected = ctai::memory::metadata::memory_metadata<expected_used_bytes,
                                                                         expected_allocated_blocks>;

                using test_result = ctai::memory::metadata::reserve_block<test_mem_metadata, 0, 1>;

                ASSERT_EQ_T(expected, test_result);
            }

            namespace t2
            {
                using expected_used_bytes = ctai::values_container_n::values_container<false, true, false, false, false>;
                using expected_allocated_blocks = ctai::tuple_n::tuple<ctai::memory::metadata::allocated_block<1, 1>>;
                using expected = ctai::memory::metadata::memory_metadata<expected_used_bytes,
                                                                         expected_allocated_blocks>;

                using test_result = ctai::memory::metadata::reserve_block<test_mem_metadata, 1, 1>;

                ASSERT_EQ_T(expected, test_result);
            }

            namespace t3
            {
                using expected_used_bytes = ctai::values_container_n::values_container<false, false, false, false, true>;
                using expected_allocated_blocks = ctai::tuple_n::tuple<ctai::memory::metadata::allocated_block<4, 1>>;
                using expected = ctai::memory::metadata::memory_metadata<expected_used_bytes,
                                                                         expected_allocated_blocks>;

                using test_result = ctai::memory::metadata::reserve_block<test_mem_metadata, 4, 1>;

                ASSERT_EQ_T(expected, test_result);
            }
        }

        namespace reserve_block_some_size
        {
            namespace t1
            {
                using expected_used_bytes = ctai::values_container_n::values_container<true, true, true, false, false>;
                using expected_allocated_blocks = ctai::tuple_n::tuple<ctai::memory::metadata::allocated_block<0, 3>>;
                using expected = ctai::memory::metadata::memory_metadata<expected_used_bytes,
                                                                         expected_allocated_blocks>;

                using test_result = ctai::memory::metadata::reserve_block<test_mem_metadata, 0, 3>;

                ASSERT_EQ_T(expected, test_result);
            }

            namespace t2
            {
                using expected_used_bytes = ctai::values_container_n::values_container<false, true, true, true, false>;
                using expected_allocated_blocks = ctai::tuple_n::tuple<ctai::memory::metadata::allocated_block<1, 3>>;
                using expected = ctai::memory::metadata::memory_metadata<expected_used_bytes,
                                                                         expected_allocated_blocks>;

                using test_result = ctai::memory::metadata::reserve_block<test_mem_metadata, 1, 3>;

                ASSERT_EQ_T(expected, test_result);
            }

            namespace t3
            {
                using expected_used_bytes = ctai::values_container_n::values_container<false, false, true, true, true>;
                using expected_allocated_blocks = ctai::tuple_n::tuple<ctai::memory::metadata::allocated_block<2, 3>>;
                using expected = ctai::memory::metadata::memory_metadata<expected_used_bytes,
                                                                         expected_allocated_blocks>;

                using test_result = ctai::memory::metadata::reserve_block<test_mem_metadata, 2, 3>;

                ASSERT_EQ_T(expected, test_result);
            }
        }
    }

    namespace tests_free_block
    {
        namespace free_block_not_allocated
        {
            namespace empty
            {
                using start_metadata = ctai::memory::metadata::memory_metadata<ctai::values_container_n::values_container<false, false, false>,
                                                                               ctai::tuple_n::tuple<>>;
                using expected = start_metadata;
                using test_result = ctai::memory::metadata::free_block<start_metadata, 0>;

                ASSERT_EQ_T(test_result, expected);
            }

            namespace not_empty
            {
                using start_metadata = ctai::memory::metadata::memory_metadata<ctai::values_container_n::values_container<false, true, true>,
                                                                               ctai::tuple_n::tuple<ctai::memory::metadata::allocated_block<1, 2>>>;
                using expected = start_metadata;
                using test_result = ctai::memory::metadata::free_block<start_metadata, 0>;

                ASSERT_EQ_T(test_result, expected);
            }
        }

        namespace free_block_allocated
        {
            namespace one_allocated
            {
                namespace t1
                {
                    using start_metadata = ctai::memory::metadata::memory_metadata<ctai::values_container_n::values_container<true, false, false>,
                                                                                   ctai::tuple_n::tuple<ctai::memory::metadata::allocated_block<0, 1>>>;
                    using expected = ctai::memory::metadata::memory_metadata<ctai::values_container_n::values_container<false, false, false>,
                                                                             ctai::tuple_n::tuple<>>;

                    using test_result = ctai::memory::metadata::free_block<start_metadata, 0>;

                    ASSERT_EQ_T(test_result, expected);
                }

                namespace t2
                {
                    using start_metadata = ctai::memory::metadata::memory_metadata<ctai::values_container_n::values_container<false, true, false>,
                                                                                   ctai::tuple_n::tuple<ctai::memory::metadata::allocated_block<1, 1>>>;
                    using expected = ctai::memory::metadata::memory_metadata<ctai::values_container_n::values_container<false, false, false>,
                                                                             ctai::tuple_n::tuple<>>;

                    using test_result = ctai::memory::metadata::free_block<start_metadata, 1>;

                    ASSERT_EQ_T(test_result, expected);
                }
            }

            namespace some_allocated
            {
                using start_metadata = ctai::memory::metadata::memory_metadata<ctai::values_container_n::values_container<true, true, false>,
                                                                               ctai::tuple_n::tuple<ctai::memory::metadata::allocated_block<0, 1>,
                                                                                                    ctai::memory::metadata::allocated_block<1, 1>>>;

                using expected = ctai::memory::metadata::memory_metadata<ctai::values_container_n::values_container<false, true, false>,
                                                                         ctai::tuple_n::tuple<ctai::memory::metadata::allocated_block<1, 1>>>;
                using test_result = ctai::memory::metadata::free_block<start_metadata, 0>;

                ASSERT_EQ_T(test_result, expected);
            }
        }
    }
}

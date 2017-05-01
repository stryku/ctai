#pragma once

#include "tests/tests_macros.hpp"
#include "memory/memory_metadata.hpp"
#include "memory/memory_alloc.hpp"

namespace ctai::tests::memory
{
    template<auto ...values>
    using vc = ctai::values_container_n::values_container<values...>;

    using test_mem_block = vc<0,1,2,3,4,5,6,7,8,9>;
    using test_memory = ctai::memory::memory<10, test_mem_block>;


    namespace test_set
    {
        namespace test_set_8
        {
            namespace t1
            {
                using expected_mem_block = vc<64,1,2,3,4,5,6,7,8,9>;
                using result_mem_block = typename ctai::memory::set_8<test_memory, 0, 64>::memory_block_t;

                using casted_expected = ctai::values_container_n::cast_values_to<expected_mem_block, size_t>;
                using casted_result = ctai::values_container_n::cast_values_to<result_mem_block, size_t>;

                ASSERT_EQ_T(casted_expected, casted_result);
            }

            namespace t2
            {
                using expected_mem_block = vc<0,1,2,64,4,5,6,7,8,9>;
                using result_mem_block = typename ctai::memory::set_8<test_memory, 3, 64>::memory_block_t;

                using casted_expected = ctai::values_container_n::cast_values_to<expected_mem_block, size_t>;
                using casted_result = ctai::values_container_n::cast_values_to<result_mem_block, size_t>;

                ASSERT_EQ_T(casted_expected, casted_result);
            }

            namespace t3
            {
                using expected_mem_block = vc<0,1,2,3,4,5,6,7,8,64>;
                using result_mem_block = typename ctai::memory::set_8<test_memory, 9, 64>::memory_block_t;

                using casted_expected = ctai::values_container_n::cast_values_to<expected_mem_block, size_t>;
                using casted_result = ctai::values_container_n::cast_values_to<result_mem_block, size_t>;

                ASSERT_EQ_T(casted_expected, casted_result);
            }
        }

        namespace test_set_16
        {
            namespace t1
            {
                using expected_mem_block = vc<0x10,0x11,2,3,4,5,6,7,8,9>;
                using result_mem_block = typename ctai::memory::set_16<test_memory, 0, 0x1011>::memory_block_t;

                using casted_expected = ctai::values_container_n::cast_values_to<expected_mem_block, size_t>;
                using casted_result = ctai::values_container_n::cast_values_to<result_mem_block, size_t>;

                ASSERT_EQ_T(casted_expected, casted_result);
            }

            namespace t2
            {
                using expected_mem_block = vc<0,1,2,0x10,0x11,5,6,7,8,9>;
                using result_mem_block = typename ctai::memory::set_16<test_memory, 3, 0x1011>::memory_block_t;

                using casted_expected = ctai::values_container_n::cast_values_to<expected_mem_block, size_t>;
                using casted_result = ctai::values_container_n::cast_values_to<result_mem_block, size_t>;

                ASSERT_EQ_T(casted_expected, casted_result);
            }

            namespace t3
            {
                using expected_mem_block = vc<0,1,2,3,4,5,6,7,0x10,0x11>;
                using result_mem_block = typename ctai::memory::set_16<test_memory, 8, 0x1011>::memory_block_t;

                using casted_expected = ctai::values_container_n::cast_values_to<expected_mem_block, size_t>;
                using casted_result = ctai::values_container_n::cast_values_to<result_mem_block, size_t>;

                ASSERT_EQ_T(casted_expected, casted_result);
            }
        }

        namespace test_set_32
        {
            namespace t1
            {
                using expected_mem_block = vc<0x10,0x11, 0x12,0x13,4,5,6,7,8,9>;
                using result_mem_block = typename ctai::memory::set_32<test_memory, 0, 0x10111213>::memory_block_t;

                using casted_expected = ctai::values_container_n::cast_values_to<expected_mem_block, size_t>;
                using casted_result = ctai::values_container_n::cast_values_to<result_mem_block, size_t>;

                ASSERT_EQ_T(casted_expected, casted_result);
            }

            namespace t2
            {
                using expected_mem_block = vc<0,1,2,0x10,0x11, 0x12,0x13,7,8,9>;
                using result_mem_block = typename ctai::memory::set_32<test_memory, 3, 0x10111213>::memory_block_t;

                using casted_expected = ctai::values_container_n::cast_values_to<expected_mem_block, size_t>;
                using casted_result = ctai::values_container_n::cast_values_to<result_mem_block, size_t>;

                ASSERT_EQ_T(casted_expected, casted_result);
            }

            namespace t3
            {
                using expected_mem_block = vc<0,1,2,3,4,5,0x10,0x11, 0x12,0x13>;
                using result_mem_block = typename ctai::memory::set_32<test_memory, 6, 0x10111213>::memory_block_t;

                using casted_expected = ctai::values_container_n::cast_values_to<expected_mem_block, size_t>;
                using casted_result = ctai::values_container_n::cast_values_to<result_mem_block, size_t>;

                ASSERT_EQ_T(casted_expected, casted_result);
            }
        }
    }

    namespace test_get
    {
        namespace test_get_8
        {
            namespace t1
            {
                constexpr auto expected = 0;
                constexpr auto result = ctai::memory::get_8<test_memory, 0>;
                ASSERT_EQ(expected, result);
            }

            namespace t2
            {
                constexpr auto expected = 3;
                constexpr auto result = ctai::memory::get_8<test_memory, 3>;
                ASSERT_EQ(expected, result);
            }

            namespace t3
            {
                constexpr auto expected = 9;
                constexpr auto result = ctai::memory::get_8<test_memory, 9>;
                ASSERT_EQ(expected, result);
            }
        }

        namespace test_get_16
        {
            namespace t1
            {
                constexpr auto expected = 0x0001;
                constexpr auto result = ctai::memory::get_16<test_memory, 0>;
                ASSERT_EQ(expected, result);
            }

            namespace t2
            {
                constexpr auto expected = 0x0304;
                constexpr auto result = ctai::memory::get_16<test_memory, 3>;
                ASSERT_EQ(expected, result);
            }

            namespace t3
            {
                constexpr auto expected = 0x0809;
                constexpr auto result = ctai::memory::get_16<test_memory, 8>;
                ASSERT_EQ(expected, result);
            }
        }

        namespace test_get_32
        {
            namespace t1
            {
                constexpr auto expected = 0x00010203;
                constexpr auto result = ctai::memory::get_32<test_memory, 0>;
                ASSERT_EQ(expected, result);
            }

            namespace t2
            {
                constexpr auto expected = 0x03040506;
                constexpr auto result = ctai::memory::get_32<test_memory, 3>;
                ASSERT_EQ(expected, result);
            }

            namespace t3
            {
                constexpr auto expected = 0x06070809;
                constexpr auto result = ctai::memory::get_32<test_memory, 6>;
                ASSERT_EQ(expected, result);
            }
        }
    }
}

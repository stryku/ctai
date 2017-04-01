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
    }
}

#pragma once

#include "tests/tests_macros.hpp"
#include "memory/memory_metadata.hpp"
#include "memory/memory_alloc.hpp"

namespace ctai::tests::memory_alloc
{
    namespace test_alloc
    {
        namespace alloc_success
        {
            namespace t1
            {
                using start_metadata = ctai::memory::metadata::memory_metadata<ctai::values_container_n::values_container<false, false, false, false, false>,
                                                                               ctai::tuple_n::tuple<>>;

                constexpr auto result = ctai::memory::alloc<start_metadata, 2>::result_ptr;

                ASSERT_NOT_EQ(result, 0);
            }
        }

        namespace alloc_fail
        {
            namespace t1
            {
                using start_metadata = ctai::memory::metadata::memory_metadata<ctai::values_container_n::values_container<false, true>,
                                                                               ctai::tuple_n::tuple<>>;

                constexpr auto result = ctai::memory::alloc<start_metadata, 2>::result_ptr;

                ASSERT_EQ(result, 0);
            }
        }
    }
}

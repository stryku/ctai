#pragma once

#include "tuple/tuple.hpp"
#include "tests/tests_macros.hpp"

namespace ctai::tests::tuple
{
    using test_tuple = ctai::tuple_n::tuple<int, char>;

    namespace test_append
    {
        namespace t1
        {
            using expected = ctai::tuple_n::tuple<int, char, bool>;
            using result = ctai::tuple_n::append<test_tuple, bool>;
            ASSERT_EQ_T(expected, result);
        }
    }

    namespace test_merge
    {
        namespace t1
        {
            using expected = ctai::tuple_n::tuple<int, char, bool, double>;
            using right_tuple = ctai::tuple_n::tuple<bool, double>;
            using result = ctai::tuple_n::merge<test_tuple, right_tuple>;
            ASSERT_EQ_T(expected, result);
        }
    }
}

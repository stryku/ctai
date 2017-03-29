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

    namespace test_find_if
    {
        template <size_t desired>
        struct predicate
        {
            template <typename pair>
            static constexpr auto value = ( pair::second == desired);
        };

        template <size_t first_v, size_t second_v>
        struct pair
        {
            static constexpr auto first = first_v;
            static constexpr auto second = second_v;
        };

        using test_tuple_pairs = ctai::tuple_n::tuple<pair<1,2>,
                                                      pair<2,3>,
                                                      pair<5,6>>;


        namespace find_if_empty_tuple
        {
            using expected = ctai::utils::empty_type;
            using test_predicate = predicate<2>;
            using result = ctai::tuple_n::find_if<ctai::tuple_n::tuple<>,
                                                  test_predicate>;

            ASSERT_EQ_T(expected, result);
        }

        namespace find_if_not_empty_not_found
        {
            using expected = ctai::utils::empty_type;
            using test_predicate = predicate<999>;
            using result = ctai::tuple_n::find_if<test_tuple_pairs,
                                                  test_predicate>;

            ASSERT_EQ_T(expected, result);
        }

        namespace find_if_not_empty_found
        {
            using expected = pair<2,3>;
            using test_predicate = predicate<3>;
            using result = ctai::tuple_n::find_if<test_tuple_pairs,
                                                  test_predicate>;

            ASSERT_EQ_T(expected, result);
        }
    }
}

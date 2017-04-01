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

    namespace test_find_if_it
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

        namespace find_if_it_empty_tuple
        {
            constexpr auto expected = ctai::utils::bad_value;
            using test_predicate = predicate<2>;
            constexpr auto result = ctai::tuple_n::find_if_it<ctai::tuple_n::tuple<>,
                                                              test_predicate>;

            ASSERT_EQ(expected, result);
        }

        namespace find_if_it_not_empty_not_found
        {
            constexpr auto expected = ctai::utils::bad_value;
            using test_predicate = predicate<999>;
            constexpr auto result = ctai::tuple_n::find_if_it<test_tuple_pairs,
                                                              test_predicate>;

            ASSERT_EQ(expected, result);
        }

        namespace find_if_it_not_empty_found
        {
            constexpr auto expected = 1;
            using test_predicate = predicate<3>;
            constexpr auto result = ctai::tuple_n::find_if_it<test_tuple_pairs,
                                                              test_predicate>;

            ASSERT_EQ(expected, result);
        }
    }

    namespace test_take
    {
        namespace take_zero
        {
            using expected_taken = ctai::tuple_n::tuple<>;
            using expected_rest = test_tuple;
            using result = ctai::tuple_n::take<test_tuple, 0>;

            ASSERT_EQ_T(expected_taken, typename result::taken);
            ASSERT_EQ_T(expected_rest, typename result::rest);
        }

        namespace take_one
        {
            using expected_taken = ctai::tuple_n::tuple<int>;
            using expected_rest = ctai::tuple_n::tuple<char>;
            using result = ctai::tuple_n::take<test_tuple, 1>;

            ASSERT_EQ_T(expected_taken, typename result::taken);
            ASSERT_EQ_T(expected_rest, typename result::rest);
        }

        namespace take_all
        {
            using expected_taken = ctai::tuple_n::tuple<int, char>;
            using expected_rest = ctai::tuple_n::tuple<>;
            using result = ctai::tuple_n::take<test_tuple, 2>;

            ASSERT_EQ_T(expected_taken, typename result::taken);
            ASSERT_EQ_T(expected_rest, typename result::rest);
        }
    }

    namespace test_erase
    {
        namespace erase_zero
        {
            using expected = test_tuple;
            using result = ctai::tuple_n::erase<test_tuple, 0, 0>;

            ASSERT_EQ_T(expected, result);
        }

        namespace erase_one
        {
            namespace t1
            {
                using expected = ctai::tuple_n::tuple<char>;
                using result = ctai::tuple_n::erase<test_tuple, 0, 1>;

                ASSERT_EQ_T(expected, result);
            }

            namespace t2
            {
                using expected = ctai::tuple_n::tuple<int>;
                using result = ctai::tuple_n::erase<test_tuple, 1, 1>;

                ASSERT_EQ_T(expected, result);
            }
        }

        namespace erase_all
        {
            using expected = ctai::tuple_n::tuple<>;
            using result = ctai::tuple_n::erase<test_tuple, 0, 2>;

            ASSERT_EQ_T(expected, result);
        }
    }
}

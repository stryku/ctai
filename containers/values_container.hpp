#pragma once

#include <utility>

namespace ctai
{
    namespace values_container_n
    {
        template<auto ...values>
        struct values_container
        {
            static constexpr auto size = sizeof...(values);
        };

        //
        //front
        //
        namespace details
        {
            template<typename container>
            struct front_impl;

            template<auto front, auto ...chars>
            struct front_impl<values_container<front, chars...>>
            {
                static constexpr auto value = front;
            };
        }

        template<typename container>
        constexpr auto front = details::front_impl<container>::value;

        //
        //append
        //
        namespace details
        {
            template <typename container, auto value>
            struct append_impl;

            template <auto ...values, auto value>
            struct append_impl<values_container<values...>, value>
            {
                using result = values_container<values..., value>;
            };
        }

        template <typename container, auto value>
        using append = typename details::append_impl<container, value>::result;

        //
        //create_with_val
        //
        namespace details
        {
            template <size_t count, auto val, bool end, typename current_container = values_container<>>
            struct create_with_val_impl;

            template <auto val, typename current_container>
            struct create_with_val_impl<0, val, true, current_container>
            {
                using result = current_container;
            };

            template <size_t count, auto val, typename current_container>
            struct create_with_val_impl<count, val, false, current_container>
            {
                using next_container = append<current_container, val>;

                using result = typename create_with_val_impl<count - 1,
                                                             val,
                                                             count == 1,
                                                             next_container>::result;
            };
        }

        template <size_t size, auto val>
        using create_with_val = typename details::create_with_val_impl<size, val, size == 0>::result;

        //
        //merge
        //
        namespace details
        {
            template <typename container1, typename container2>
            struct merge_impl;

            template <auto ...values1, auto ...values2>
            struct merge_impl<values_container<values1...>, values_container<values2...>>
            {
                using result = values_container<values1..., values2...>;
            };
        }

        template <typename container1, typename container2>
        using merge = typename details::merge_impl<container1, container2>::result;

        //
        //pop_front
        //
        namespace details
        {
            template <typename container>
            struct pop_front_impl;

            template <auto front, auto ...values>
            struct pop_front_impl<values_container<front, values...>>
            {
                using result = values_container<values...>;
            };
        }

        template <typename container>
        using pop_front = typename details::pop_front_impl<container>::result;


        //
        // take
        //
        namespace details
        {
            template <size_t count, bool end, typename rest, typename taken = values_container<>>
            struct take_impl;

            template <auto ...rest_of_values, auto ...taken_values>
            struct take_impl<0, true, values_container<rest_of_values...>, values_container<taken_values...>>
            {
                using type = take_impl;

                using taken_container = values_container<taken_values...>;
                using rest_container = values_container<rest_of_values...>;
            };

            template <size_t count, auto to_take, auto ...rest_of_values, auto ...taken_values>
            struct take_impl<count, false, values_container<to_take, rest_of_values...>, values_container<taken_values...>>
            {
                using type = take_impl<count-1,
                                       count == 1,
                                       values_container<rest_of_values...>,
                                       values_container<taken_values..., to_take>>;

                using taken_container = typename type::taken_container;
                using rest_container = typename type::rest_container;
            };
        }

        template <size_t count, typename container_t>
        using take = typename details::take_impl<count, count==0, container_t>::type;

        //
        //set
        //
        namespace details
        {
            template <size_t position, typename container, auto ...values>
            struct set_impl
            {
                using take_result = take<position, container>;

                using taken_front = typename take_result::taken_container;
                using taken_rest = typename take_result::rest_container;

                static constexpr auto values_count = sizeof...(values);

                using rest_of_rest = typename take<values_count, taken_rest>::rest_container;
                using merged_back = merge<values_container<values...>, rest_of_rest >;

                using result = merge<taken_front, merged_back>;
            };
        }

        template <size_t position, typename container, auto ...values>
        using set = typename details::set_impl<position, container, values...>::result;

        //
        //set_from_container
        //
        namespace details
        {
            template <size_t position, typename container, typename container_with_values>
            struct set_from_container_impl;

            template <size_t position, typename container, auto ...values>
            struct set_from_container_impl<position, container, values_container<values...>>
            {
                using result = set<position, container, values...>;
            };
        }

        template <typename container, size_t position, typename container_with_values>
        using set_from_container = typename details::set_from_container_impl<position, container, container_with_values>::result;

        //
        //set_val
        //
        namespace details
        {
            template <typename container, size_t position, size_t count, auto val>
            struct set_val_impl
            {
                using take_result = take<position, container>;
                using front = typename take_result::taken_container;
                using rest = typename take_result::rest_container;

                using tail = typename take<count, rest>::rest_container;

                using setted_part = create_with_val<count, val>;

                using merged_tail = merge<setted_part, tail>;

                using result = merge<front,merged_tail>;
            };
        }

        template <typename container, size_t position, size_t count, auto val>
        using set_val = typename details::set_val_impl<container, position, count, val>::result; // todo tests

        //
        //drop_front
        //
        namespace details
        {
            template <size_t count, bool end, typename rest>
            struct drop_front_impl;

            template <auto ...rest_of_values>
            struct drop_front_impl<0, true, values_container<rest_of_values...>>
            {
                using result = values_container<rest_of_values...>;
            };

            template <size_t count, auto to_drop, auto ...rest_of_values>
            struct drop_front_impl<count, false, values_container<to_drop, rest_of_values...>>
            {
                using result = typename drop_front_impl<count-1,
                                                        count == 1,
                                                        values_container<rest_of_values...>>::result;
            };
        }

        template <typename container, size_t count>
        using drop_front = typename details::drop_front_impl<count, count==0, container>::result;

        //
        //get
        //
        namespace details
        {
            template <size_t ptr, typename container>
            struct get_impl
            {
                using after_drop = drop_front<container, ptr>;

                static constexpr auto val = front<after_drop>;
            };
        }

        template <size_t ptr, typename container>
        constexpr auto get = details::get_impl<ptr, container>::val;

        //
        //create
        //
        namespace details
        {
            template <typename value_type, typename Is>
            struct create_impl;


            template <typename value_type, auto ...tmps>
            struct create_impl<value_type, std::integer_sequence<size_t, tmps...>>
            {
                using result = values_container<static_cast<value_type>(tmps^tmps)...>;
            };
        }

        template <typename value_type, size_t size>
        using create = typename details::create_impl<value_type, std::make_index_sequence<size>>::result;
    }
}

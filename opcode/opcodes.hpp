#pragma once

#include <containers/values_container.hpp>
#include "eip_change.hpp"
#include <cstdlib>

namespace ctai
{
    namespace opcode
    {
        template <auto ...values>
        struct opcodes
        {
            static constexpr size_t opcodes_array[sizeof...(values)] = { values... };
        };

        //
        //create_opcodes_impl
        //
        namespace details
        {
            template <typename opcodes_values_container>
            struct create_opcodes_impl;

            template <auto ...values>
            struct create_opcodes_impl<values_container_n::values_container<values...>>
            {
                using result = opcodes<values...>;
            };
        }

        template <typename opcodes_values_container>
        using create_opcodes = typename details::create_opcodes_impl<opcodes_values_container>::result;

        //
        //get_next_opcodes
        //
        namespace details
        {
            template <typename opcodes_t, size_t eip, size_t instruction_size>
            struct get_next_opcodes_impl;

            template <typename opcodes_t, size_t eip>
            struct get_next_opcodes_impl<opcodes_t, eip, 1>
            {
                using result = values_container_n::values_container<opcodes_t::opcodes_array[eip]>;
            };

            template <typename opcodes_t, size_t eip>
            struct get_next_opcodes_impl<opcodes_t, eip, 2>
            {
                using result = values_container_n::values_container<opcodes_t::opcodes_array[eip],
                                                                    opcodes_t::opcodes_array[eip + 1]>;
            };

            template <typename opcodes_t, size_t eip>
            struct get_next_opcodes_impl<opcodes_t, eip, 3>
            {
                using result = values_container_n::values_container<opcodes_t::opcodes_array[eip],
                                                                    opcodes_t::opcodes_array[eip + 1],
                                                                    opcodes_t::opcodes_array[eip + 2]>;
            };

            template <typename opcodes_t, size_t eip>
            struct get_next_opcodes_impl<opcodes_t, eip, 4>
            {
                using result = values_container_n::values_container<opcodes_t::opcodes_array[eip],
                                                                    opcodes_t::opcodes_array[eip + 1],
                                                                    opcodes_t::opcodes_array[eip + 2],
                                                                    opcodes_t::opcodes_array[eip + 3]>;
            };

            template <typename opcodes_t, size_t eip>
            struct get_next_opcodes_impl<opcodes_t, eip, 5>
            {
                using result = values_container_n::values_container<opcodes_t::opcodes_array[eip],
                                                                    opcodes_t::opcodes_array[eip + 1],
                                                                    opcodes_t::opcodes_array[eip + 2],
                                                                    opcodes_t::opcodes_array[eip + 3],
                                                                    opcodes_t::opcodes_array[eip + 4]>;
            };
        }

        template <typename opcodes_t, size_t eip>
        using get_next_opcodes = typename details::get_next_opcodes_impl<opcodes_t,
                                                                         eip,
                                                                         get_eip_change<inst::to_id<opcodes_t::opcodes_array[eip]>>>::result;
    }
}

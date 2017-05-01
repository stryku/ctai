#pragma once

#include "ctai/tuple.hpp"
#include "ctai/tokenize/syscalls_tokens.hpp"
#include "ctai/instructions/ids_vaules.hpp"
#include "ctai/containers/values_container.hpp"
#include "ctai/eip_change.hpp"

namespace ctai
{
    namespace details
    {
        //sys_is_thread_running
        template <typename ...rest_of_tokens>
        struct matcher_impl<tuple<
                tokens::tok_sys_is_thread_running,
                rest_of_tokens...>>
        {
            using instruction = values_container_n::values_container<inst::to_size<inst::id_t::SYS_IS_THREAD_RUNNING>>;
            using instruction_tokens = tuple<tokens::tok_sys_is_thread_running>;
            static constexpr auto eip_change = get_eip_change<inst::id_t::SYS_IS_THREAD_RUNNING>;
            using rest_of_tokens_t = tuple<rest_of_tokens...>;
        };
    }

    template <typename tokens>
    using instruction_match = typename details::matcher_impl<tokens>;
}
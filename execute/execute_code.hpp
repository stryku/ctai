#pragma once

#include "instructions/ids_vaules.hpp"
#include "register.hpp"
#include "tuple.hpp"
#include "machine_state.hpp"
#include "execute/ex_instruction.hpp"
#include "execute/ex_push.hpp"
#include "tokenize/tokenizer.hpp"
#include "assembler/assembler.hpp"
#include "assembler/label_substitute.hpp"
#include "assembler/label_extract.hpp"

#include "thread/thread.hpp"
#include "thread/scheduler_round_robin.hpp"
#include "execute/execute_thread.hpp"
#include "machine/machine_state.hpp"
#include "utils/empty_type.hpp"
#include "io/output.hpp"
#include "execute/execution_result.hpp"

class empty_type;
namespace ctai
{
    namespace execute2
    {
        namespace details
        {
            template <typename machine_state>
            struct execute_impl;

            template <typename memory_t,
                    typename opcodes_t,
                    typename output_t,
                      size_t time_v,
                              size_t last_thread_id>
            struct execute_impl<machine::state<memory_t,
                                               opcodes_t,
                                               tuple_n::tuple<>,
                    output_t,
                                               time_v,
                    last_thread_id>>
            {
                using output = output_t;
                static constexpr auto ret_val = time_v;

                using result = execute_impl;
            };

            template <typename machine_state>
            struct execute_impl
            {
                using scheduler_result = thread::schedule::next<typename machine_state::threads>;
                using thread_to_execute = typename scheduler_result::result_thread;
                using machine_state_to_execute = machine::set_threads<machine_state, typename scheduler_result::result_threads_queue>;
                using thread_execution_result = execute2::execute_thread<thread_to_execute,
                                                                         machine_state_to_execute,
                                                                         typename machine_state::opcodes>;

                using threads_queue_after_execution = typename thread_execution_result::result_machine_state::threads;

                using next_threads_queue = std::conditional_t<thread_execution_result::result_thread::finished,
                                                              threads_queue_after_execution,
                                                              queue::push<threads_queue_after_execution,
                                                                          typename thread_execution_result::result_thread>>;

                using next_machine_state = machine::set_threads_and_time<typename thread_execution_result::result_machine_state,
                        next_threads_queue,
                        machine_state::time + thread_execution_result::executed_instructions_count>;

                using result = typename execute_impl<next_machine_state>::result;
            };
        }

        namespace details
        {
            template <typename code>
            struct prepare_and_execute
            {
                using tokens = tokenize<code>;
                using extract_labels_result = extract_labels<tokens>;
                using tokens_without_labels = typename extract_labels_result::tokens;
                using labels_metadata = typename extract_labels_result::labels;
                using tokens_after_labels_substitution = substutite_labels<tokens_without_labels, labels_metadata>;
                using opcodes = assemble<tokens_after_labels_substitution>;

                static constexpr auto main_ip = labels_get_ip<labels_metadata, string<'.', 'm', 'a', 'i', 'n'>>;

                static constexpr auto memory_size = static_cast<size_t>(100);

                using root_thread = thread::create<18, //priority
                                                   0,   //id
                                                   main_ip,   //eip
                        memory_size>;  //esp

                using memory_t = memory::memory_create<memory_size>;


                using machine_state = machine::state<memory_t,
                                                     opcodes,
                                                     tuple_n::tuple<root_thread>,
                        io::output::buffer<>,
                                                     0,
                                                             0>; //time

                using execute_result = typename execute_impl<machine_state>::result;

                using result = execution_result<typename execute_result::output,
                        execute_result::ret_val>;
            };
        }

        template <typename code>
        using execute_code = typename details::prepare_and_execute<code>::result;
    }
}

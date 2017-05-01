#pragma once

#include "ctai/instructions/ids_vaules.hpp"
#include "ctai/register.hpp"
#include "ctai/tuple.hpp"
#include "ctai/tokenize/tokenizer.hpp"
#include "ctai/assembler/assembler.hpp"
#include "ctai/assembler/label_substitute.hpp"
#include "ctai/assembler/label_extract.hpp"
#include "ctai/thread/thread.hpp"
#include "ctai/thread/scheduler_round_robin.hpp"
#include "execute_thread.hpp"
#include "ctai/machine/machine_state.hpp"
#include "ctai/utils/empty_type.hpp"
#include "ctai/io/output.hpp"
#include "execution_result.hpp"
#include "ctai/io/input.hpp"
#include "ctai/opcode/opcodes.hpp"

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
                      typename input_t,
                      size_t time_v,
                      size_t last_thread_id>
            struct execute_impl<machine::state<memory_t,
                                               opcodes_t,
                                               tuple_n::tuple<>, //empty threads queue
                                               output_t,
                                               input_t,
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
            template <typename code, typename input_t>
            struct prepare_and_execute
            {
                using tokens = tokenize<code>;
                using extract_labels_result = extract_labels<tokens>;
                using tokens_without_labels = typename extract_labels_result::tokens;
                using labels_metadata = typename extract_labels_result::labels;
                using tokens_after_labels_substitution = substutite_labels<tokens_without_labels, labels_metadata>;
                using opcodes_values_container = assemble<tokens_after_labels_substitution>;
                using opcodes_t = opcode::create_opcodes<opcodes_values_container>;

                static constexpr auto main_ip = labels_get_ip<labels_metadata, string<'.', 'm', 'a', 'i', 'n'>>;
                static constexpr auto memory_size = static_cast<size_t>(400);

                using root_thread = thread::create<18, //priority
                                                   0,   //id
                                                   main_ip,   //eip
                                                   memory_size>;  //esp

                using memory_t = memory::memory_create<memory_size>;

                using machine_state = machine::state<memory_t,
                                                     opcodes_t,
                                                     tuple_n::tuple<root_thread>,
                                                     io::output::buffer<>,
                                                     input_t,
                                                     0,//time
                                                     0>; //thread id counter

                using execute_result = typename execute_impl<machine_state>::result;

                using result = execution_result<typename execute_result::output,
                                                execute_result::ret_val>;
            };
        }

        template <typename code, typename input_t>
        using execute_code = typename details::prepare_and_execute<code, input_t>::result;
    }
}

#pragma once

#include "tuple.hpp"

#include <cstddef>

namespace ctai
{
    template <typename name, size_t ip>
    struct label_metadata {};

    //
    // labels_add
    //
    namespace details
    {
        template <typename, typename, size_t>
        struct label_add_impl;

        template <typename ...current_labels, typename name, size_t ip>
        struct label_add_impl<tuple<current_labels...>, name, ip>
        {
            using type = tuple<current_labels..., label_metadata<name, ip>>;
        };
    }

    template <typename current_labels, typename name, size_t ip>
    using labels_add = typename details::label_add_impl<current_labels, name, ip>::type;

    //
    // labels_get_ip
    //
    namespace details
    {
        template <typename, typename>
        struct labels_get_ip_impl;

        template <typename ...current_labels, typename name, size_t ip_v>
        struct labels_get_ip_impl<tuple<label_metadata<name, ip_v>, current_labels...>, name>
        {
            static constexpr auto ip = ip_v;
        };

        template <typename current_label, typename ...current_labels, typename name>
        struct labels_get_ip_impl<tuple<current_label, current_labels...>, name>
        {
            static constexpr auto ip = labels_get_ip_impl<tuple<current_labels...>, name>::ip;
        };
    }

    template <typename labels, typename name>
    constexpr auto labels_get_ip = details::labels_get_ip_impl<labels, name>::ip;
}
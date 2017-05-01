#pragma once

namespace ctai::utils
{
    template <typename predicate, typename type>
    constexpr auto predicate_v = predicate::template value<type>;
}
#pragma once

#include "param_type.hpp"

namespace ethabi::details
{
    constexpr inline auto
    make_shared(auto&& arg)
    {
        return std::make_shared<param_type>(std::forward<param_type>(arg));
    }

    inline namespace param_type_literals
    {
        constexpr inline auto operator"" _u(unsigned long long int n) { return uint_t(n); }

        constexpr inline auto operator"" _i(unsigned long long int n) { return int_t(n); }

        constexpr inline auto operator"" _fb(unsigned long long int n) { return fixed_bytes_t(n); }
    } // namespace param_type_literals
} // namespace ethabi::details
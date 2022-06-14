#pragma once

#include <utility>

#include "./is_strong_type.hpp"

namespace st
{
    template <typename T>
    constexpr inline auto&&
    unwrap(T&& t) noexcept
    {
        if constexpr (is_strong_type_v<std::remove_cv_t<std::remove_reference_t<T>>>)
        {
            return std::forward<T>(t).value();
        }
        else
        {
            return std::forward<T>(t);
        }
    }
} // namespace st

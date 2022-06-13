#pragma once

/// Deps Headers
#include <fmt/format.h>

/// Project Headers
#include "param_type.hpp"

template<>
struct fmt::formatter<ethabi::details::uint_t> : formatter<std::size_t>
{
  static inline auto format(ethabi::details::uint_t val, format_context& ctx)
  {
    return fmt::format_to(ctx.out(), "uint{}", val.value());
  }
};

template<>
struct fmt::formatter<ethabi::details::int_t> : formatter<std::size_t>
{
  static inline auto format(ethabi::details::int_t val, format_context& ctx)
  {
    return fmt::format_to(ctx.out(), "int{}", val.value());
  }
};
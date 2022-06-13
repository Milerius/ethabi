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

template<>
struct fmt::formatter<ethabi::details::fixed_bytes_t> : formatter<std::size_t>
{
  static inline auto format(ethabi::details::fixed_bytes_t val, format_context& ctx)
  {
    return fmt::format_to(ctx.out(), "bytes{}", val.value());
  }
};

template<>
struct fmt::formatter<ethabi::details::bool_t> : formatter<std::size_t>
{
  static inline auto format([[maybe_unused]] ethabi::details::bool_t val, format_context& ctx)
  {
    return fmt::format_to(ctx.out(), "bool");
  }
};
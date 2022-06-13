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
struct fmt::formatter<ethabi::details::bool_t> : formatter<bool>
{
  static inline auto format([[maybe_unused]] ethabi::details::bool_t val, format_context& ctx)
  {
    return fmt::format_to(ctx.out(), "bool");
  }
};

template<>
struct fmt::formatter<ethabi::details::string_t> : formatter<std::string>
{
  static inline auto format([[maybe_unused]] ethabi::details::string_t val, format_context& ctx)
  {
    return fmt::format_to(ctx.out(), "string");
  }
};

template<>
struct fmt::formatter<ethabi::details::bytes_t>
{
  static constexpr auto parse(format_parse_context& ctx)
  {
    return ctx.begin();
  }

  static inline auto format([[maybe_unused]] ethabi::details::bytes_t val, format_context& ctx)
  {
    return fmt::format_to(ctx.out(), "bytes");
  }
};

template<>
struct fmt::formatter<ethabi::details::address_t>
{
  static constexpr auto parse(format_parse_context& ctx)
  {
    return ctx.begin();
  }

  static inline auto format([[maybe_unused]] ethabi::details::address_t val, format_context& ctx)
  {
    return fmt::format_to(ctx.out(), "address");
  }
};
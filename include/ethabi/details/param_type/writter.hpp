#pragma once

/// Deps Headers
#include <fmt/format.h>

#include <range/v3/view/iota.hpp>
#include <range/v3/view/zip.hpp>

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

namespace ethabi::details
{
  std::string format(const param_type& pt, bool serialize_tuple_content = true)
  {
    std::string result;
    auto visit_functor = [&result, serialize_tuple_content](auto&& arg)
    {
      using T = std::remove_cvref_t<decltype(arg)>;
      if constexpr (
          std::is_same_v<T, int_t> || std::is_same_v<T, uint_t> || std::is_same_v<T, address_t> ||
          std::is_same_v<T, bytes_t> || std::is_same_v<T, bool_t> || std::is_same_v<T, fixed_bytes_t>)
      {
        result = fmt::format("{}", arg);
      }
      else if constexpr (std::is_same_v<T, fixed_array_t<details::param_type>>)
      {
        if (arg.first)
        {
          result = fmt::format("{}[{}]", format(*arg.first, serialize_tuple_content), arg.second);
        }
      }
      else if constexpr (std::is_same_v<T, array_t<details::param_type>>)
      {
        if (arg)
        {
          result = fmt::format("{}[]", format(*arg, serialize_tuple_content));
        }
      }
      else if constexpr (std::is_same_v<T, tuple_t<details::param_type>>)
      {
        using namespace ranges::views;
        for (auto&& [cur, idx] : zip(arg, ints(0u, ranges::unreachable)))
        {
          result.append(format(cur, serialize_tuple_content));
          if (idx < arg.size() - 1)
          {
            result.append(",");
          }
        }
        result = fmt::format("({})", result);
      }
    };
    rva::visit(visit_functor, pt);
    return result;
  }
}  // namespace ethabi::details
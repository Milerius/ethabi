//
// Created by sztergbaum roman on 13/06/2022.
//

#include "ethabi/details/param_type/writter.hpp"

namespace ethabi::details {
  std::string format(const param_type& pt, bool serialize_tuple_content)
  {
    std::string result;
    auto visit_functor = [&result, serialize_tuple_content](auto&& arg)
    {
      using T = std::remove_cvref_t<decltype(arg)>;
      if constexpr (meta::is_any_of<T, int_t, uint_t, address_t, bytes_t, bool_t, fixed_bytes_t>)
      {
        result = fmt::format("{}", arg);
      }
      else if constexpr (std::is_same_v<T, fixed_array_t<details::param_type>>)
      {
        result = fmt::format("{}[{}]", format(*arg.first, serialize_tuple_content), arg.second);
      }
      else if constexpr (std::is_same_v<T, array_t<details::param_type>>)
      {
        result = fmt::format("{}[]", format(*arg, serialize_tuple_content));
      }
      else if constexpr (std::is_same_v<T, tuple_t<details::param_type>>)
      {
        using namespace ranges::views;
        if (serialize_tuple_content)
        {
          for (auto&& [idx, cur] : ranges::views::enumerate(arg))
          {
            result.append(format(cur, serialize_tuple_content));
            if (idx < arg.size() - 1)
            {
              result.append(",");
            }
          }
          result = fmt::format("({})", result);
        }
      }
      else
      {
        result = "tuple";
      }
    };
    rva::visit(visit_functor, pt);
    return result;
  }
}
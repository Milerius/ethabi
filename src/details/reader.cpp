//
// Created by sztergbaum roman on 13/06/2022.
//

#include "ethabi/details/param_type/reader.hpp"

#include <fmt/format.h>

#include <algorithm>
#include <range/v3/view/enumerate.hpp>
#include <range/v3/view/reverse.hpp>
#include <range/v3/view/take_while.hpp>

#include "ethabi/details/param_type/factory.hpp"

enum class parser_token : char
{
  left_parenthesis = '(',
  right_parenthesis = ')',
  comma = ',',
  right_bracket = ']',
  left_bracket = '['
};

namespace ethabi::details
{
  [[maybe_unused]] read_result_t read([[maybe_unused]] const std::string& name)
  {
    const auto last_char = static_cast<parser_token>(name.back());
    switch (last_char)
    {
      case parser_token::right_parenthesis:
      {
        if (!name.starts_with(static_cast<char>(parser_token::left_parenthesis)))
        {
          return tl::make_unexpected(ethabi::error::param_type_read::invalid_name);
        }

        std::vector<param_type> subtypes;
        std::vector<tuple_t<param_type>> subtuples;
        std::int64_t nested{ 0 };
        std::size_t top_level_paren_open{ 0 };
        std::size_t last_item{ 1 };
        for (auto&& [pos, c] : ranges::views::enumerate(name))
        {
          const auto token = static_cast<parser_token>(c);
          switch (token)
          {
            case parser_token::left_parenthesis:
            {
              top_level_paren_open = pos;
              nested += 1;
              if (nested > 1)
              {
                subtuples.emplace_back(std::vector<param_type>{});
                last_item = pos + 1;
              }
              break;
            }
            case parser_token::right_parenthesis:
            {
              nested -= 1;
              if (nested < 0)
              {
                return tl::make_unexpected(ethabi::error::param_type_read::invalid_name);
              }
              if (std::string(begin(name) + static_cast<long>(last_item), begin(name) + static_cast<long>(pos)).empty())
              {
                last_item = pos + 1;
              }
              else if (nested == 0)
              {
                auto sub = std::string(begin(name) + static_cast<long>(last_item), begin(name) + static_cast<long>(pos));
                if (auto result = read(sub); result)
                {
                  subtypes.emplace_back(result.value());
                  last_item = pos + 1;
                }
              }
              else if (nested > 0)
              {
                auto sub = std::string(begin(name) + static_cast<long>(pos) + 1, end(name));
                for (auto&& [_, cur] : ranges::views::enumerate(sub))
                {
                  if (cur == ',' || cur == ')')
                  {
                    break;
                  }
                  pos += 1;
                }
                auto inner_tuple = std::string(
                    begin(name) + static_cast<long>(top_level_paren_open), begin(name) + static_cast<long>(pos) + 1);
                if (auto subtype = read(inner_tuple); subtype)
                {
                  if (nested > 1)
                  {
                    auto subtuple = subtuples[static_cast<unsigned long>(nested - 2)];
                    subtuple.emplace_back(subtype.value());
                    subtypes.emplace_back(subtuple);
                  }
                  else
                  {
                    subtypes.emplace_back(subtype.value());
                  }
                  last_item = pos + 1;
                }
              }
              break;
            }
            case parser_token::comma:
            {
              auto str = std::string(begin(name) + static_cast<long>(last_item), begin(name) + static_cast<long>(pos));
              if (str.empty())
              {
                last_item = pos + 1;
              }
              // If the item is in the top level of the tuple insert it into subtypes
              else if (nested == 1)
              {
                auto sub = std::string(begin(name) + static_cast<long>(last_item), begin(name) + static_cast<long>(pos));
                if (auto subtype = read(sub); subtype)
                {
                  subtypes.emplace_back(subtype.value());
                  last_item = pos + 1;
                }
              }
              // If the item is in a sublevel of the tuple
              // insert it into the subtuple vector for the current depth level
              else if (nested > 1)
              {
                auto sub = std::string(begin(name) + static_cast<long>(last_item), begin(name) + static_cast<long>(pos));
                if (auto subtype = read(sub); subtype)
                {
                  subtuples[static_cast<unsigned long>(nested - 2)].emplace_back(subtype.value());
                  last_item = pos + 1;
                }
              }
              break;
            }
            default: break;
          }
        }
        return subtypes;
      }
      case parser_token::right_bracket:
      {
        namespace rg = ranges::views;
        auto reversed = std::string(name.rbegin() + 1, name.rend());
        auto take_while_functor = [](char cur) { return static_cast<parser_token>(cur) != parser_token::left_bracket; };
        std::string num = rg::take_while(reversed, take_while_functor) | rg::reverse | ranges::to<std::string>();
        std::size_t count = name.size();
        if (num.empty())
        {
          if (auto subtype = read(name.substr(0, count - 2)); subtype)
          {
            return array_t<param_type>(make_shared(subtype.value()));
          }
        }
        else
        {
          if (auto subtype = read(name.substr(0, count - num.size() - 2)); subtype)
          {
            auto len = static_cast<size_t>(std::stoi(num));
            return fixed_array_t<param_type>(make_shared(subtype.value()), len);
          }
        }
        break;
      }
      default: break;
    }
    if (name == "address")
    {
      return address_t();
    }
    if (name == "bytes")
    {
      return bytes_t();
    }
    if (name == "bool")
    {
      return bool_t();
    }
    if (name == "string")
    {
      return string_t();
    }
    if (name == "int")
    {
      return int_t(256);
    }
    if (name == "tuple")
    {
      return tuple_t<param_type>();
    }
    if (name == "uint")
    {
      return uint_t(256);
    }
    if (name.starts_with("bytes"))
    {
      auto nb_bytes = static_cast<std::size_t>(std::stoi(name.substr(sizeof("bytes") - 1)));
      return fixed_bytes_t(nb_bytes);
    }
    if (name.starts_with("int"))
    {
      auto size_integer = static_cast<std::size_t>(std::stoi(name.substr(sizeof("int") - 1)));
      return int_t(size_integer);
    }
    if (name.starts_with("uint"))
    {
      auto size_integer = static_cast<std::size_t>(std::stoi(name.substr(sizeof("uint") - 1)));
      return uint_t(size_integer);
    }
    return uint_t(8);
  }
}  // namespace ethabi::details

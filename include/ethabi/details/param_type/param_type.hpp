#pragma once

/// Standard C include
#include <cstdint>  // std::uint8_t

/// Standard C++ Headers
#include <memory>  // std::shared_ptr
#include <string>  // std::string
#include <vector>  // std::vector

#include "../rva/rva.hpp"
#include "../st/st.hpp"

namespace ethabi::details
{
  /// https://docs.soliditylang.org/en/latest/abi-spec.html#types
  /// Ethereum ABI params.
  /// https://github.com/rust-ethereum/ethabi/blob/master/ethabi/src/param_type/param_type.rs

  struct address_t
  {
  };

  using int_t = st::type<std::size_t, struct int_tag>;

  using uint_t = st::type<std::size_t, struct uint_tag>;

  using bool_t = st::type<bool, struct bool_tag>;

  using fixed_bytes = st::type<std::size_t, struct fixed_bytes_tag>;

  template<typename T = rva::self_t>
  using tuple_t = std::vector<T>;

  template<typename T = rva::self_t>
  using array_t = std::shared_ptr<T>;

  template<typename T = rva::self_t>
  using fixed_array_t = std::pair<array_t<T>, std::size_t>;

  using string_t = std::string;

  using param_type =
      rva::variant<address_t, int_t, uint_t, bool_t, fixed_bytes, tuple_t<>, string_t, array_t<>, fixed_array_t<>>;
}  // namespace ethabi::details
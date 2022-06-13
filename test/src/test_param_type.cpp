#include <doctest/doctest.h>

#include "ethabi/ethabi.hpp"

using namespace ethabi;

TEST_CASE("format uint")
{
  CHECK_EQ(fmt::format("{}", ethabi::details::uint_t(256)), "uint256");
  CHECK_EQ(fmt::format("{}", ethabi::details::int_t(64)), "int64");
}

TEST_CASE("test")
{
  ethabi::details::param_type v;
  v = ethabi::details::int_t(1);
  CHECK_EQ(std::holds_alternative<details::int_t>(v), true);
  v = ethabi::details::uint_t(2);
  CHECK_EQ(std::holds_alternative<details::uint_t>(v), true);
  v = ethabi::details::address_t();
  CHECK_EQ(std::holds_alternative<details::address_t>(v), true);
  v = ethabi::details::bool_t(true);
  CHECK_EQ(std::holds_alternative<details::bool_t>(v), true);
  v = ethabi::details::fixed_bytes(32);
  CHECK_EQ(std::holds_alternative<details::fixed_bytes>(v), true);
  v = std::vector<ethabi::details::param_type>{ details::int_t(1), details::bool_t(false) };
  CHECK_EQ(std::holds_alternative<details::tuple_t<details::param_type>>(v), true);
  auto ptr = std::make_shared<details::param_type>(details::int_t(1));
  v = ethabi::details::array_t<details::param_type>(ptr);
  CHECK_EQ(std::holds_alternative<details::array_t<details::param_type>>(v), true);
  v = ethabi::details::fixed_array_t<details::param_type>(ptr, 2);
  CHECK_EQ(std::holds_alternative<details::fixed_array_t<details::param_type>>(v), true);
}

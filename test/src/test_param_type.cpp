#include <doctest/doctest.h>

#include "ethabi/ethabi.hpp"

using namespace ethabi;

TEST_SUITE_BEGIN("ethabi");

TEST_CASE("abi format types")
{
  CHECK_EQ(fmt::format("{}", details::uint_t(256)), "uint256");
  CHECK_EQ(fmt::format("{}", details::int_t(64)), "int64");
  CHECK_EQ(fmt::format("{}", details::fixed_bytes(32)), "bytes32");
}

TEST_CASE("abi variant type")
{
  details::param_type v;
  v = details::int_t(1);
  CHECK_EQ(std::holds_alternative<details::int_t>(v), true);
  v = details::uint_t(2);
  CHECK_EQ(std::holds_alternative<details::uint_t>(v), true);
  v = details::address_t();
  CHECK_EQ(std::holds_alternative<details::address_t>(v), true);
  v = details::bool_t(true);
  CHECK_EQ(std::holds_alternative<details::bool_t>(v), true);
  v = details::fixed_bytes(32);
  CHECK_EQ(std::holds_alternative<details::fixed_bytes>(v), true);
  v = std::vector<details::param_type>{ details::int_t(1), details::bool_t(false) };
  CHECK_EQ(std::holds_alternative<details::tuple_t<details::param_type>>(v), true);
  auto ptr = std::make_shared<details::param_type>(details::int_t(1));
  v = details::array_t<details::param_type>(ptr);
  CHECK_EQ(std::holds_alternative<details::array_t<details::param_type>>(v), true);
  v = details::fixed_array_t<details::param_type>(ptr, 2);
  CHECK_EQ(std::holds_alternative<details::fixed_array_t<details::param_type>>(v), true);
}

TEST_SUITE_END();
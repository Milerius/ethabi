#include <doctest/doctest.h>

#include "ethabi/ethabi.hpp"

using namespace ethabi;

TEST_SUITE_BEGIN("ethabi::param_type::test_suite");

TEST_CASE("format_type")
{
  CHECK_EQ(fmt::format("{}", details::uint_t(256)), "uint256");
  CHECK_EQ(fmt::format("{}", details::int_t(64)), "int64");
  CHECK_EQ(fmt::format("{}", details::fixed_bytes_t(32)), "bytes32");
  CHECK_EQ(fmt::format("{}", details::bool_t(true)), "bool");
  CHECK_EQ(fmt::format("{}", details::string_t("42")), "string");
  CHECK_EQ(fmt::format("{}", details::bytes_t{}), "bytes");
  CHECK_EQ(fmt::format("{}", details::address_t{}), "address");
}

TEST_CASE("variant_type")
{
  details::param_type val;
  val = details::int_t(1);
  CHECK_EQ(std::holds_alternative<details::int_t>(val), true);
  val = details::uint_t(2);
  CHECK_EQ(std::holds_alternative<details::uint_t>(val), true);
  val = details::address_t();
  CHECK_EQ(std::holds_alternative<details::address_t>(val), true);
  val = details::bool_t(true);
  CHECK_EQ(std::holds_alternative<details::bool_t>(val), true);
  constexpr std::size_t nb_bytes = 32;
  val = details::fixed_bytes_t(nb_bytes);
  CHECK_EQ(std::holds_alternative<details::fixed_bytes_t>(val), true);
  val = std::vector<details::param_type>{ details::int_t(1), details::bool_t(false) };
  CHECK_EQ(std::holds_alternative<details::tuple_t<details::param_type>>(val), true);
  auto ptr = std::make_shared<details::param_type>(details::int_t(1));
  val = details::array_t<details::param_type>(ptr);
  CHECK_EQ(std::holds_alternative<details::array_t<details::param_type>>(val), true);
  val = details::fixed_array_t<details::param_type>(ptr, 2);
  CHECK_EQ(std::holds_alternative<details::fixed_array_t<details::param_type>>(val), true);
  val = details::string_t("42");
  CHECK_EQ(std::holds_alternative<details::string_t>(val), true);
}

TEST_SUITE_END();
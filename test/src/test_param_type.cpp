#include <doctest/doctest.h>

#include <iostream>

#include "ethabi/details/param_type.hpp"

using namespace ethabi;

TEST_SUITE_BEGIN("ethabi::param_type::test_suite");

TEST_CASE("format_type")
{
  using namespace details::param_type_literals;
  CHECK_EQ(fmt::format("{}", 256_u), "uint256");
  CHECK_EQ(fmt::format("{}", 64_i), "int64");
  CHECK_EQ(fmt::format("{}", 32_fb), "bytes32");
  CHECK_EQ(fmt::format("{}", details::bool_t(true)), "bool");
  CHECK_EQ(fmt::format("{}", details::string_t("42")), "string");
  CHECK_EQ(fmt::format("{}", details::bytes_t{}), "bytes");
  CHECK_EQ(fmt::format("{}", details::address_t{}), "address");
}

TEST_CASE("format_variant")
{
  using namespace details::param_type_literals;
  details::param_type val;
  val = 64_i;
  CHECK_EQ(details::format(val), "int64");
  val = 256_u;
  CHECK_EQ(details::format(val), "uint256");
  auto ptr = details::make_shared(32_i);
  val = details::fixed_array_t<details::param_type>(ptr, 2);
  CHECK_EQ(details::format(val), "int32[2]");
  val = details::array_t<details::param_type>(ptr);
  CHECK_EQ(details::format(val), "int32[]");
  val = std::vector<details::param_type>{ 32_i, details::bool_t(false) };
  CHECK_EQ(details::format(val), "(int32,bool)");

  // complex
  auto array = details::array_t<details::param_type>(details::make_shared(details::bool_t(true)));
  ptr = details::make_shared(details::array_t<details::param_type>(array));
  CHECK_EQ(details::format(details::fixed_array_t<details::param_type>(ptr, 2)), "bool[][2]");

  // complex tupled
  auto tuple = std::vector<details::param_type>{ 256_i, 256_u };
  auto fixed_bytes = 32_fb;
  array = details::array_t<details::param_type>(details::make_shared(tuple));
  auto another_tuple = std::vector<details::param_type>{ array, fixed_bytes };
  val = details::array_t<details::param_type>(details::make_shared(another_tuple));
  CHECK_EQ(details::format(val), "((int256,uint256)[],bytes32)[]");
}

TEST_CASE("variant_type")
{
  using namespace details::param_type_literals;
  details::param_type val;
  val = details::int_t(1);
  CHECK_EQ(std::holds_alternative<details::int_t>(val), true);
  val = details::uint_t(2);
  CHECK_EQ(std::holds_alternative<details::uint_t>(val), true);
  val = details::address_t();
  CHECK_EQ(std::holds_alternative<details::address_t>(val), true);
  val = details::bool_t(true);
  CHECK_EQ(std::holds_alternative<details::bool_t>(val), true);
  val = 32_fb;
  CHECK_EQ(std::holds_alternative<details::fixed_bytes_t>(val), true);
  val = std::vector<details::param_type>{ 1_i, details::bool_t(false) };
  CHECK_EQ(std::holds_alternative<details::tuple_t<details::param_type>>(val), true);
  auto ptr = details::make_shared(1_i);
  val = details::array_t<details::param_type>(ptr);
  CHECK_EQ(std::holds_alternative<details::array_t<details::param_type>>(val), true);
  val = details::fixed_array_t<details::param_type>(ptr, 2);
  CHECK_EQ(std::holds_alternative<details::fixed_array_t<details::param_type>>(val), true);
  val = details::string_t("42");
  CHECK_EQ(std::holds_alternative<details::string_t>(val), true);
}

TEST_SUITE_END();
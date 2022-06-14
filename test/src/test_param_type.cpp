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

TEST_CASE("read_param")
{
  using namespace details::param_type_literals;
  auto functor = []<typename T>(T value, const std::string& name)
  {
    auto result = details::read(name);
    CHECK(result.has_value());
    CHECK_EQ(std::get<T>(result.value()), value);
  };
  functor(details::address_t(), "address");
  functor(details::bytes_t(), "bytes");
  functor(details::fixed_bytes_t(32), "bytes32");
  functor(details::bool_t(), "bool");
  functor(details::string_t(), "string");
  functor(details::int_t(256), "int");
  functor(details::uint_t(256), "uint");
  functor(details::uint_t(32), "uint32");
  functor(details::int_t(32), "int32");
}

TEST_CASE("read_array_param")
{
  using array_t = details::array_t<details::param_type>;
  auto functor = []<typename T>(T value, const std::string& name)
  {
    auto result = details::read(name);
    CHECK(result.has_value());
    details::param_type value_to_check = std::get<T>(result.value());
    CHECK_EQ(details::format(value), details::format(value_to_check));
  };
  functor(array_t(details::make_shared(details::address_t())), "address[]");
  functor(array_t(details::make_shared(details::uint_t(256))), "uint[]");
  functor(array_t(details::make_shared(details::bytes_t())), "bytes[]");
  functor(array_t(details::make_shared(array_t(details::make_shared(details::bool_t())))), "bool[][]");
}

TEST_CASE("read_fixed_array_param")
{
  using fixed_array_t = details::fixed_array_t<details::param_type>;
  auto functor = []<typename T>(T value, const std::string& name)
  {
    auto result = details::read(name);
    CHECK(result.has_value());
    details::param_type value_to_check = std::get<T>(result.value());
    CHECK_EQ(details::format(value), details::format(value_to_check));
  };
  functor(fixed_array_t(details::make_shared(details::address_t()), 2), "address[2]");
  functor(fixed_array_t(details::make_shared(details::bool_t()), 17), "bool[17]");
  functor(
      fixed_array_t(details::make_shared(fixed_array_t(details::make_shared(details::bytes_t()), 45)), 3), "bytes[45][3]");
}

TEST_CASE("read_mixed_array_param")
{
  using fixed_array_t = details::fixed_array_t<details::param_type>;
  using array_t = details::array_t<details::param_type>;
  auto functor = []<typename T>(T value, const std::string& name)
  {
    auto result = details::read(name);
    CHECK(result.has_value());
    details::param_type value_to_check = std::get<T>(result.value());
    CHECK_EQ(details::format(value), details::format(value_to_check));
  };

  functor(fixed_array_t(details::make_shared(array_t(details::make_shared(details::bool_t()))), 3), "bool[][3]");
  functor(array_t(details::make_shared(fixed_array_t(details::make_shared(details::bool_t()), 3))), "bool[3][]");
}

TEST_CASE("read_struct_param")
{
  using namespace details::param_type_literals;
  using fixed_array_t = details::fixed_array_t<details::param_type>;
  using tuple_t = details::tuple_t<details::param_type>;
  auto functor = []<typename T>(T value, const std::string& name)
  {
    auto result = details::read(name);
    CHECK(result.has_value());
    details::param_type value_to_check = std::get<T>(result.value());
    CHECK_EQ(details::format(value), details::format(value_to_check));
  };

  functor(tuple_t{ details::address_t{}, details::bool_t{} }, "(address,bool)");
  functor(tuple_t{ fixed_array_t(details::make_shared(details::bool_t{}), 3), 256_u }, "(bool[3],uint256)");
}

TEST_CASE("read_nested_struct_param")
{
  using namespace details::param_type_literals;
  using tuple_t = details::tuple_t<details::param_type>;
  auto functor = []<typename T>(T value, const std::string& name)
  {
    auto result = details::read(name);
    CHECK(result.has_value());
    details::param_type value_to_check = std::get<T>(result.value());
    CHECK_EQ(details::format(value), details::format(value_to_check));
  };

  functor(
      tuple_t{ details::address_t{}, details::bool_t{}, tuple_t{ details::bool_t{}, 256_u } },
      "(address,bool,(bool,uint256))");
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
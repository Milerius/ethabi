#include <doctest/doctest.h>

#include <iostream>

#include "ethabi/details/param_type.hpp"

using namespace ethabi;
using namespace ethabi::details::param_type_literals;
using array_t       = details::array_t<details::param_type>;
using fixed_array_t = details::fixed_array_t<details::param_type>;
using tuple_t       = details::tuple_t<details::param_type>;

namespace
{
    template <typename T>
    inline void
    check_format(T value, const std::string& name)
    {
        details::read(name).map([&value](auto&& val) { CHECK_EQ(details::format(val), details::format(value)); });
    }

    template <typename T>
    inline void
    check_eq(T value, const std::string& name)
    {
        details::read(name).map([&value](auto&& val) { CHECK_EQ(val, value); });
    }
} // namespace

TEST_SUITE_BEGIN("ethabi::param_type::test_suite");

TEST_CASE("format_type")
{
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
    check_eq(details::address_t(), "address");
    check_eq(details::bytes_t(), "bytes");
    check_eq(details::fixed_bytes_t(32), "bytes32");
    check_eq(details::bool_t(), "bool");
    check_eq(details::string_t(), "string");
    check_eq(details::int_t(256), "int");
    check_eq(details::uint_t(256), "uint");
    check_eq(details::uint_t(32), "uint32");
    check_eq(details::int_t(32), "int32");
}

TEST_CASE("read_array_param")
{
    check_format(array_t(details::make_shared(details::address_t())), "address[]");
    check_format(array_t(details::make_shared(details::uint_t(256))), "uint[]");
    check_format(array_t(details::make_shared(details::bytes_t())), "bytes[]");
    check_format(array_t(details::make_shared(array_t(details::make_shared(details::bool_t())))), "bool[][]");
}

TEST_CASE("read_fixed_array_param")
{
    check_format(fixed_array_t(details::make_shared(details::address_t()), 2), "address[2]");
    check_format(fixed_array_t(details::make_shared(details::bool_t()), 17), "bool[17]");
    check_format(fixed_array_t(details::make_shared(fixed_array_t(details::make_shared(details::bytes_t()), 45)), 3), "bytes[45][3]");
}

TEST_CASE("read_mixed_array_param")
{
    check_format(fixed_array_t(details::make_shared(array_t(details::make_shared(details::bool_t()))), 3), "bool[][3]");
    check_format(array_t(details::make_shared(fixed_array_t(details::make_shared(details::bool_t()), 3))), "bool[3][]");
}

TEST_CASE("read_struct_param")
{
    check_format(tuple_t{details::address_t{}, details::bool_t{}}, "(address,bool)");
    check_format(tuple_t{fixed_array_t(details::make_shared(details::bool_t{}), 3), 256_u}, "(bool[3],uint256)");
}

TEST_CASE("read_nested_struct_param")
{
    check_format(tuple_t{details::address_t{}, details::bool_t{}, tuple_t{details::bool_t{}, 256_u}}, "(address,bool,(bool,uint256))");
}

TEST_CASE("read_complex_nested_struct_param")
{
    check_format(
        tuple_t{
            details::address_t{}, details::bool_t{}, tuple_t{details::bool_t{}, 256_u, tuple_t{details::bool_t{}, 256_u}}, tuple_t{details::bool_t{}, 256_u}},
        "(address,bool,(bool,uint256,(bool,uint256)),(bool,uint256))");
}

TEST_CASE("read_inner_tuple_array_param")
{
    //
    check_format(array_t(details::make_shared(tuple_t{256_u, 32_fb})), "(uint256,bytes32)[]");
}

TEST_CASE("read_nested_tuple_array_param")
{
    //
    check_format(tuple_t{array_t(details::make_shared(tuple_t{256_u, 32_fb})), details::address_t{}}, "((uint256,bytes32)[],address)");
}

TEST_CASE("format_variant")
{
    details::param_type val;
    val = 64_i;
    CHECK_EQ(details::format(val), "int64");
    val = 256_u;
    CHECK_EQ(details::format(val), "uint256");
    auto ptr = details::make_shared(32_i);
    val      = fixed_array_t(ptr, 2);
    CHECK_EQ(details::format(val), "int32[2]");
    val = array_t(ptr);
    CHECK_EQ(details::format(val), "int32[]");
    val = tuple_t{32_i, details::bool_t(false)};
    CHECK_EQ(details::format(val), "(int32,bool)");

    // complex
    auto array = array_t(details::make_shared(details::bool_t(true)));
    ptr        = details::make_shared(array_t(array));
    CHECK_EQ(details::format(fixed_array_t(ptr, 2)), "bool[][2]");

    // complex tupled
    auto tuple         = tuple_t{256_i, 256_u};
    auto fixed_bytes   = 32_fb;
    array              = array_t(details::make_shared(tuple));
    auto another_tuple = tuple_t{array, fixed_bytes};
    val                = array_t(details::make_shared(another_tuple));
    CHECK_EQ(details::format(val), "((int256,uint256)[],bytes32)[]");
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
    val = 32_fb;
    CHECK_EQ(std::holds_alternative<details::fixed_bytes_t>(val), true);
    val = tuple_t{1_i, details::bool_t(false)};
    CHECK_EQ(std::holds_alternative<details::tuple_t<details::param_type>>(val), true);
    auto ptr = array_t(details::make_shared(1_i));
    val      = array_t(ptr);
    CHECK_EQ(std::holds_alternative<array_t>(val), true);
    val = fixed_array_t(ptr, 2);
    CHECK_EQ(std::holds_alternative<fixed_array_t>(val), true);
    val = details::string_t("42");
    CHECK_EQ(std::holds_alternative<details::string_t>(val), true);
}

TEST_SUITE_END();
#include <doctest/doctest.h>

#include <algorithm>
#include <unordered_set>

#include <ethcore/fixed_hash.hpp>

TEST_SUITE_BEGIN("ethcore::fixed_hash::test_suite");

using h256 = ethcore::fixed_hash<256 / 8>;

static_assert(sizeof(h256) == 256 / 8);

constexpr auto
get_sample_hash()
{
    std::array<std::byte, 256 / 8> arr{};
    std::generate(arr.begin(), arr.end(), [n = 0]() mutable { return std::byte(n++); });
    return h256(arr);
}

TEST_CASE("zero")
{
    constexpr h256 zero;

    CHECK(zero.is_zero());
    CHECK_EQ(zero.size(), 256 / 8);
    CHECK_EQ(zero.to_lower_hex(), "0000000000000000000000000000000000000000000000000000000000000000");
    CHECK_EQ(zero, zero);
    CHECK_EQ(zero, h256());
}

TEST_CASE("basic")
{
    auto hash = get_sample_hash();

    CHECK(not hash.is_zero());
    CHECK_EQ(hash.size(), 256 / 8);
    CHECK_EQ(hash, hash);
    CHECK_NE(hash, h256());
}

TEST_CASE("array")
{
    std::array<std::byte, 256 / 8> arr{};
    std::generate(arr.begin(), arr.end(), [n = 0]() mutable { return std::byte(n++); });
    h256 hash(arr);

    CHECK_EQ(hash.as_array(), arr);
    CHECK_EQ(hash.to_array(), arr);
}

TEST_CASE("to_hex")
{
    auto hash = get_sample_hash();

    CHECK_EQ(hash.to_lower_hex(), "000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f");
    CHECK_EQ(hash.to_upper_hex(), "000102030405060708090A0B0C0D0E0F101112131415161718191A1B1C1D1E1F");
}

TEST_CASE("bitwise")
{
    auto hash = get_sample_hash();

    CHECK_EQ(hash & hash, hash);
    CHECK_EQ(hash | hash, hash);
    CHECK_EQ(hash ^ hash, h256());
}

TEST_CASE("constexpr_bitwise")
{
    constexpr auto hash = get_sample_hash();
    constexpr auto zero = h256();

    CHECK_EQ(hash & hash, hash);
    CHECK_EQ(hash & zero, zero);
    CHECK_EQ(hash & ~zero, hash);
    CHECK_EQ(hash | hash, hash);
    CHECK_EQ(hash | zero, hash);
    CHECK_EQ(hash | ~zero, ~zero);
    CHECK_EQ(hash ^ hash, zero);
    CHECK_EQ(hash ^ zero, hash);
    CHECK_EQ(~zero ^ ~zero, zero);
}

TEST_CASE("covers")
{
    constexpr auto hash = get_sample_hash();

    CHECK(hash.covers(hash));
    CHECK(not hash.covers(h256()));
    CHECK(not hash.covers(~hash));
}

TEST_CASE("hash")
{
    constexpr auto hash   = get_sample_hash();
    auto           hasher = std::hash<h256>();

    CHECK_EQ(hasher(hash), hasher(hash));

    std::unordered_set<h256> s;
    s.emplace(hash);
    CHECK_EQ(s.size(), 1);
    CHECK_NE(s.find(hash), s.end());
    s.emplace(hash);
    CHECK_EQ(s.size(), 1);
    s.emplace(h256());
    CHECK_EQ(s.size(), 2);
    CHECK_NE(s.find(hash), s.end());
    CHECK_NE(s.find(h256()), s.end());
}

TEST_CASE("from_str")
{
    auto sample = get_sample_hash();
    auto zero   = h256();

    auto hash = h256::from_str(zero.to_lower_hex()).value();
    CHECK_EQ(hash, zero);

    hash = h256::from_str(sample.to_lower_hex()).value();
    CHECK_EQ(hash.to_lower_hex(), sample.to_lower_hex());

    hash = h256::from_str(std::string("0x") + sample.to_lower_hex()).value();
    CHECK_EQ(hash.to_lower_hex(), sample.to_lower_hex());

    hash = h256::from_str(sample.to_upper_hex()).value();
    CHECK_EQ(hash, sample);

    hash = h256::from_str(sample.to_upper_hex()).value();
    CHECK_EQ(hash, sample);

    auto result = h256::from_str("zzz");
    CHECK(not result);

    result = h256::from_str("000102030405060708090a0b0c0d0e0f");
    CHECK(not result);

    CHECK(ethcore::h160::from_str("0x71C7656EC7ab88b098defB751B7401B5f6d8976F").has_value());
}

TEST_SUITE_END();

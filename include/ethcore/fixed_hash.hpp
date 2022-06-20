#pragma once

#include <algorithm>
#include <array>
#include <charconv>
#include <concepts>
#include <cstddef>
#include <string>

#include <fmt/format.h>
#include <range/v3/algorithm/all_of.hpp>
#include <range/v3/range_concepts.hpp>
#include <range/v3/view/enumerate.hpp>
#include <range/v3/view/transform.hpp>
#include <tl/expected.hpp>

namespace ethcore
{

    namespace details
    {

        template <typename T, typename V>
        concept range_of = ranges::input_range<T> && std::same_as<V, ranges::range_value_t<T>>;
    }

    template <std::size_t Bytes>
    class fixed_hash
    {
      public:
        using storage = std::array<std::byte, Bytes>;

      private:
        storage bytes_;

      public:
        constexpr fixed_hash() noexcept { bytes_.fill(std::byte(0)); };

        explicit constexpr fixed_hash(const storage& bytes) noexcept : bytes_(bytes) {}

        template <details::range_of<std::byte> R>
        explicit constexpr fixed_hash(R&& r) noexcept
        {
            std::copy_n(r.begin(), Bytes, bytes_.begin());
        }

        template <details::range_of<std::uint8_t> R>
        explicit constexpr fixed_hash(R&& r) noexcept
        {
            fixed_hash(ranges::views::transform(r, [](auto&& cur) { return std::byte(cur); }));
        }

        static tl::expected<fixed_hash, std::errc>
        from_str(std::string_view view) noexcept
        {
            if (view.starts_with("0x"))
            {
                view.remove_prefix(2);
            }

            if (view.length() != Bytes * 2)
            {
                return tl::make_unexpected(std::errc::invalid_argument);
            }

            fixed_hash ret;
            for (std::size_t i = 0; i < Bytes; ++i)
            {
                std::uint8_t  value = 0;
                constexpr int base{16};
                const char*   expected_end = view.begin() + (i + 1) * 2;
                const char*   first        = view.begin() + i * 2;

                auto res = std::from_chars(first, expected_end, value, base);
                if (res.ptr != expected_end)
                {
                    return tl::make_unexpected(std::errc::invalid_argument);
                }
                ret.bytes_[i] = std::byte(value);
            }
            return ret;
        }

        [[nodiscard]] constexpr auto
        begin() const noexcept
        {
            return bytes_.begin();
        }

        [[nodiscard]] constexpr auto
        end() const noexcept
        {
            return bytes_.end();
        }

        [[nodiscard]] constexpr auto
        data() const noexcept
        {
            return bytes_.data();
        }

        [[nodiscard]] constexpr auto
        size() const noexcept
        {
            return bytes_.size();
        }

        [[nodiscard]] constexpr bool
        is_zero() const noexcept
        {
            return ranges::all_of(bytes_, [](auto&& value) { return std::to_integer<std::uint8_t>(value) == 0; });
        }

        [[nodiscard]] constexpr const storage&
        as_array() const noexcept
        {
            return bytes_;
        }

        [[nodiscard]] constexpr auto
        to_array() const noexcept
        {
            return bytes_;
        }

        [[nodiscard]] std::string
        to_lower_hex() const noexcept
        {
            return fmt::format("{:x}", *this);
        }

        [[nodiscard]] std::string
        to_upper_hex() const noexcept
        {
            return fmt::format("{:X}", *this);
        }

        friend inline constexpr auto operator<=>(const fixed_hash& lhs, const fixed_hash& rhs) noexcept = default;

        constexpr fixed_hash&
        operator&=(const fixed_hash& rhs) noexcept
        {
            for (std::size_t i = 0; i < size(); ++i) { bytes_[i] &= rhs.bytes_[i]; }
            return *this;
        }

        constexpr fixed_hash
        operator&(const fixed_hash& rhs) const noexcept
        {
            return fixed_hash(*this) &= rhs;
        }

        constexpr fixed_hash&
        operator|=(const fixed_hash& rhs) noexcept
        {
            for (std::size_t i = 0; i < size(); ++i) { bytes_[i] |= rhs.bytes_[i]; }
            return *this;
        }

        constexpr fixed_hash
        operator|(const fixed_hash& rhs) const noexcept
        {
            return fixed_hash(*this) |= rhs;
        }


        constexpr fixed_hash&
        operator^=(const fixed_hash& rhs) noexcept
        {
            for (std::size_t i = 0; i < size(); ++i) { bytes_[i] ^= rhs.bytes_[i]; }
            return *this;
        }

        constexpr fixed_hash
        operator^(const fixed_hash& rhs) const noexcept
        {
            return fixed_hash(*this) ^= rhs;
        }

        constexpr fixed_hash
        operator~() const noexcept
        {
            fixed_hash ret;
            for (std::size_t i = 0; i < size(); ++i) { ret.bytes_[i] = ~bytes_[i]; }
            return ret;
        }

        constexpr void
        clear() noexcept
        {
            bytes_.fill(std::byte(0));
        }

        [[nodiscard]] constexpr bool
        covers(const fixed_hash& rhs) const noexcept
        {
            return (*this & rhs) == *this;
        }
    };

    using h512 = ethcore::fixed_hash<512 / 8>;
    using h256 = ethcore::fixed_hash<256 / 8>;
    using h160 = ethcore::fixed_hash<160 / 8>;
} // namespace ethcore

template <typename std::size_t Bytes>
struct fmt::formatter<ethcore::fixed_hash<Bytes>>
{
    bool lower = true;

    constexpr auto
    parse(format_parse_context& ctx)
    {
        auto       it  = ctx.begin();
        const auto end = ctx.end();

        if (it != end && (*it == 'x' || *it == 'X'))
        {
            lower = *it++ == 'x';
        }

        if (it != end && *it != '}')
        {
            throw format_error("invalid format");
        }

        return it;
    }

    template <typename FormatContext>
    auto
    format(const ethcore::fixed_hash<Bytes>& hash, FormatContext& ctx) -> decltype(ctx.out())
    {
        auto out = ctx.out();

        if (lower)
        {
            for (auto&& b: hash) { out = format_to(out, "{:02x}", std::to_integer<std::uint8_t>(b)); }
        }
        else
        {
            for (auto&& b: hash) { out = format_to(out, "{:02X}", std::to_integer<std::uint8_t>(b)); }
        }

        return out;
    }
};

template <std::size_t Bytes>
struct std::hash<ethcore::fixed_hash<Bytes>>
{
    std::size_t
    operator()(const ethcore::fixed_hash<Bytes>& h) const noexcept
    {
        std::size_t result = 0;

        for (auto&& b: h)
        {
            auto cur_hash = std::hash<std::uint8_t>()(std::to_integer<std::uint8_t>(b));
            result ^= cur_hash + 0x9e3779b9 + (result << 6) + (result >> 2); // same as in boost::hash_combine
        }
        return result;
    }
};
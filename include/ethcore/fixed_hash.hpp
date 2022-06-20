#pragma once

#include <algorithm>
#include <array>
#include <charconv>
#include <cstddef>
#include <ranges>
#include <string>

#include <fmt/format.h>
#include <range/v3/view/enumerate.hpp>

namespace ethcore {

    namespace details {
        template<typename T, typename V>
        concept range_of = std::ranges::range<T> && std::same_as<V, std::ranges::range_value_t<T>>;
    }

    template<std::size_t Bytes>
    class fixed_hash {
    public:
        using storage = std::array<std::byte, Bytes>;

    private:
        storage bytes_;

    public:
        constexpr fixed_hash() noexcept {
            bytes_.fill(std::byte(0));
        };

        explicit constexpr fixed_hash(const storage &bytes) noexcept: bytes_(bytes) {
        }

        template<details::range_of<std::byte> R>
        explicit constexpr fixed_hash(R &&r) noexcept {
            std::copy_n(r.begin(), Bytes, bytes_.begin());
        }

        [[nodiscard]] constexpr auto begin() const noexcept {
            return bytes_.begin();
        }

        [[nodiscard]] constexpr auto end() const noexcept {
            return bytes_.end();
        }

        [[nodiscard]] constexpr auto data() const noexcept {
            return bytes_.data();
        }

        [[nodiscard]] constexpr auto size() const noexcept {
            return bytes_.size();
        }

        [[nodiscard]] constexpr bool is_zero() const noexcept {
            return std::ranges::all_of(bytes_, [](auto &&value) { return std::to_integer<unsigned char>(value) == 0; });
        }

        [[nodiscard]] constexpr const storage &as_array() const noexcept {
            return bytes_;
        }

        [[nodiscard]] constexpr auto to_array() const noexcept {
            return bytes_;
        }

        [[nodiscard]] std::string to_lower_hex() const noexcept {
            return fmt::format("{:x}", *this);
        }

        [[nodiscard]] std::string to_upper_hex() const noexcept {
            return fmt::format("{:X}", *this);
        }

        friend inline constexpr auto operator<=>(const fixed_hash &lhs, const fixed_hash &rhs) noexcept = default;

        constexpr fixed_hash &operator&=(const fixed_hash &rhs) noexcept {
            for (std::size_t i = 0; i < size(); ++i) {
                bytes_[i] &= rhs.bytes_[i];
            }
            return *this;
        }

        constexpr fixed_hash operator&(const fixed_hash &rhs) const noexcept {
            return fixed_hash(*this) &= rhs;
        }

        constexpr fixed_hash &operator|=(const fixed_hash &rhs) noexcept {
            for (std::size_t i = 0; i < size(); ++i) {
                bytes_[i] |= rhs.bytes_[i];
            }
            return *this;
        }

        constexpr fixed_hash operator|(const fixed_hash &rhs) const noexcept {
            return fixed_hash(*this) |= rhs;
        }


        constexpr fixed_hash &operator^=(const fixed_hash &rhs) noexcept {
            for (std::size_t i = 0; i < size(); ++i) {
                bytes_[i] ^= rhs.bytes_[i];
            }
            return *this;
        }

        constexpr fixed_hash operator^(const fixed_hash &rhs) const noexcept {
            return fixed_hash(*this) ^= rhs;
        }

        constexpr fixed_hash operator~() const noexcept {
            fixed_hash ret;
            for (std::size_t i = 0; i < size(); ++i) {
                ret.bytes_[i] = ~bytes_[i];
            }
            return ret;
        }

        constexpr void clear() noexcept {
            bytes_.fill(std::byte(0));
        }

        [[nodiscard]] constexpr bool covers(const fixed_hash &rhs) const noexcept {
            return (*this & rhs) == *this;
        }
    };
}

template<typename std::size_t Bytes>
struct fmt::formatter<ethcore::fixed_hash<Bytes>> {
    bool lower = true;

    constexpr auto parse(format_parse_context &ctx) {
        auto it = ctx.begin();
        const auto end = ctx.end();

        if (it != end && (*it == 'x' || *it == 'X')) {
            lower = *it++ == 'x';
        }

        if (it != end && *it != '}') {
            throw format_error("invalid format");
        }

        return it;
    }

    template<typename FormatContext>
    auto format(const ethcore::fixed_hash<Bytes> &hash, FormatContext &ctx) -> decltype(ctx.out()) {
        auto out = ctx.out();

        if (lower) {
            for (auto &&b: hash) {
                out = format_to(out, "{:02x}", std::to_integer<unsigned char>(b));
            }
        } else {
            for (auto &&b: hash) {
                out = format_to(out, "{:02X}", std::to_integer<unsigned char>(b));
            }
        }

        return out;
    }
};

template<std::size_t Bytes>
struct std::hash<ethcore::fixed_hash<Bytes>> {
    std::size_t operator()(const ethcore::fixed_hash<Bytes> &h) const noexcept {
        std::size_t result = 0;

        for (auto &&b: h) {
            auto cur_hash = std::hash<unsigned char>()(std::to_integer<unsigned char>(b));
            result ^= cur_hash + 0x9e3779b9 + (result << 6) + (result >> 2); // same as in boost::hash_combine
        }
        return result;
    }
};
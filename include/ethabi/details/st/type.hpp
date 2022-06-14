#pragma once

#include <type_traits>
#include <utility>

namespace st
{
    template <typename T>
    class type_base
    {
      public:
        explicit constexpr type_base() : _t() {}

        explicit constexpr type_base(const T& t) : _t(t) {}

        explicit constexpr type_base(T&& t) noexcept(std::is_nothrow_move_constructible_v<T>) : _t(std::move(t)) {}

        constexpr type_base(const type_base&) = default;

        constexpr type_base(type_base&&) noexcept(std::is_nothrow_move_constructible_v<T>) = default;

        constexpr type_base& operator=(const type_base&) = default;

        constexpr type_base& operator=(type_base&&) noexcept(std::is_nothrow_move_assignable_v<T>) = default;

        constexpr const T&
        value() const& noexcept
        {
            return _t;
        }

        constexpr T&
        value() & noexcept
        {
            return _t;
        }

        constexpr const T&&
        value() const&& noexcept
        {
            return std::move(_t);
        }

        constexpr T&&
        value() && noexcept
        {
            return std::move(_t);
        }

      private:
        T _t;
    };

    template <typename T, typename Tag, typename... Traits>
    class type : public Traits::template type<type<T, Tag, Traits...>>..., public type_base<T>
    {
      public:
        using type_base<T>::type_base;

        using value_type = T;
        using tag_type   = Tag;
    };
} // namespace st

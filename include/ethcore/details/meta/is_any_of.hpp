#pragma once

#include <concepts>

namespace meta::details
{
    template <typename T>
    struct tester
    {
        using tester_tag = T;
    };

    template <typename T, typename... Us>
    struct is_any_of : tester<Us>...
    {
        using this_type = is_any_of<T, Us...>;
        static constexpr void f(tester<T>);
        static constexpr int  f(...);
        static constexpr bool value = std::same_as<void, decltype(f(std::declval<this_type>()))>;
    };
} // namespace meta::details

namespace meta
{
    template <typename... Ts>
    constexpr bool is_any_of = details::is_any_of<Ts...>::value;
}
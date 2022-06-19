#pragma once

#include "./type.hpp"

namespace st
{
    namespace traits
    {
        template <typename T>
        struct is_strong_type_helper
        {
            template <typename WrappedT>
            static std::true_type  test(const type_base<WrappedT>&);
            static std::false_type test(...);

            using type = decltype(test(std::declval<const T&>()));
        };
    } // namespace traits

    template <typename T>
    struct is_strong_type : traits::is_strong_type_helper<T>::type
    {
    };

    template <typename T>
    inline constexpr const bool is_strong_type_v = is_strong_type<T>::value;
} // namespace st

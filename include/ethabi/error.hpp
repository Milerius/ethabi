#pragma once

#include <system_error>

namespace ethabi::error
{
    enum class param_type_read
    {
        invalid_name
    };

    std::error_code make_error_code(ethabi::error::param_type_read error);
} // namespace ethabi::error

namespace std
{
    template <>
    struct is_error_code_enum<ethabi::error::param_type_read> : true_type
    {
    };
} // namespace std
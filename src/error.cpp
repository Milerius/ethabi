//
// Created by sztergbaum roman on 13/06/2022.
//

#include "ethabi/error.hpp"

namespace ethabi::error
{
    struct param_type_read_category : std::error_category
    {
        [[nodiscard]] const char* name() const noexcept final;
        [[nodiscard]] std::string message(int error_value) const final;
    };

    std::string
    param_type_read_category::message(int error_value) const
    {
        switch (static_cast<param_type_read>(error_value))
        {
        case param_type_read::invalid_name:
            return "invalid_name";
        default:
            return "(unrecognized error)";
        }
    }

    const char*
    param_type_read_category::name() const noexcept
    {
        return "param_type";
    }

} // namespace ethabi::error

std::error_code
make_error_code(ethabi::error::param_type_read error)
{
    return {static_cast<int>(error), ethabi::error::param_type_read_category{}};
}
#pragma once

// C++ System headers
#include <string> // std::string

// Deps headers
#include <tl/expected.hpp>

#include "ethabi/details/param_type/param_type.hpp"
#include "ethabi/error.hpp"

namespace ethabi::details
{
    using read_result_t = tl::expected<param_type, ethabi::error::param_type_read>;

    [[maybe_unused]] read_result_t read(const std::string& name);
} // namespace ethabi::details

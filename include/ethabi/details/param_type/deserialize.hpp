#pragma once

#include <system_error>

#include <nlohmann/json_fwd.hpp>
#include <tl/expected.hpp>

#include "ethabi/details/param_type/param_type.hpp"

namespace ethabi::details
{
    using json         = nlohmann::json;
    using params       = details::tuple_t<param_type>;
    using deser_result = tl::expected<params, std::error_code>;
    deser_result deserialize(const json& input) noexcept;
} // namespace ethabi::details
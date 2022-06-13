#pragma once

// C++ System headers
#include <string>  // std::string

// Deps headers
#include <tl/expected.hpp>

#include "ethabi/error.hpp"

namespace ethabi::details
{
  using read_result_t = tl::expected<std::string, ethabi::error::param_type_read>;

  [[maybe_unused]] static read_result_t read(const std::string& name);
}  // namespace ethabi::details

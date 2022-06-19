//
// Created by sztergbaum roman on 15/06/2022.
//

#include "fmt/printf.h"
#include <nlohmann/json.hpp>

#include "ethabi/details/param_type/deserialize.hpp"
#include "ethabi/details/param_type/reader.hpp"

namespace
{
    class sax_consumer : public nlohmann::json::json_sax_t
    {
      public:
        using json   = nlohmann::json;
        using params = ethabi::details::params;
        bool
        null() final
        {
            return true;
        }

        bool
        boolean([[maybe_unused]] bool val) final
        {
            return true;
        }

        bool
        number_integer([[maybe_unused]] number_integer_t val) final
        {
            return true;
        }

        bool
        number_unsigned([[maybe_unused]] number_unsigned_t val) final
        {
            return true;
        }

        bool
        number_float([[maybe_unused]] number_float_t val, [[maybe_unused]] const string_t& s) final
        {
            return true;
        }

        bool
        string(string_t& val) final
        {
            if (auto result = ethabi::details::read(val); result)
            {
                m_value.emplace_back(result.value());
                return true;
            }
            return false;
        }

        bool
        start_object([[maybe_unused]] std::size_t elements) final
        {
            return true;
        }

        bool
        end_object() final
        {
            return true;
        }

        bool
        start_array([[maybe_unused]] std::size_t elements) final
        {
            return true;
        }

        bool
        end_array() final
        {
            return true;
        }

        bool
        key([[maybe_unused]] string_t& val) final
        {
            return true;
        }

        bool
        binary([[maybe_unused]] json::binary_t& val) final
        {
            return true;
        }

        bool
        parse_error([[maybe_unused]] std::size_t position, [[maybe_unused]] const std::string& last_token, [[maybe_unused]] const json::exception& ex) final
        {
            return false;
        }


        params
        get_params() const noexcept
        {
            return m_value;
        }

      private:
        params m_value{};
    };
} // namespace

namespace ethabi::details
{
    deser_result
    deserialize(const json& j) noexcept
    {
        sax_consumer sec;
        if (auto result = json::sax_parse(j.dump(), &sec); !result)
        {
            return tl::make_unexpected(error::param_type_read::unable_to_deserialize);
        }
        return sec.get_params();
    }
} // namespace ethabi::details
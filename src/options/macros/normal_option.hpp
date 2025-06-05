#pragma once

#include <algorithm>
#include <iosfwd>
#include <iostream>
#include <sstream>
#include <type_traits>

#include "option_base.hpp"

namespace Clockwork::options {
template<typename ParamType>
struct Option : OptionBase {
protected:
    ParamType value;
    ParamType default_value;
    ParamType min_value;
    ParamType max_value;
    bool      has_range;

public:
    Option(const std::string& name,
           ParamType          default_value,
           ParamType          min_value,
           ParamType          max_value) :
        OptionBase(name,
                   std::is_same_v<ParamType, float> ? OptionType::Float
                   : std::is_same_v<ParamType, int> ? OptionType::Int
                                                    : OptionType::String),
        value(default_value),
        default_value(default_value),
        min_value(min_value),
        max_value(max_value),
        has_range(true) {
    }

    Option(const std::string& name, ParamType default_value) :
        OptionBase(name,
                   std::is_same_v<ParamType, bool>          ? OptionType::Bool
                   : std::is_same_v<ParamType, std::string> ? OptionType::String
                                                            : OptionType::String),
        value(default_value),
        default_value(default_value),
        min_value(ParamType()),
        max_value(ParamType()),
        has_range(false) {
    }

    [[nodiscard]] bool set_from_string(const std::string& str) override {
        std::istringstream iss(str);

        if constexpr (std::is_same_v<ParamType, float>) {
            int x;

            if (!(iss >> x)) {
                return false;
            }

            if (x < static_cast<int>(min_value * 100) || x > static_cast<int>(max_value * 100)) {
                return false;
            }

            value = static_cast<float>(x) / 100.0f;
            return true;
        } else if constexpr (std::is_same_v<ParamType, int>) {
            int x;

            if (!(iss >> x)) {
                return false;
            }

            if (x < min_value || x > max_value) {
                return false;
            }

            value = x;
            return true;
        } else if constexpr (std::is_same_v<ParamType, bool>) {
            std::string str_copy = str;
            std::transform(str_copy.begin(), str_copy.end(), str_copy.begin(),
                           [](unsigned char character) {
                               return std::tolower(character);
                           });

            if (str_copy == "true") {
                value = true;
                return true;
            }

            if (str_copy == "false") {
                value = false;
                return true;
            }

            return false;
        } else if constexpr (std::is_same_v<ParamType, std::string>) {
            value = str;
            return true;
        }
        return false;
    }

    [[nodiscard]] std::string default_as_string(bool float_as_int) const override {
        if constexpr (std::is_same_v<ParamType, float>) {
            std::ostringstream output;
            auto dw = float_as_int ? static_cast<int>(default_value * 100) : default_value;
            output << dw;
            return output.str();
        }
        if constexpr (std::is_same_v<ParamType, int>) {
            std::ostringstream output;
            output << default_value;
            return output.str();
        }
        if constexpr (std::is_same_v<ParamType, bool>) {
            return default_value ? "true" : "false";
        }
        if constexpr (std::is_same_v<ParamType, std::string>) {
            return default_value;
        }
        return "";
    }

    [[nodiscard]] std::string uci_type_name() const override {
        if constexpr (std::is_same_v<ParamType, float> || std::is_same_v<ParamType, int>) {
            return "spin";
        }
        if constexpr (std::is_same_v<ParamType, bool>) {
            return "check";
        }
        if constexpr (std::is_same_v<ParamType, std::string>) {
            return "string";
        }
        return "";
    }

    void print_option_line() const override {
        std::cout << "option name " << get_option_name() << " type " << uci_type_name()
                  << " default " << default_as_string(true);
        if constexpr (std::is_same_v<ParamType, float>) {
            std::ostringstream output_min, output_max;
            output_min << static_cast<int>(min_value * 100);
            output_max << static_cast<int>(max_value * 100);

            std::cout << " min " << output_min.str() << " max " << output_max.str();
        }
        if constexpr (std::is_same_v<ParamType, int>) {
            std::ostringstream output_min, output_max;
            output_min << min_value;
            output_max << max_value;
            std::cout << " min " << output_min.str() << " max " << output_max.str();
        }
        std::cout << "\n";
    }

    ParamType get() const {
        return value;
    }
};
}
#pragma once

#include <format>

#include "normal_option.hpp"

namespace Clockwork::options {
template<typename ParamType>
struct TunableOption final : Option<ParamType> {
private:
    ParamType c;
    float     r;

public:
    TunableOption(const std::string& name,
                  ParamType          default_value,
                  ParamType          min_value,
                  ParamType          max_value,
                  ParamType          c,
                  const float        r) :
        Option<ParamType>(name, default_value, min_value, max_value),
        c(c),
        r(r) {
    }

    [[nodiscard]] bool set_from_string(const std::string& s) override {
        return Option<ParamType>::set_from_string(s);
    }

    void print_option_line() const override {
        //add to only do it when there is correct compile flag
        Option<ParamType>::print_option_line();
    }

    void print_tunable_line() const override {
        std::string type = "";
        if constexpr (std::is_same_v<ParamType, float>) {
            type = "float";
        } else if constexpr (std::is_same_v<ParamType, int>) {
            type = "int";
        }

        std::cout << std::format("{}, {}, {}, {}, {}, {}, {}", this->get_option_name(), type,
                                 this->default_as_string(false), this->min_value, this->max_value,
                                 this->c, this->r)
                  << "\n";
    }
};
}

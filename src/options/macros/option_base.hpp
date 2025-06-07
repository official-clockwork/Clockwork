#pragma once

#include <string>

namespace Clockwork::options {
static std::string make_pretty(std::string_view str);

enum class OptionType {
    Float,
    Int,
    Bool,
    String
};

struct OptionBase {
protected:
    std::string option_name;
    OptionType  option_type;

public:
    OptionBase(const std::string& name, const OptionType type) :
        option_name(make_pretty(name)),
        option_type(type) {
    }

    virtual ~OptionBase() = default;

    [[nodiscard]] std::string get_option_name() const {
        return option_name;
    }
    [[nodiscard]] virtual bool        set_from_string(const std::string& val)    = 0;
    [[nodiscard]] virtual std::string default_as_string(bool float_as_int) const = 0;
    [[nodiscard]] virtual std::string uci_type_name() const                      = 0;
    virtual void                      print_option_line() const                  = 0;
    virtual void                      print_tunable_line() const {
    }
};

static std::string make_pretty(const std::string_view str) {
    std::string out;
    out.reserve(str.size());
    bool capitalize_next = true;
    for (const char character : str) {
        if (character == '_') {
            out.push_back('_');
            capitalize_next = true;
        } else {
            if (capitalize_next) {
                out.push_back(static_cast<char>(std::toupper(character)));
                capitalize_next = false;
            } else {
                out.push_back(character);
            }
        }
    }
    return out;
}
}
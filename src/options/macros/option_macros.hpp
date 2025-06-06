#pragma once

#include <vector>

#include "option_base.hpp"
#include "tunable_option.hpp"

namespace Clockwork {
namespace Options {
inline std::vector<options::OptionBase*>& all_options() {
    static std::vector<options::OptionBase*> values;
    return values;
}

inline std::vector<options::OptionBase*>& all_tunables() {
    static std::vector<options::OptionBase*> values;
    return values;
}

inline void register_option(options::OptionBase* o) {
    all_options().push_back(o);
}

inline void register_tunable(options::OptionBase* o) {
    all_tunables().push_back(o);
}

inline bool set(const std::string& name, const std::string& value) {
    for (auto* option : all_options()) {
        if (option->get_option_name() == name) {
            return option->set_from_string(value);
        }
    }
    for (auto* option : all_tunables()) {
        if (option->get_option_name() == name) {
            return option->set_from_string(value);
        }
    }
    return false;
}

inline void print_all_options() {
    for (const auto* option : all_options()) {
        option->print_option_line();
    }
    for (const auto* option : all_tunables()) {
        option->print_option_line();
    }
}

inline void print_all_tunables() {
    for (const auto* option : all_tunables()) {
        option->print_tunable_line();
    }
}
}

#define PARAM_FLOAT(internal, default_value, min_value, max_value) \
    static options::Option<float> internal##_opt(#internal, default_value, min_value, max_value); \
    inline float                  internal() { \
        return internal##_opt.get(); \
    }

#define PARAM_INT(internal, default_value, min_value, max_value) \
    static options::Option<int> internal##_opt(#internal, default_value, min_value, max_value); \
    inline int                  internal() { \
        return internal##_opt.get(); \
    }

#define PARAM_BOOL(internal, default_value) \
    static options::Option<bool> internal##_opt(#internal, default_value); \
    inline bool                  internal() { \
        return internal##_opt.get(); \
    }

#define PARAM_STRING(internal, default_value) \
    static options::Option<std::string> internal##_opt(#internal, default_value); \
    inline const std::string&           internal() { \
        return internal##_opt.get(); \
    }

#define TUNE_PARAM_FLOAT(internal, default_value, min_value, max_value, c, r) \
    static options::TunableOption<float> internal##_opt(#internal, default_value, min_value, \
                                                        max_value, c, r); \
    inline float                         internal() { \
        return internal##_opt.get(); \
    }

#define TUNE_PARAM_INT(internal, default_value, min_value, max_value, c, r) \
    static options::TunableOption<int> internal##_opt(#internal, default_value, min_value, \
                                                      max_value, c, r); \
    inline int                         internal() { \
        return internal##_opt.get(); \
    }

#include "../engine_options.inc"

#undef PARAM_FLOAT
#undef PARAM_INT
#undef PARAM_BOOL
#undef PARAM_STRING
#undef TUNE_PARAM_FLOAT
#undef TUNE_PARAM_INT

struct _OptionRegisterer {
    explicit _OptionRegisterer(options::OptionBase* option, bool is_tunable) {
        if (is_tunable) {
            Options::register_tunable(option);
        } else {
            Options::register_option(option);
        }
    }
};

#define PARAM_FLOAT(internal, default_value, min_value, max_value) \
    static _OptionRegisterer _##internal##_reg(&internal##_opt, false);

#define PARAM_INT(internal, default_value, min_value, max_value) \
    static _OptionRegisterer _##internal##_reg(&internal##_opt, false);

#define PARAM_BOOL(internal, default_value) \
    static _OptionRegisterer _##internal##_reg(&internal##_opt, false);

#define PARAM_STRING(internal, default_value) \
    static _OptionRegisterer _##internal##_reg(&internal##_opt, false);

#define TUNE_PARAM_FLOAT(internal, default_value, min_value, max_value, c, r) \
    static _OptionRegisterer _##internal##_reg(&internal##_opt, true);

#define TUNE_PARAM_INT(internal, default_value, min_value, max_value, c, r) \
    static _OptionRegisterer _##internal##_reg(&internal##_opt, true);

#include "../engine_options.inc"

#undef PARAM_FLOAT
#undef PARAM_INT
#undef PARAM_BOOL
#undef PARAM_STRING
#undef TUNE_PARAM_FLOAT
#undef TUNE_PARAM_INT
}
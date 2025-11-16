#include "tuned.hpp"
#include "util/parse.hpp"
#include <algorithm>
#include <iostream>
#include <string_view>

namespace Clockwork::tuned {

void uci_print_tunable_options() {
#if CLOCKWORK_IS_TUNING
    #define TUNE(NAME, DEFAULTVAL, MINVAL, MAXVAL, ...)                                          \
        std::cout << "option name tune_" #NAME " type spin default " #DEFAULTVAL " min " #MINVAL \
                     " max " #MAXVAL                                                             \
                  << std::endl;
    #define NO_TUNE(...) /* do nothing */
    CLOCKWORK_TUNABLES(TUNE, NO_TUNE)
    #undef TUNE
    #undef NO_TUNE
#endif
}

void uci_print_tunable_values() {
#define TUNE(NAME, DEFAULTVAL, MINVAL, MAXVAL, CEND, REND)                                \
    std::cout << "tune_" #NAME ", int, " #DEFAULTVAL ", " #MINVAL ", " #MAXVAL ", " #CEND \
                 ", " #REND                                                               \
              << std::endl;
#define NO_TUNE(...) /* do nothing */
    CLOCKWORK_TUNABLES(TUNE, NO_TUNE)
#undef TUNE
#undef NO_TUNE
}

bool uci_parse_tunable([[maybe_unused]] std::string_view name,
                       [[maybe_unused]] std::string_view value_str) {
#if CLOCKWORK_IS_TUNING
    #define TUNE(NAME, DEFAULTVAL, MINVAL, MAXVAL, ...)         \
        if (name == "tune_" #NAME) {                            \
            if (auto value = parse_number<i32>(value_str)) {    \
                NAME = std::clamp<i32>(*value, MINVAL, MAXVAL); \
                return true;                                    \
            } else {                                            \
                return false;                                   \
            }                                                   \
        }
    #define NO_TUNE(...) /* do nothing */
    CLOCKWORK_TUNABLES(TUNE, NO_TUNE)
    #undef TUNE
    #undef NO_TUNE
#endif
    return false;
}
}  // namespace Clockwork::tuned

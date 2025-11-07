#pragma once

#include "util/types.hpp"
#include <string_view>

#ifndef CLOCKWORK_IS_TUNING
    #define CLOCKWORK_IS_TUNING 0
#endif

namespace Clockwork::tuned {

#define CLOCKWORK_TUNABLES(TUNE, NO_TUNE)                         \
                                                                  \
    /* RFP Values */                                              \
    TUNE(rfp_margin, 147, 40, 160, 4, 0.002)                      \
    NO_TUNE(rfp_depth, 7, 4, 10, .5, 0.002)                       \
                                                                  \
    /* NMP Values */                                              \
    NO_TUNE(nmp_depth, 3, 1, 10, .5, 0.002)                       \
    NO_TUNE(nmp_base_r, 3, 1, 10, .5, 0.002)                      \
    NO_TUNE(nmp_verif_min_depth, 14, 1, 40, .5, 0.002)            \
                                                                  \
    /* SEE Values */                                              \
    TUNE(quiesce_see_threshold, 0, -1000, 100, 20, 0.002)         \
    TUNE(movepicker_see_capthist_divisor, 48, 16, 192, 10, 0.002) \
                                                                  \
    /* End of Tunables */

#define DEFINE_VARIABLE(NAME, DEFAULT, ...) inline i32 NAME = DEFAULT;
#define DEFINE_CONSTANT(NAME, DEFAULT, ...) constexpr i32 NAME = DEFAULT;

#if CLOCKWORK_IS_TUNING
// TUNEs are defined as variables, NO_TUNEs are defined as constexpr constants.
CLOCKWORK_TUNABLES(DEFINE_VARIABLE, DEFINE_CONSTANT)
#else
// Both TUNEs and NO_TUNEs are defined as constexpr constants.
CLOCKWORK_TUNABLES(DEFINE_CONSTANT, DEFINE_CONSTANT)
#endif

#undef DEFINE_VARIABLE
#undef DEFINE_CONSTANT

void uci_print_tunable_options();
void uci_print_tunable_values();
bool uci_parse_tunable(std::string_view name, std::string_view value);

}  // namespace Clockwork::tuned

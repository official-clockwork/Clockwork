#pragma once

#include "util/types.hpp"
#include <string_view>

// #define CLOCKWORK_IS_TUNING

#ifndef CLOCKWORK_IS_TUNING
    #define CLOCKWORK_IS_TUNING 0
#endif

namespace Clockwork::tuned {

#define CLOCKWORK_TUNABLES(TUNE, NO_TUNE)                         \
                                                                  \
    /* RFP Values */                                              \
    TUNE(rfp_margin, 144, 40, 160, 4, 0.002)                      \
    NO_TUNE(rfp_depth, 7, 4, 10, .5, 0.002)                       \
                                                                  \
    /* NMP Values */                                              \
    NO_TUNE(nmp_depth, 3, 1, 10, .5, 0.002)                       \
    NO_TUNE(nmp_base_r, 192, 64, 384, 16, 0.002)                  \
    TUNE(nmp_depth_r, 16, 8, 32, 1, 0.002)                       \
    NO_TUNE(nmp_verif_min_depth, 14, 1, 40, .5, 0.002)            \
    TUNE(nmp_beta_margin, 30, 10, 60, 3, 0.002)                   \
    TUNE(nmp_beta_diff, 428, 200, 800, 38, 0.002)                 \
    TUNE(nmp_improving_r, 64, 32, 128, 5, 0.002)                  \
                                                                  \
    /* ProbCut Values */                                          \
    TUNE(probcut_margin, 332, 166, 664, 25, 0.002)                \
    TUNE(probcut_see, 108, 54, 200, 10, 0.002)                     \
    NO_TUNE(probcut_min_depth, 5, 1, 20, 0.5, 0.002)              \
                                                                  \
    /* SEE Values */                                              \
    TUNE(quiesce_see_threshold, 18, -1000, 100, 20, 0.002)        \
    TUNE(movepicker_see_capthist_divisor, 54, 16, 192, 10, 0.002) \
    TUNE(see_pawn_val, 100, 50, 200, 8, 0.002)                    \
    TUNE(see_knight_val, 300, 150, 600, 23, 0.002)                \
    TUNE(see_bishop_val, 300, 150, 600, 23, 0.002)                \
    TUNE(see_rook_val, 500, 250, 1000, 38, 0.002)                 \
    TUNE(see_queen_val, 900, 450, 1800, 68, 0.002)                \
                                                                  \
    /* Stat Bonus */                                              \
    TUNE(stat_bonus_max, 1828, 948, 3792, 142, 0.002)             \
    TUNE(stat_bonus_quad, 4, 2, 8, .5, 0.002)                     \
    TUNE(stat_bonus_lin, 118, 60, 240, 9, 0.002)                  \
    TUNE(stat_bonus_sub, 123, 60, 240, 9, 0.002)                  \
                                                                  \
    /* Stat Malus */                                              \
    TUNE(stat_malus_max, 1627, 948, 3792, 142, 0.002)             \
    TUNE(stat_malus_quad, 4, 2, 8, .5, 0.002)                     \
    TUNE(stat_malus_lin, 137, 60, 240, 9, 0.002)                  \
    TUNE(stat_malus_sub, 111, 60, 240, 9, 0.002)                  \
                                                                  \
    /* Search Params */                                           \
    TUNE(asp_window_delta, 32, 25, 100, 4, 0.002)                 \
    NO_TUNE(razor_depth, 7, 1, 20, 0.5, 0.002)                    \
    TUNE(razor_margin, 681, 353, 1414, 53, 0.002)                 \
    NO_TUNE(lmp_depth_mult, 3, 1, 20, 0.5, 0.002)                 \
                                                                  \
    /* Futility Pruning */                                        \
    TUNE(ffp_margin_base, 437, 250, 1000, 38, 0.002)              \
    TUNE(ffp_margin_mult, 89, 50, 200, 8, 0.002)                  \
    TUNE(ffp_hist_div, 25, 16, 64, 3, 0.002)                      \
    NO_TUNE(ffp_depth, 8, 1, 20, 0.5, 0.002)                      \
                                                                  \
    /* Quiet History Pruning */                                   \
    NO_TUNE(qhp_depth, 4, 1, 20, 0.5, 0.002)                      \
    TUNE(qhp_threshold, -2183, -4096, -1024, 154, 0.002)          \
                                                                  \
    /* SEE PVS */                                                 \
    TUNE(see_pvs_quiet, -65, -134, -33, 5, 0.002)                 \
    TUNE(see_pvs_noisy_quad, -20, -44, -11, 2, 0.002)             \
    TUNE(see_pvs_hist_mult, 16, 10, 40, 2, 0.002)                 \
                                                                  \
    /* Singular Extensions */                                     \
    NO_TUNE(sing_min_depth, 6, 1, 20, 0.5, 0.002)                 \
    NO_TUNE(sing_depth_margin, 3, 1, 20, 0.5, 0.002)              \
    TUNE(sing_beta_margin, 320, 160, 640, 19, 0.002)              \
    TUNE(dext_margin, 40, 20, 80, 3, 0.002)                       \
    TUNE(dext_hist_div, 491, 256, 1024, 39, 0.002)                \
    TUNE(triext_margin, 126, 60, 240, 9, 0.002)                   \
    TUNE(triext_hist_div, 573, 256, 1024, 39, 0.002)              \
                                                                  \
    /* LMR */                                                     \
    TUNE(lmr_quiet_base, 638, 394, 1576, 59, 0.002)               \
    TUNE(lmr_quiet_div, 164, 104, 416, 16, 0.002)                 \
    TUNE(lmr_noisy_base, 258, 128, 512, 20, 0.002)                \
    TUNE(lmr_noisy_div, 193, 98, 394, 15, 0.002)                  \
    TUNE(lmr_pv_node_red, 1207, 512, 2048, 77, 0.002)             \
    TUNE(lmr_alpha_raise_red, 514, 256, 1024, 38, 0.002)          \
    TUNE(lmr_not_improving_red, 520, 256, 1024, 38, 0.002)        \
    TUNE(lmr_in_check_red, 1021, 512, 2048, 77, 0.002)            \
    TUNE(lmr_cutnode_red, 1087, 512, 2048, 77, 0.002)             \
    TUNE(lmr_no_tt_red, 997, 512, 2048, 77, 0.002)                \
    TUNE(lmr_ttpv_red, 1097, 512, 2048, 77, 0.002)                \
    TUNE(lmr_tt_capture_red, 968, 512, 2048, 77, 0.002)           \
    TUNE(lmr_fail_high_red, 980, 512, 2048, 77, 0.002)            \
    TUNE(lmr_quiet_hist_base, 940, 512, 2048, 77, 0.002)          \
    TUNE(lmr_hist_div, 12, 4, 16, 2, 0.002)                       \
    TUNE(lmr_fut_red_base, 562, 250, 1000, 38, 0.002)             \
    TUNE(lmr_fut_red_mult, 99, 50, 200, 8, 0.002)                 \
    TUNE(lmr_fut_red, 864, 512, 2048, 77, 0.002)                  \
    TUNE(lmr_max_red, 2985, 1536, 6144, 231, 0.002)               \
                                                                  \
    /* TIME MANAGEMENT */                                         \
    TUNE(time_hard_limit, 256, 128, 512, 19, 0.002)               \
    TUNE(time_soft_limit, 51, 25, 100, 3, 0.002)                  \
    TUNE(time_soft_increment, 512, 256, 1024, 38, 0.002)          \
    TUNE(nodetm_min_factor, 512, 256, 1024, 38, 0.002)            \
    TUNE(nodetm_avg_factor, 2048, 1024, 4096, 153, 0.002)         \
    TUNE(nodetm_frac_factor, 1895, 948, 3792, 142, 0.002)         \
    TUNE(d1plexity_base, 788, 394, 1576, 59, 0.002)               \
    TUNE(d1plexity_max_complexity, 200, 100, 400, 15, 0.002)      \
    TUNE(d1plexity_divisor, 386, 193, 772, 29, 0.002)             \
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

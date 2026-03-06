#pragma once

#include "util/types.hpp"
#include <string_view>

#define CLOCKWORK_IS_TUNING 0

#ifndef CLOCKWORK_IS_TUNING
    #define CLOCKWORK_IS_TUNING 0
#endif

namespace Clockwork::tuned {

#define CLOCKWORK_TUNABLES(TUNE, NO_TUNE)                         \
                                                                  \
    /* RFP Values */                                              \
    TUNE(rfp_margin, 136, 40, 160, 4, 0.002)                      \
    NO_TUNE(rfp_depth, 7, 4, 10, .5, 0.002)                       \
                                                                  \
    /* NMP Values */                                              \
    NO_TUNE(nmp_depth, 3, 1, 10, .5, 0.002)                       \
    NO_TUNE(nmp_base_r, 192, 64, 384, 16, 0.002)                  \
    TUNE(nmp_depth_r, 17, 8, 32, 1, 0.002)                       \
    NO_TUNE(nmp_verif_min_depth, 14, 1, 40, .5, 0.002)            \
    TUNE(nmp_beta_margin, 33, 10, 60, 3, 0.002)                   \
    TUNE(nmp_beta_diff, 392, 200, 800, 38, 0.002)                 \
    TUNE(nmp_improving_r, 62, 32, 128, 5, 0.002)                  \
                                                                  \
    /* ProbCut Values */                                          \
    TUNE(probcut_margin, 341, 166, 664, 25, 0.002)                \
    TUNE(probcut_see, 114, 54, 200, 10, 0.002)                     \
    NO_TUNE(probcut_min_depth, 5, 1, 20, 0.5, 0.002)              \
                                                                  \
    /* SEE Values */                                              \
    TUNE(quiesce_see_threshold, 25, -1000, 200, 20, 0.002)        \
    TUNE(movepicker_see_capthist_divisor, 51, 16, 192, 10, 0.002) \
    TUNE(see_pawn_val, 98, 50, 200, 8, 0.002)                    \
    TUNE(see_knight_val, 314, 150, 600, 23, 0.002)                \
    TUNE(see_bishop_val, 283, 150, 600, 23, 0.002)                \
    TUNE(see_rook_val, 567, 250, 1000, 38, 0.002)                 \
    TUNE(see_queen_val, 947, 450, 1800, 68, 0.002)                \
                                                                  \
    /* Stat Bonus */                                              \
    TUNE(stat_bonus_max, 1883, 948, 3792, 142, 0.002)             \
    TUNE(stat_bonus_quad, 4, 2, 8, .5, 0.002)                     \
    TUNE(stat_bonus_lin, 92, 60, 240, 9, 0.002)                  \
    TUNE(stat_bonus_sub, 149, 60, 240, 9, 0.002)                  \
                                                                  \
    /* Stat Malus */                                              \
    TUNE(stat_malus_max, 1051, 948, 3792, 142, 0.002)             \
    TUNE(stat_malus_quad, 3, 2, 8, .5, 0.002)                     \
    TUNE(stat_malus_lin, 123, 60, 240, 9, 0.002)                  \
    TUNE(stat_malus_sub, 118, 60, 240, 9, 0.002)                  \
                                                                  \
    /* Search Params */                                           \
    TUNE(asp_window_delta, 26, 25, 100, 4, 0.002)                 \
    NO_TUNE(razor_depth, 7, 1, 20, 0.5, 0.002)                    \
    TUNE(razor_margin, 660, 353, 1414, 53, 0.002)                 \
    NO_TUNE(lmp_depth_mult, 3, 1, 20, 0.5, 0.002)                 \
                                                                  \
    /* Futility Pruning */                                        \
    TUNE(ffp_margin_base, 423, 250, 1000, 38, 0.002)              \
    TUNE(ffp_margin_mult, 93, 50, 200, 8, 0.002)                  \
    TUNE(ffp_hist_div, 21, 16, 64, 3, 0.002)                      \
    NO_TUNE(ffp_depth, 8, 1, 20, 0.5, 0.002)                      \
                                                                  \
    /* Quiet History Pruning */                                   \
    NO_TUNE(qhp_depth, 4, 1, 20, 0.5, 0.002)                      \
    TUNE(qhp_threshold, -2127, -4096, -1024, 154, 0.002)          \
                                                                  \
    /* SEE PVS */                                                 \
    TUNE(see_pvs_quiet, -58, -134, -33, 5, 0.002)                 \
    TUNE(see_pvs_noisy_quad, -12, -44, -11, 2, 0.002)             \
    TUNE(see_pvs_hist_mult, 17, 10, 40, 2, 0.002)                 \
                                                                  \
    /* Singular Extensions */                                     \
    NO_TUNE(sing_min_depth, 6, 1, 20, 0.5, 0.002)                 \
    NO_TUNE(sing_depth_margin, 3, 1, 20, 0.5, 0.002)              \
    TUNE(sing_beta_margin, 252, 160, 640, 19, 0.002)              \
    TUNE(dext_margin, 38, 20, 80, 3, 0.002)                       \
    TUNE(dext_hist_div, 494, 256, 1024, 39, 0.002)                \
    TUNE(triext_margin, 119, 60, 240, 9, 0.002)                   \
    TUNE(triext_hist_div, 536, 256, 1024, 39, 0.002)              \
                                                                  \
    /* LMR */                                                     \
    TUNE(lmr_quiet_base, 605, 394, 1576, 59, 0.002)               \
    TUNE(lmr_quiet_div, 173, 104, 416, 16, 0.002)                 \
    TUNE(lmr_noisy_base, 242, 128, 512, 20, 0.002)                \
    TUNE(lmr_noisy_div, 227, 98, 394, 15, 0.002)                  \
    TUNE(lmr_pv_node_red, 1315, 512, 2048, 77, 0.002)             \
    TUNE(lmr_alpha_raise_red, 530, 256, 1024, 38, 0.002)          \
    TUNE(lmr_not_improving_red, 548, 256, 1024, 38, 0.002)        \
    TUNE(lmr_in_check_red, 1045, 512, 2048, 77, 0.002)            \
    TUNE(lmr_cutnode_red, 1267, 512, 2048, 77, 0.002)             \
    TUNE(lmr_no_tt_red, 928, 512, 2048, 77, 0.002)                \
    TUNE(lmr_ttpv_red, 992, 512, 2048, 77, 0.002)                \
    TUNE(lmr_tt_capture_red, 1017, 512, 2048, 77, 0.002)           \
    TUNE(lmr_fail_high_red, 943, 512, 2048, 77, 0.002)            \
    TUNE(lmr_quiet_hist_base, 874, 512, 2048, 77, 0.002)          \
    TUNE(lmr_hist_div, 14, 4, 16, 2, 0.002)                       \
    TUNE(lmr_fut_red_base, 537, 250, 1000, 38, 0.002)             \
    TUNE(lmr_fut_red_mult, 108, 50, 200, 8, 0.002)                 \
    TUNE(lmr_fut_red, 650, 512, 2048, 77, 0.002)                  \
    TUNE(lmr_max_red, 3187, 1536, 6144, 231, 0.002)               \
                                                                  \
    /* TIME MANAGEMENT */                                         \
    TUNE(time_hard_limit, 287, 128, 512, 19, 0.002)               \
    TUNE(time_soft_limit, 50, 25, 100, 3, 0.002)                  \
    TUNE(time_soft_increment, 554, 256, 1024, 38, 0.002)          \
    TUNE(nodetm_min_factor, 411, 256, 1024, 38, 0.002)            \
    TUNE(nodetm_avg_factor, 2358, 1024, 4096, 153, 0.002)         \
    TUNE(nodetm_frac_factor, 2201, 948, 3792, 142, 0.002)         \
    TUNE(d1plexity_base, 965, 394, 1576, 59, 0.002)               \
    TUNE(d1plexity_max_complexity, 222, 100, 400, 15, 0.002)      \
    TUNE(d1plexity_divisor, 405, 193, 772, 29, 0.002)             \
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

#include "tm.hpp"
#include "uci.hpp"
#include "util/types.hpp"
#include <iostream>

namespace Clockwork::TM {
time::TimePoint compute_hard_limit(time::TimePoint               search_start,
                                   const Search::SearchSettings& settings,
                                   const Color                   stm) {
    using namespace std;
    using namespace time;

    auto hard_limit = TimePoint::max();

    if (settings.w_time >= 0) {
        const auto compute_buffer_time = [&]() -> i64 {
            if (stm == Color::White) {
                return settings.w_time / 4;
            } else {
                return settings.b_time / 4;
            }
        };
        hard_limit = min(hard_limit, search_start + Milliseconds(compute_buffer_time()));
    }

    if (settings.move_time >= 0) {
        hard_limit = min(hard_limit, search_start + Milliseconds(settings.move_time));
    }

    return hard_limit - UCI_LATENCY;
}

template<bool ADJUST_FOR_NODES_TM>
time::TimePoint compute_soft_limit(time::TimePoint               search_start,
                                   const Search::SearchSettings& settings,
                                   const Color                   stm,
                                   const f64                     nodes_tm_fraction,
                                   const f64                     complexity) {
    using namespace std;
    using namespace time;

    auto soft_limit = TimePoint::max();

    if (settings.w_time >= 0) {

        // Base time calculation
        const auto compute_buffer_time = [&]() -> f64 {
            if (stm == Color::White) {
                return static_cast<f64>(settings.w_time / 20 + settings.w_inc / 2);
            } else {
                return static_cast<f64>(settings.b_time / 20 + settings.b_inc / 2);
            }
        };

        // Adjustment based on mainline nodes vs total nodes
        const auto compute_nodestm_factor = [&]() -> f64 {
            if constexpr (!ADJUST_FOR_NODES_TM) {
                return 1.0;
            }
            return std::max<f64>(0.5, 2.0 - nodes_tm_fraction * (100.0 / 54.038));
        };

        // Adjustment based on difference between depth 1 search score and current score
        // This essentially estimates the complexity of a position
        const auto compute_complexitytm_factor = [&]() -> f64 {
            return std::max<f64>(0.77 + std::clamp<f64>(complexity, 0.0, 200.0) / 386.0, 1.0);
        };

        soft_limit =
          min(soft_limit,
              search_start
                + Milliseconds(static_cast<i64>(compute_buffer_time() * compute_nodestm_factor()
                                                * compute_complexitytm_factor())));
    }

    return soft_limit;
}

// Explicit instantiations
template time::TimePoint compute_soft_limit<true>(
  time::TimePoint, const Search::SearchSettings&, const Color, const f64, const f64);

template time::TimePoint compute_soft_limit<false>(
  time::TimePoint, const Search::SearchSettings&, const Color, const f64, const f64);
}  // namespace Clockwork::TM

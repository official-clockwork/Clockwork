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
        const auto compute_buffer_time = [&]() -> u64 {
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

time::TimePoint compute_soft_limit(time::TimePoint               search_start,
                                   const Search::SearchSettings& settings,
                                   const Color                   stm) {
    using namespace std;
    using namespace time;

    auto soft_limit = TimePoint::max();

    if (settings.w_time >= 0) {
        const auto compute_buffer_time = [&]() -> u64 {
            if (stm == Color::White) {
                return settings.w_time / 20 + settings.w_inc / 2;
            } else {
                return settings.b_time / 20 + settings.b_inc / 2;
            }
        };
        soft_limit = min(soft_limit, search_start + Milliseconds(compute_buffer_time()));
    }

    return soft_limit;
}

}

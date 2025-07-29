#include "tm.hpp"
#include "uci.hpp"
#include "util/types.hpp"
#include <iostream>

namespace Clockwork::TM {
time::TimePoint compute_hard_limit(time::TimePoint            search_start,
                                   const UCI::SearchSettings& settings,
                                   const Color                stm) {
    using namespace std;
    using namespace time;

    auto hard_limit = TimePoint::max();

    if (settings.w_time >= 0) {
        const auto compute_buffer_time = [&]() -> u64 {
            if (stm == Color::White) {
                return settings.w_time / 20 + settings.w_inc / 2;
            } else {
                return settings.b_time / 20 + settings.b_inc / 2;
            }
        };
        hard_limit = min(hard_limit, search_start + Milliseconds(compute_buffer_time()));
    }

    if (settings.move_time >= 0) {
        hard_limit = min(hard_limit, search_start + Milliseconds(settings.move_time));
    }

    return hard_limit - UCI_LATENCY;
}

}

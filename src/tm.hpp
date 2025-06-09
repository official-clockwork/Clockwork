#pragma once
#include "uci.hpp"
#include "util/types.hpp"

namespace Clockwork::TM {
constexpr time::Milliseconds UCI_LATENCY(50);
time::TimePoint              compute_hard_limit(time::TimePoint            search_start,
                                                const UCI::SearchSettings& settings,
                                                const Color                stm);
// Will add soft tm and other helper functions here
}

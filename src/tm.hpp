#pragma once
#include "search.hpp"
#include "uci.hpp"
#include "util/types.hpp"

namespace Clockwork::TM {
constexpr time::Milliseconds UCI_LATENCY(50);
time::TimePoint              compute_hard_limit(time::TimePoint               search_start,
                                                const Search::SearchSettings& settings,
                                                const Color                   stm);
time::TimePoint              compute_soft_limit(time::TimePoint               search_start,
                                                const Search::SearchSettings& settings,
                                                const Color                   stm);
// Will add soft tm and other helper functions here
}

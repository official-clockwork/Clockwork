#pragma once

#include "search.hpp"
#include "uci.hpp"
#include "util/types.hpp"

namespace Clockwork::TM {
constexpr time::Milliseconds UCI_LATENCY(50);
time::TimePoint              compute_hard_limit(time::TimePoint               search_start,
                                                const Search::SearchSettings& settings,
                                                const Color                   stm);
template<bool ADJUST_FOR_NODES_TM = true>
time::TimePoint compute_soft_limit(time::TimePoint               search_start,
                                   const Search::SearchSettings& settings,
                                   const Color                   stm,
                                   const f64                     nodes_tm_fraction,
                                   const f64                     complexity);
// Will add soft tm and other helper functions here
}  // namespace Clockwork::TM

#pragma once

#include "eval_types.hpp"
#include "position.hpp"
#include "psqt_state.hpp"
#include <array>

namespace Clockwork {

Score evaluate_white_pov(const Position& pos, const PsqtState& psqt_state);
Score evaluate_stm_pov(const Position& pos, const PsqtState& psqt_state);

inline Score evaluate_white_pov(const Position& pos) {
    return evaluate_white_pov(pos, PsqtState{pos});
}

inline Score evaluate_stm_pov(const Position& pos) {
    return evaluate_stm_pov(pos, PsqtState{pos});
}

};  // namespace Clockwork

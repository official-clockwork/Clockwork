#include "psqt_state.hpp"

namespace Clockwork {

PScore PsqtState::score(const Position& pos) const {
    return m_accumulators[static_cast<usize>(Color::White)][pos.king_side(Color::White)]
         - m_accumulators[static_cast<usize>(Color::Black)][pos.king_side(Color::Black)];
}

}

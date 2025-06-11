#include "movepick.hpp"

namespace Clockwork {

void MovePicker::skip_quiets() {
    m_skip_quiets = true;
    if (m_stage == Stage::EmitQuiet) {
        m_stage = Stage::End;
    }
}

Move MovePicker::next() {
    switch (m_stage) {
    case Stage::GenerateMoves:
        generate_moves();

        m_stage         = Stage::EmitNoisy;
        m_current_index = 0;

        [[fallthrough]];
    case Stage::EmitNoisy:
        if (m_current_index < m_noisy.size()) {
            return m_noisy[m_current_index++];
        }

        if (m_skip_quiets) {
            m_stage = Stage::End;
            return Move::none();
        }

        m_stage         = Stage::EmitQuiet;
        m_current_index = 0;

        [[fallthrough]];
    case Stage::EmitQuiet:
        if (m_current_index < m_quiet.size()) {
            return m_quiet[m_current_index++];
        }

        m_stage = Stage::End;

        [[fallthrough]];
    case Stage::End:
        return Move::none();
    }
}

void MovePicker::generate_moves() {
    m_movegen.generate_moves(m_noisy, m_quiet);
}

}

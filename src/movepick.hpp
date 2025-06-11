#pragma once

#include <array>
#include <optional>

#include "movegen.hpp"
#include "position.hpp"

namespace Clockwork {

bool quiet_move(Move move);

class MovePicker {
public:
    explicit MovePicker(const Position& pos, Move tt_move = Move::none()) :
        m_pos(pos),
        m_movegen(pos),
        m_tt_move(tt_move) {
    }

    void skip_quiets();

    Move next();

private:
    enum class Stage {
        GenerateMoves,
        EmitTTMove,
        EmitNoisy,
        EmitQuiet,
        End,
    };

    void generate_moves();

    Stage m_stage = Stage::GenerateMoves;

    const Position& m_pos;
    MoveGen         m_movegen;
    MoveList        m_noisy;
    MoveList        m_quiet;
    usize           m_current_index = 0;
    bool            m_skip_quiets   = false;

    Move m_tt_move;
};

}

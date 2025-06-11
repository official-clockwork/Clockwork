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
        ScoreNoisy,
        EmitNoisy,
        EmitQuiet,
        End,
    };

    void generate_moves();
    Move pick_next(MoveList& moves);
    void score_moves(MoveList& moves);

    i32 score_move(Move move) const;

    Stage m_stage = Stage::GenerateMoves;

    const Position&      m_pos;
    MoveGen              m_movegen;
    MoveList             m_noisy;
    MoveList             m_quiet;
    usize                m_current_index = 0;
    bool                 m_skip_quiets   = false;
    std::array<i32, 256> m_scores;

    Move m_tt_move;
};

}

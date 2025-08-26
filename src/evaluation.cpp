#include <ranges>

#include "common.hpp"
#include "position.hpp"

#include "evaluation.hpp"

#include "eval_types.hpp"
#include "square.hpp"

namespace Clockwork {

const PScore PAWN_MAT     = S(212, 423);
const PScore KNIGHT_MAT   = S(856, 781);
const PScore BISHOP_MAT   = S(965, 785);
const PScore ROOK_MAT     = S(1094, 1441);
const PScore QUEEN_MAT    = S(2667, 2573);
const PScore MOBILITY_VAL = S(15, 28);
const PScore TEMPO_VAL    = S(13, -8);

const std::array<PScore, 48> PAWN_PSQT = {
    S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0),
    S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0),
    S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0),
    S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0),
    S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0),
    S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0),
};

const std::array<PScore, 64> KNIGHT_PSQT = {
    S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0),
    S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0),
    S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0),
    S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0),
    S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0),
    S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0),
    S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0),
    S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0),
};

const std::array<PScore, 64> BISHOP_PSQT = {
    S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0),
    S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0),
    S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0),
    S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0),
    S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0),
    S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0),
    S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0),
    S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0),
};

const std::array<PScore, 64> ROOK_PSQT = {
    S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0),
    S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0),
    S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0),
    S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0),
    S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0),
    S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0),
    S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0),
    S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0),
};

const std::array<PScore, 64> QUEEN_PSQT = {
    S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0),
    S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0),
    S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0),
    S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0),
    S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0),
    S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0),
    S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0),
    S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0),
};

const std::array<PScore, 64> KING_PSQT = {
    S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0),
    S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0),
    S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0),
    S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0),
    S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0),
    S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0),
    S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0),
    S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0),
};

Score evaluate_white_pov(Position pos) {

    const Color us    = pos.active_color();
    i32         phase = pos.piece_count(Color::White, PieceType::Knight)
              + pos.piece_count(Color::Black, PieceType::Knight)
              + pos.piece_count(Color::White, PieceType::Bishop)
              + pos.piece_count(Color::Black, PieceType::Bishop)
              + 2
                  * (pos.piece_count(Color::White, PieceType::Rook)
                     + pos.piece_count(Color::Black, PieceType::Rook))
              + 4
                  * (pos.piece_count(Color::White, PieceType::Queen)
                     + pos.piece_count(Color::Black, PieceType::Queen));

    phase = std::min<i32>(phase, 24);

    PScore material = PAWN_MAT
                      * (pos.piece_count(Color::White, PieceType::Pawn)
                         - pos.piece_count(Color::Black, PieceType::Pawn))
                    + KNIGHT_MAT
                        * (pos.piece_count(Color::White, PieceType::Knight)
                           - pos.piece_count(Color::Black, PieceType::Knight))
                    + BISHOP_MAT
                        * (pos.piece_count(Color::White, PieceType::Bishop)
                           - pos.piece_count(Color::Black, PieceType::Bishop))
                    + ROOK_MAT
                        * (pos.piece_count(Color::White, PieceType::Rook)
                           - pos.piece_count(Color::Black, PieceType::Rook))
                    + QUEEN_MAT
                        * (pos.piece_count(Color::White, PieceType::Queen)
                           - pos.piece_count(Color::Black, PieceType::Queen));

    i32 mob_count = 0;
    for (u64 x : std::bit_cast<std::array<u64, 16>>(pos.attack_table(Color::White))) {
        mob_count += std::popcount(x);
    }
    for (u64 x : std::bit_cast<std::array<u64, 16>>(pos.attack_table(Color::Black))) {
        mob_count -= std::popcount(x);
    }

    PScore psqt = PSCORE_ZERO;

for (Color c : { Color::White, Color::Black }) {
    auto& pieces  = pos.piece_list(c);
    auto& squares = pos.piece_list_sq(c);

    for (size_t i = 0; i < 16; ++i) {
        PieceType pt = pieces[i];
        if (pt == PieceType::None)
            continue;

        u8 sq = squares[i].raw;

        // Mirror board for White
        if (c == Color::White)
            sq ^= 56;

        switch (pt) {
            case PieceType::Pawn:
                psqt = psqt + (c == Color::White ? PAWN_PSQT[sq - 8] : -PAWN_PSQT[sq - 8]);
                break;
            case PieceType::Knight:
            psqt = psqt + (c == Color::White ? KNIGHT_PSQT[sq] : -KNIGHT_PSQT[sq]);
                break;
            case PieceType::Bishop:
            psqt = psqt + (c == Color::White ? BISHOP_PSQT[sq] : -BISHOP_PSQT[sq]);
                break;
            case PieceType::Rook:
            psqt = psqt + (c == Color::White ? ROOK_PSQT[sq] : -ROOK_PSQT[sq]);
                break;
            case PieceType::Queen:
            psqt = psqt + (c == Color::White ? QUEEN_PSQT[sq] : -QUEEN_PSQT[sq]);
                break;
            case PieceType::King:
            psqt = psqt + (c == Color::White ? KING_PSQT[sq] : -KING_PSQT[sq]);
                break;
            default:
                break;
        }        
    }
}

    PScore mobility = MOBILITY_VAL * mob_count;

    PScore tempo = (us == Color::White) ? TEMPO_VAL : -TEMPO_VAL;
    PScore sum   = material + mobility + tempo + psqt;
#ifdef EVAL_TUNING
    return sum->phase<24.0>(static_cast<f64>(phase));
#else
    return sum.phase<24>(phase);
#endif
};

Score evaluate_stm_pov(Position pos) {
    const Color us = pos.active_color();
    return (us == Color::White) ? evaluate_white_pov(pos) : -evaluate_white_pov(pos);
}

}  // namespace Clockwork

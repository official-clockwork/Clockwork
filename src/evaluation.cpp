#include <array>
#include <ranges>

#include "bitboard.hpp"
#include "common.hpp"
#include "eval_constants.hpp"
#include "position.hpp"
#include "psqt_state.hpp"

#include "evaluation.hpp"

#include "eval_types.hpp"
#include "square.hpp"

namespace Clockwork {

static i32 chebyshev_distance(Square a, Square b) {
    i32 file_dist = std::abs(a.file() - b.file());
    i32 rank_dist = std::abs(a.rank() - b.rank());
    return std::max(file_dist, rank_dist);
}

std::array<Bitboard, 64> king_ring_table = []() {
    std::array<Bitboard, 64> king_ring_table{};
    for (u8 sq_idx = 0; sq_idx < 64; sq_idx++) {
        Bitboard sq_bb     = Bitboard::from_square(Square{sq_idx});
        Bitboard king_ring = sq_bb;
        king_ring |= sq_bb.shift(Direction::North);
        king_ring |= sq_bb.shift(Direction::South);
        king_ring |= sq_bb.shift(Direction::East);
        king_ring |= sq_bb.shift(Direction::West);
        king_ring |= sq_bb.shift(Direction::NorthEast);
        king_ring |= sq_bb.shift(Direction::SouthEast);
        king_ring |= sq_bb.shift(Direction::NorthWest);
        king_ring |= sq_bb.shift(Direction::SouthWest);
        king_ring_table[sq_idx] = king_ring;
    }
    return king_ring_table;
}();

std::array<std::array<Bitboard, 64>, 2> passed_pawn_spans = []() {
    std::array<std::array<Bitboard, 64>, 2> passed_pawn_masks{};
    for (Color color : {Color::White, Color::Black}) {
        for (u8 sq_idx = 0; sq_idx < 64; sq_idx++) {
            Bitboard sq_bb = Bitboard::from_square(Square{sq_idx});
            Bitboard mask  = sq_bb.shift_relative(color, Direction::North);
            mask |= mask.shift(Direction::East) | mask.shift(Direction::West);
            for (u8 i = 0; i < 8; i++) {
                mask |= mask.shift_relative(color, Direction::North);
            }
            passed_pawn_masks[static_cast<usize>(color)][sq_idx] = mask;
        }
    }
    return passed_pawn_masks;
}();

template<Color color>
PScore evaluate_pawns(const Position& pos) {
    constexpr i32   RANK_2 = 1;
    constexpr i32   RANK_3 = 2;
    constexpr Color them   = color == Color::White ? Color::Black : Color::White;

    Bitboard pawns      = pos.board().bitboard_for(color, PieceType::Pawn);
    Bitboard opp_pawns  = pos.board().bitboard_for(~color, PieceType::Pawn);
    Square   our_king   = pos.king_sq(color);
    Square   their_king = pos.king_sq(them);
    PScore   eval       = PSCORE_ZERO;

    eval += DOUBLED_PAWN_VAL * static_cast<i32>((pawns & pawns.shift(Direction::North)).popcount());

    for (Square sq : pawns) {
        Square   push     = sq.push<color>();
        Bitboard stoppers = opp_pawns & passed_pawn_spans[static_cast<usize>(color)][sq.raw];
        if (stoppers.empty()) {
            eval += PASSED_PAWN[sq.relative_sq(color).rank() - RANK_2];
            if (pos.attack_table(color).read(push).popcount()
                > pos.attack_table(them).read(push).popcount()) {
                eval += DEFENDED_PASSED_PUSH[sq.relative_sq(color).rank() - RANK_2];
            }
            if (pos.piece_at(push) != PieceType::None) {
                eval += BLOCKED_PASSED_PAWN[sq.relative_sq(color).rank() - RANK_2];
            }

            i32 our_king_dist   = chebyshev_distance(our_king, sq);
            i32 their_king_dist = chebyshev_distance(their_king, sq);

            eval += FRIENDLY_KING_PASSED_PAWN_DISTANCE[our_king_dist];
            eval += ENEMY_KING_PASSED_PAWN_DISTANCE[their_king_dist];
        }
    }

    Bitboard phalanx = pawns & pawns.shift(Direction::East);
    for (Square sq : phalanx) {
        eval += PAWN_PHALANX[static_cast<usize>(sq.relative_sq(color).rank() - RANK_2)];
    }

    Bitboard defended = pawns & pos.attacked_by(color, PieceType::Pawn);
    for (Square sq : defended) {
        eval += DEFENDED_PAWN[sq.relative_sq(color).rank() - RANK_3];
    }

    return eval;
}

template<Color color>
PScore evaluate_pieces(const Position& pos) {
    constexpr Color opp  = ~color;
    PScore          eval = PSCORE_ZERO;
    Bitboard bb = pos.bitboard_for(color, PieceType::Pawn) | pos.attacked_by(opp, PieceType::Pawn);
    Bitboard opp_king_ring = king_ring_table[pos.king_sq(opp).raw];
    for (PieceId id : pos.get_piece_mask(color, PieceType::Knight)) {
        eval += KNIGHT_MOBILITY[pos.mobility_of(color, id, ~bb)];
        eval += KNIGHT_KING_RING[pos.mobility_of(color, id, opp_king_ring)];
    }
    for (PieceId id : pos.get_piece_mask(color, PieceType::Bishop)) {
        eval += BISHOP_MOBILITY[pos.mobility_of(color, id, ~bb)];
        eval += BISHOP_KING_RING[pos.mobility_of(color, id, opp_king_ring)];
    }
    for (PieceId id : pos.get_piece_mask(color, PieceType::Rook)) {
        eval += ROOK_MOBILITY[pos.mobility_of(color, id, ~bb)];
        eval += ROOK_KING_RING[pos.mobility_of(color, id, opp_king_ring)];
    }
    for (PieceId id : pos.get_piece_mask(color, PieceType::Queen)) {
        eval += QUEEN_MOBILITY[pos.mobility_of(color, id, ~bb)];
        eval += QUEEN_KING_RING[pos.mobility_of(color, id, opp_king_ring)];
    }
    eval += KING_MOBILITY[pos.mobility_of(color, PieceId::king(), ~bb)];

    if (pos.piece_count(color, PieceType::Bishop) >= 2) {
        eval += BISHOP_PAIR_VAL;
    }

    return eval;
}

template<Color color>
PScore evaluate_potential_checkers(const Position& pos) {
    constexpr Color opp = ~color;

    const PieceMask orth   = pos.get_piece_mask<PieceType::Rook, PieceType::Queen>(opp);
    const PieceMask diag   = pos.get_piece_mask<PieceType::Bishop, PieceType::Queen>(opp);
    const PieceMask knight = pos.get_piece_mask<PieceType::Knight>(opp);

    CreateSuperpieceMaskInfo cmi;
    cmi.knight     = knight.value();
    cmi.orth       = orth.value();
    cmi.orth_near  = orth.value();
    cmi.wpawn_near = diag.value();
    cmi.bpawn_near = diag.value();
    cmi.diag       = diag.value();

    Wordboard mask = pos.create_attack_table_superpiece_mask(pos.king_sq(color), cmi);
    mask           = mask & pos.attack_table(opp);
    return POTENTIAL_CHECKER_VAL * mask.popcount();
}

template<Color color>
PScore evaluate_threats(const Position& pos) {
    constexpr Color opp  = ~color;
    PScore          eval = PSCORE_ZERO;

    Bitboard pawn_attacks = pos.attacked_by(color, PieceType::Pawn);
    eval +=
      PAWN_THREAT_KNIGHT * (pos.bitboard_for(opp, PieceType::Knight) & pawn_attacks).popcount();
    eval +=
      PAWN_THREAT_BISHOP * (pos.bitboard_for(opp, PieceType::Bishop) & pawn_attacks).popcount();
    eval += PAWN_THREAT_ROOK * (pos.bitboard_for(opp, PieceType::Rook) & pawn_attacks).popcount();
    eval += PAWN_THREAT_QUEEN * (pos.bitboard_for(opp, PieceType::Queen) & pawn_attacks).popcount();

    Bitboard knight_attacks = pos.attacked_by(color, PieceType::Knight);
    eval +=
      KNIGHT_THREAT_BISHOP * (pos.bitboard_for(opp, PieceType::Bishop) & knight_attacks).popcount();
    eval +=
      KNIGHT_THREAT_ROOK * (pos.bitboard_for(opp, PieceType::Rook) & knight_attacks).popcount();
    eval +=
      KNIGHT_THREAT_QUEEN * (pos.bitboard_for(opp, PieceType::Queen) & knight_attacks).popcount();

    Bitboard bishop_attacks = pos.attacked_by(color, PieceType::Bishop);
    eval +=
      BISHOP_THREAT_KNIGHT * (pos.bitboard_for(opp, PieceType::Knight) & bishop_attacks).popcount();
    eval +=
      BISHOP_THREAT_ROOK * (pos.bitboard_for(opp, PieceType::Rook) & bishop_attacks).popcount();
    eval +=
      BISHOP_THREAT_QUEEN * (pos.bitboard_for(opp, PieceType::Queen) & bishop_attacks).popcount();

    return eval;
}

Score evaluate_white_pov(const Position& pos, const PsqtState& psqt_state) {
    const Color us    = pos.active_color();
    usize       phase = pos.piece_count(Color::White, PieceType::Knight)
                + pos.piece_count(Color::Black, PieceType::Knight)
                + pos.piece_count(Color::White, PieceType::Bishop)
                + pos.piece_count(Color::Black, PieceType::Bishop)
                + 2
                    * (pos.piece_count(Color::White, PieceType::Rook)
                       + pos.piece_count(Color::Black, PieceType::Rook))
                + 4
                    * (pos.piece_count(Color::White, PieceType::Queen)
                       + pos.piece_count(Color::Black, PieceType::Queen));

    phase = std::min<usize>(phase, 24);

    PScore eval = psqt_state.score();
    eval += evaluate_pieces<Color::White>(pos) - evaluate_pieces<Color::Black>(pos);
    eval += evaluate_pawns<Color::White>(pos) - evaluate_pawns<Color::Black>(pos);
    eval += evaluate_potential_checkers<Color::White>(pos)
          - evaluate_potential_checkers<Color::Black>(pos);
    eval += evaluate_threats<Color::White>(pos) - evaluate_threats<Color::Black>(pos);
    eval += (us == Color::White) ? TEMPO_VAL : -TEMPO_VAL;
    return eval->phase<24>(static_cast<i32>(phase));
};

Score evaluate_stm_pov(const Position& pos, const PsqtState& psqt_state) {
    const Color us = pos.active_color();
    return (us == Color::White) ? evaluate_white_pov(pos, psqt_state)
                                : -evaluate_white_pov(pos, psqt_state);
}

}  // namespace Clockwork

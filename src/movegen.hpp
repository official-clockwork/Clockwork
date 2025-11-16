#pragma once

#include "bitboard.hpp"
#include "board.hpp"
#include "move.hpp"
#include "position.hpp"
#include "square.hpp"
#include "util/static_vector.hpp"
#include <tuple>

namespace Clockwork {

using MoveList = StaticVector<Move, 256>;

class MoveGen {
public:
    explicit MoveGen(const Position& position) :
        m_active_color(position.active_color()),
        m_position(position) {
        std::tie(m_pin_mask, m_pinned) = m_position.calc_pin_mask();
    }

    [[nodiscard]] bool is_legal(Move m) const;

    void generate_moves(MoveList& noisy, MoveList& quiet);

private:
    [[nodiscard]] std::tuple<Bitboard, Bitboard, bool>
                           valid_destinations_one_checker(PieceMask checker) const;
    [[nodiscard]] Bitboard valid_destinations_two_checkers(PieceMask checker) const;

    [[nodiscard]] bool is_legal_no_checkers(Move m, Bitboard valid_dests, bool can_ep) const;
    [[nodiscard]] bool is_legal_king_move(Move m, Bitboard valid_dests) const;
    [[nodiscard]] bool is_legal_one_checker(Move m, PieceMask checkers) const;
    [[nodiscard]] bool is_legal_two_checkers(Move m, PieceMask checkers) const;

    [[nodiscard]] bool is_aside_castling_legal(Bitboard empty, Bitboard danger) const;
    [[nodiscard]] bool is_hside_castling_legal(Bitboard empty, Bitboard danger) const;

    template<bool king_moves>
    void generate_moves_to(MoveList& noisy, MoveList& quiet, Bitboard valid_dests, bool can_ep);
    void generate_king_moves_to(MoveList& noisy, MoveList& quiet, Bitboard valid_dests);

    void generate_moves_one_checker(MoveList& noisy, MoveList& quiet, PieceMask checker);
    void generate_moves_two_checkers(MoveList& noisy, MoveList& quiet, PieceMask checkers);

    // Write moves that go to dest. Source are pieces in piecemask.
    void write(MoveList& moves, Square dest, PieceMask piecemask, MoveFlags mf);

    // Write moves that go to all squares in dest_bb. Possible sources are pieces in piecemask, masked as appropriate by attack_table.
    void write(MoveList&                        moves,
               const std::array<PieceMask, 64>& attack_table,
               Bitboard                         dest_bb,
               PieceMask                        piecemask,
               MoveFlags                        mf);

    // Write quiet pawn moves that start from src_bb, moved by shift.
    void write_pawn(MoveList& moves, Bitboard src_bb, i32 shift, MoveFlags mf);

    [[nodiscard]] bool is_ep_clearance_pinned(PieceMask ep_attackers_mask) const;

    Color           m_active_color;
    const Position& m_position;
    Wordboard       m_pin_mask;
    Bitboard        m_pinned;
};
}  // namespace Clockwork

#pragma once

#include "bitboard.hpp"
#include "move.hpp"
#include "position.hpp"
#include "square.hpp"
#include "util/static_vector.hpp"

namespace Clockwork {

using MoveList = StaticVector<Move, 256>;

class MoveGen {
public:
    explicit MoveGen(const Position& position) :
        m_active_color(position.active_color()),
        m_position(position) {
    }

    void generate_moves(MoveList& noisy, MoveList& quiet);

private:
    template<bool king_moves>
    void generate_moves_to(MoveList& noisy, MoveList& quiet, Bitboard valid_dests, bool can_ep);
    void generate_king_moves_to(MoveList& noisy, MoveList& quiet, Bitboard valid_dests);

    void generate_moves_one_checker(MoveList& noisy, MoveList& quiet, u16 checker);
    void generate_moves_two_checkers(MoveList& noisy, MoveList& quiet, u16 checkers);

    // Write moves that go to dest. Source are pieces in piecemask.
    void write(MoveList& moves, Square dest, u16 piecemask, MoveFlags mf);

    // Write moves that go to all squares in dest_bb. Possible sources are pieces in piecemask, masked as appropriate by attack_table.
    void write(MoveList&                  moves,
               const std::array<u16, 64>& attack_table,
               Bitboard                   dest_bb,
               u16                        piecemask,
               MoveFlags                  mf);

    // Write quiet pawn moves that start from src_bb, moved by shift.
    void write_pawn(MoveList& moves, Bitboard src_bb, i32 shift, MoveFlags mf);

    [[nodiscard]] bool is_ep_clearance_pinned(u16 ep_attackers_mask) const;

    Color           m_active_color;
    const Position& m_position;
};

}

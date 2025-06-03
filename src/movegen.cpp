#include "movegen.hpp"

#include <bit>
#include <tuple>

#include "common.hpp"
#include "move.hpp"
#include "position.hpp"
#include "util/types.hpp"

namespace Clockwork {

static std::tuple<u64, u64, u64, int, int> validPawns(Color color, u64 bb, u64 empty, u64 dests) {
    switch (color)
    {
    case Color::white : {
        const u64 single = bb & ((empty & dests) >> 8);
        return {single & 0x0000FFFFFFFFFF00,
                bb & (empty >> 8) & ((empty & dests) >> 16) & 0x000000000000FF00,
                single & 0x00FF000000000000, 8, 16};
    }
    case Color::black : {
        const u64 single = bb & ((empty & dests) << 8);
        return {single & 0x00FFFFFFFFFF0000,
                bb & (empty << 8) & ((empty & dests) << 16) & 0x00FF000000000000,
                single & 0x000000000000FF00, -8, -16};
    }
    }
}

void MoveGen::generateMoves(MoveList& moves) {
    const Color active_color = m_position.activeColor();

    const u64 empty = m_position.board().getEmptyBitboard();
    const u64 enemy = m_position.board().getColorBitboard(invert(active_color));

    const Wordboard& at = m_position.attackTable(active_color);

    const u64 active = m_position.attackTable(active_color).getAttackedBitboard();
    const u64 danger = m_position.attackTable(invert(active_color)).getAttackedBitboard();

    const u16 valid_plist   = m_position.pieceList(active_color).maskValid();
    const u16 king_mask     = 1;
    const u16 pawn_mask     = m_position.pieceList(active_color).maskEq(PieceType::pawn);
    const u16 non_pawn_mask = valid_plist & ~pawn_mask;

    // En passant
    if (const Square ep = m_position.enPassant(); ep.isValid())
        write(moves, ep, at[ep] & pawn_mask, MoveFlags::en_passant);

    // Undefended non-pawn captures
    write(moves, at, active & enemy & ~danger, non_pawn_mask, MoveFlags::capture_bit);
    // Defended non-pawn captures
    write(moves, at, active & enemy & danger, non_pawn_mask & ~king_mask, MoveFlags::capture_bit);

    const u64 promo_zone = static_cast<u64>(0xFF) << (active_color == Color::white ? 56 : 0);
    // Capture-with-promotion
    write(moves, at, active & enemy & promo_zone, pawn_mask, MoveFlags::promo_queen_capture);
    write(moves, at, active & enemy & promo_zone, pawn_mask, MoveFlags::promo_knight_capture);
    write(moves, at, active & enemy & promo_zone, pawn_mask, MoveFlags::promo_rook_capture);
    write(moves, at, active & enemy & promo_zone, pawn_mask, MoveFlags::promo_bishop_capture);
    // Non-promotion pawn captures
    write(moves, at, active & enemy & ~promo_zone, pawn_mask, MoveFlags::capture_bit);

    // Castling
    // TODO: FRC
    {
        const int      color_shift = active_color == Color::white ? 0 : 56;
        const Square   king_sq     = m_position.kingSq(active_color);
        const RookInfo rook_info   = m_position.rookInfo(active_color);
        if (rook_info.aside.isValid())
        {
            const u64 clear      = empty | king_sq.toBitboard() | rook_info.aside.toBitboard();
            const u8  rank_empty = static_cast<u8>(clear >> color_shift);
            const u8  rank_safe  = static_cast<u8>(~danger >> color_shift);
            if ((rank_empty & 0x1F) == 0x1F && (rank_safe & 0x1C) == 0x1C)
                moves.push_back(Move{king_sq, rook_info.aside, MoveFlags::castle});
        }
        if (rook_info.hside.isValid())
        {
            const u64 clear      = empty | king_sq.toBitboard() | rook_info.hside.toBitboard();
            const u8  rank_empty = static_cast<u8>(clear >> color_shift);
            const u8  rank_safe  = static_cast<u8>(~danger >> color_shift);
            if ((rank_empty & 0xF0) == 0xF0 && (rank_safe & 0x70) == 0x70)
                moves.push_back(Move{king_sq, rook_info.hside, MoveFlags::castle});
        }
    }

    // Undefended non-pawn quiets
    write(moves, at, active & empty & ~danger, non_pawn_mask, MoveFlags::normal);

    // Defended non-pawn quiets
    write(moves, at, active & empty & danger, non_pawn_mask & ~king_mask, MoveFlags::normal);

    // Pawn quiets
    {
        const u64 bb = m_position.board().bitboardFor(active_color, PieceType::pawn);
        const auto [single_push, double_push, promo, single_shift, double_shift] =
          validPawns(active_color, bb, empty, empty);

        writePawn(moves, promo, single_shift, MoveFlags::promo_queen);
        writePawn(moves, promo, single_shift, MoveFlags::promo_knight);
        writePawn(moves, promo, single_shift, MoveFlags::promo_rook);
        writePawn(moves, promo, single_shift, MoveFlags::promo_bishop);

        writePawn(moves, single_push, single_shift, MoveFlags::normal);

        writePawn(moves, double_push, double_shift, MoveFlags::normal);
    }
}

void MoveGen::write(MoveList& moves, Square dest, u16 piecemask, MoveFlags mf) {
    if (!piecemask)
        return;

    const usize count = std::popcount(piecemask);
    for (u8 i = 0; i < count; i++, piecemask &= piecemask - 1)
    {
        const PieceId id{static_cast<u8>(std::countr_zero(piecemask))};
        const Square  src = m_position.pieceListSq(m_active_color)[id];
        moves.push_back(Move{src, dest, mf});
    }
}

void MoveGen::write(MoveList& moves, const Wordboard& at, u64 bb, u16 piecemask, MoveFlags mf) {
    for (; bb != 0; bb &= bb - 1)
    {
        const Square dest{static_cast<u8>(std::countr_zero(bb))};
        write(moves, dest, piecemask & at[dest], mf);
    }
}

void MoveGen::writePawn(MoveList& moves, u64 bb, int shift, MoveFlags mf) {
    for (; bb != 0; bb &= bb - 1)
    {
        const Square src{static_cast<u8>(std::countr_zero(bb))};
        const Square dest{static_cast<u8>(src.raw + shift)};
        moves.push_back(Move{src, dest, mf});
    }
}

}

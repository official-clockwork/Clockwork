#include "movegen.hpp"

#include <bit>
#include <tuple>

#include "common.hpp"
#include "move.hpp"
#include "position.hpp"
#include "util/bit.hpp"
#include "util/types.hpp"

namespace Clockwork {

static std::tuple<u64, u64, u64, int, int> valid_pawns(Color color, u64 bb, u64 empty, u64 dests) {
    switch (color) {
    case Color::White: {
        u64 single = bb & ((empty & dests) >> 8);
        return {single & 0x0000FFFFFFFFFF00,
                bb & (empty >> 8) & ((empty & dests) >> 16) & 0x000000000000FF00,
                single & 0x00FF000000000000, 8, 16};
    }
    case Color::Black: {
        u64 single = bb & ((empty & dests) << 8);
        return {single & 0x00FFFFFFFFFF0000,
                bb & (empty << 8) & ((empty & dests) << 16) & 0x00FF000000000000,
                single & 0x000000000000FF00, -8, -16};
    }
    }
}

void MoveGen::generate_moves(MoveList& moves) {
    Color active_color = m_position.active_color();

    u64 empty = m_position.board().get_empty_bitboard();
    u64 enemy = m_position.board().get_color_bitboard(invert(active_color));

    std::array<u16, 64> at = m_position.attack_table(active_color).to_mailbox();

    u64 active = m_position.attack_table(active_color).get_attacked_bitboard();
    u64 danger = m_position.attack_table(invert(active_color)).get_attacked_bitboard();

    u16 valid_plist   = m_position.piece_list(active_color).mask_valid();
    u16 king_mask     = 1;
    u16 pawn_mask     = m_position.piece_list(active_color).mask_eq(PieceType::Pawn);
    u16 non_pawn_mask = valid_plist & ~pawn_mask;

    if (Square ep = m_position.en_passant(); ep.is_valid()) {
        write(moves, ep, at[ep.raw] & pawn_mask, MoveFlags::EnPassant);
    }

    // Undefended non-pawn captures
    write(moves, at, active & enemy & ~danger, non_pawn_mask, MoveFlags::CaptureBit);
    // Defended non-pawn captures
    write(moves, at, active & enemy & danger, non_pawn_mask & ~king_mask, MoveFlags::CaptureBit);

    u64 promo_zone = static_cast<u64>(0xFF) << (active_color == Color::White ? 56 : 0);
    // Capture-with-promotion
    write(moves, at, active & enemy & promo_zone, pawn_mask, MoveFlags::PromoQueenCapture);
    write(moves, at, active & enemy & promo_zone, pawn_mask, MoveFlags::PromoKnightCapture);
    write(moves, at, active & enemy & promo_zone, pawn_mask, MoveFlags::PromoRookCapture);
    write(moves, at, active & enemy & promo_zone, pawn_mask, MoveFlags::PromoBishopCapture);
    // Non-promotion pawn captures
    write(moves, at, active & enemy & ~promo_zone, pawn_mask, MoveFlags::CaptureBit);

    // Castling
    // TODO: FRC
    {
        int      color_shift = active_color == Color::White ? 0 : 56;
        Square   king_sq     = m_position.king_sq(active_color);
        RookInfo rook_info   = m_position.rook_info(active_color);
        if (rook_info.aside.is_valid()) {
            u64 clear      = empty | king_sq.to_bitboard() | rook_info.aside.to_bitboard();
            u8  rank_empty = static_cast<u8>(clear >> color_shift);
            u8  rank_safe  = static_cast<u8>(~danger >> color_shift);
            if ((rank_empty & 0x1F) == 0x1F && (rank_safe & 0x1C) == 0x1C) {
                moves.push_back(Move{king_sq, rook_info.aside, MoveFlags::Castle});
            }
        }
        if (rook_info.hside.is_valid()) {
            u64 clear      = empty | king_sq.to_bitboard() | rook_info.hside.to_bitboard();
            u8  rank_empty = static_cast<u8>(clear >> color_shift);
            u8  rank_safe  = static_cast<u8>(~danger >> color_shift);
            if ((rank_empty & 0xF0) == 0xF0 && (rank_safe & 0x70) == 0x70) {
                moves.push_back(Move{king_sq, rook_info.hside, MoveFlags::Castle});
            }
        }
    }

    // Undefended non-pawn quiets
    write(moves, at, active & empty & ~danger, non_pawn_mask, MoveFlags::Normal);

    // Defended non-pawn quiets
    write(moves, at, active & empty & danger, non_pawn_mask & ~king_mask, MoveFlags::Normal);

    // Pawn quiets
    {
        u64 bb = m_position.board().bitboard_for(active_color, PieceType::Pawn);
        auto [single_push, double_push, promo, single_shift, double_shift] =
          valid_pawns(active_color, bb, empty, empty);

        write_pawn(moves, promo, single_shift, MoveFlags::PromoQueen);
        write_pawn(moves, promo, single_shift, MoveFlags::PromoKnight);
        write_pawn(moves, promo, single_shift, MoveFlags::PromoRook);
        write_pawn(moves, promo, single_shift, MoveFlags::PromoBishop);

        write_pawn(moves, single_push, single_shift, MoveFlags::Normal);

        write_pawn(moves, double_push, double_shift, MoveFlags::Normal);
    }
}

void MoveGen::write(MoveList& moves, Square dest, u16 piecemask, MoveFlags mf) {
    if (!piecemask) {
        return;
    }

    usize count = std::popcount(piecemask);
    for (u8 i = 0; i < count; i++, piecemask = clear_lowest_bit(piecemask)) {
        PieceId id{static_cast<u8>(std::countr_zero(piecemask))};
        Square  src = m_position.piece_list_sq(m_active_color)[id];
        moves.push_back(Move{src, dest, mf});
    }
}

void MoveGen::write(
  MoveList& moves, const std::array<u16, 64>& at, u64 bb, u16 piecemask, MoveFlags mf) {
    for (; bb != 0; bb = clear_lowest_bit(bb)) {
        Square dest{static_cast<u8>(std::countr_zero(bb))};
        write(moves, dest, piecemask & at[dest.raw], mf);
    }
}

void MoveGen::write_pawn(MoveList& moves, u64 bb, int shift, MoveFlags mf) {
    for (; bb != 0; bb = clear_lowest_bit(bb)) {
        Square src{static_cast<u8>(std::countr_zero(bb))};
        Square dest{static_cast<u8>(src.raw + shift)};
        moves.push_back(Move{src, dest, mf});
    }
}

}

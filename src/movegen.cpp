#include "movegen.hpp"

#include <bit>
#include <cassert>
#include <tuple>

#include "bitboard.hpp"
#include "common.hpp"
#include "geometry.hpp"
#include "move.hpp"
#include "position.hpp"
#include "rays.hpp"
#include "util/bit.hpp"
#include "util/types.hpp"

namespace Clockwork {

static std::tuple<Bitboard, Bitboard, Bitboard, i32, i32>
valid_pawns(Color color, Bitboard bb, Bitboard empty, Bitboard valid_dests) {
    switch (color) {
    case Color::White: {
        Bitboard single = bb & ((empty & valid_dests) >> 8);
        return {single & Bitboard{0x0000FFFFFFFFFF00},
                bb & (empty >> 8) & ((empty & valid_dests) >> 16) & Bitboard{0x000000000000FF00},
                single & Bitboard{0x00FF000000000000}, 8, 16};
    }
    case Color::Black: {
        Bitboard single = bb & ((empty & valid_dests) << 8);
        return {single & Bitboard{0x00FFFFFFFFFF0000},
                bb & (empty << 8) & ((empty & valid_dests) << 16) & Bitboard{0x00FF000000000000},
                single & Bitboard{0x000000000000FF00}, -8, -16};
    }
    }
    unreachable();
}

bool MoveGen::is_legal(Move m) const {
    u16 checkers = m_position.checker_mask();
    switch (std::popcount(checkers)) {
    case 0:
        return is_legal_no_checkers(m, ~Bitboard{0}, true);
    case 1:
        return is_legal_one_checker(m, checkers);
    default:
        return is_legal_two_checkers(m, checkers);
    }
}

void MoveGen::generate_moves(MoveList& noisy, MoveList& quiet) {
    u16 checkers = m_position.checker_mask();
    switch (std::popcount(checkers)) {
    case 0:
        return generate_moves_to<true>(noisy, quiet, ~Bitboard{0}, true);
    case 1:
        return generate_moves_one_checker(noisy, quiet, checkers);
    default:
        return generate_moves_two_checkers(noisy, quiet, checkers);
    }
}

std::tuple<Bitboard, Bitboard, bool> MoveGen::valid_destinations_one_checker(u16 checker) const {
    Color  active_color = m_position.active_color();
    Square king_sq      = m_position.king_sq(active_color);

    u8        checker_id    = static_cast<u8>(std::countr_zero(checker));
    Square    checker_sq    = m_position.piece_list_sq(invert(active_color))[checker_id];
    PieceType checker_ptype = m_position.piece_list(invert(active_color))[checker_id];

    Bitboard valid_dests = rays::inclusive(king_sq, checker_sq);
    Bitboard checker_ray =
      is_slider(checker_ptype) ? rays::infinite_exclusive(king_sq, checker_sq) : Bitboard{};

    return {valid_dests, ~checker_ray, checker_ptype == PieceType::Pawn};
}

Bitboard MoveGen::valid_destinations_two_checkers(u16 checkers) const {
    Color  active_color = m_position.active_color();
    Square king_sq      = m_position.king_sq(active_color);

    Bitboard checkers_rays;
    for (; checkers != 0; checkers = clear_lowest_bit(checkers)) {
        u8        checker_id    = static_cast<u8>(std::countr_zero(checkers));
        Square    checker_sq    = m_position.piece_list_sq(invert(active_color))[checker_id];
        PieceType checker_ptype = m_position.piece_list(invert(active_color))[checker_id];

        if (is_slider(checker_ptype)) {
            checkers_rays |= rays::infinite_exclusive(king_sq, checker_sq);
        }
    }

    return ~checkers_rays;
}

bool MoveGen::is_legal_no_checkers(Move m, Bitboard valid_dests, bool can_ep) const {
    Color  active_color = m_position.active_color();
    Square king_sq      = m_position.king_sq(active_color);

    Place src = m_position.board()[m.from()];
    if (src.is_empty() || src.color() != active_color) {
        return false;
    }

    bool empty_dest = m_position.board()[m.to()].is_empty();
    bool enemy_dest = !empty_dest && m_position.board()[m.to()].color() != active_color;

    // valid_attack implies valid from square in the move! (exception: quiet pawn moves)
    u16  at_dest = m_position.attack_table(active_color).read(m.to()) & m_pin_mask.read(m.to());
    bool valid_attack = (at_dest >> src.id().raw) & 1;

    // There are positions where enpassant is legal but is not set in valid_dests
    if (src.ptype() == PieceType::Pawn && m.flags() == MoveFlags::EnPassant) {
        u16 pawn_mask = m_position.piece_list(active_color).mask_eq(PieceType::Pawn);
        return m_position.en_passant().is_valid() && can_ep && empty_dest && valid_attack
            && m.to() == m_position.en_passant() && !is_ep_clearance_pinned(at_dest & pawn_mask);
    }

    if (!valid_dests.is_set(m.to())) {
        return false;
    }

    if (src.ptype() == PieceType::Pawn) {

        if (m.is_capture()) {
            Bitboard promo_zone{static_cast<u64>(0xFF) << (active_color == Color::White ? 56 : 0)};

            if (m.is_promotion()) {
                return promo_zone.is_set(m.to()) && valid_attack && enemy_dest;
            }

            if (m.flags() == MoveFlags::CaptureBit) {
                return !promo_zone.is_set(m.to()) && valid_attack && enemy_dest;
            }

            return false;
        }

        if (m.flags() == MoveFlags::Normal || m.is_promotion()) {
            Bitboard empty        = m_position.board().get_empty_bitboard();
            Bitboard pinned_pawns = m_pinned & ~Bitboard::file_mask(king_sq.file());
            Bitboard bb =
              m_position.board().bitboard_for(active_color, PieceType::Pawn) & ~pinned_pawns;
            auto [single_push, double_push, promo, single_shift, double_shift] =
              valid_pawns(active_color, bb, empty, valid_dests);

            int move_shift = m.to().raw - m.from().raw;

            if (m.is_promotion()) {
                return promo.is_set(m.from()) && move_shift == single_shift;
            } else {
                return (single_push.is_set(m.from()) && move_shift == single_shift)
                    || (double_push.is_set(m.from()) && move_shift == double_shift);
            }
        }

        return false;
    } else if (src.ptype() == PieceType::King) {
        Bitboard danger = m_position.attack_table(invert(active_color)).get_attacked_bitboard();

        if (m.flags() == MoveFlags::Castle) {
            // TODO: FRC
            Bitboard empty     = m_position.board().get_empty_bitboard();
            RookInfo rook_info = m_position.rook_info(active_color);
            if (rook_info.aside == m.to()) {
                Bitboard clear =
                  empty | Bitboard::from_square(king_sq) | Bitboard::from_square(rook_info.aside);
                u8 rank_empty = clear.front_rank(active_color);
                u8 rank_safe  = (~danger).front_rank(active_color);
                return (rank_empty & 0x1F) == 0x1F && (rank_safe & 0x1C) == 0x1C;
            }
            if (rook_info.hside == m.to()) {
                Bitboard clear =
                  empty | Bitboard::from_square(king_sq) | Bitboard::from_square(rook_info.hside);
                u8 rank_empty = clear.front_rank(active_color);
                u8 rank_safe  = (~danger).front_rank(active_color);

                return (rank_empty & 0xF0) == 0xF0 && (rank_safe & 0x70) == 0x70;
            }
            return false;
        }

        if (danger.is_set(m.to())) {
            // The king cannot walk into danger
            return false;
        }
        // fallthrough
    }

    // Non-pawn moves or King moves with safe destinations

    if (m.flags() == MoveFlags::CaptureBit) {
        return valid_attack && enemy_dest;
    }

    if (m.flags() == MoveFlags::Normal) {
        return valid_attack && empty_dest;
    }

    return false;
}

bool MoveGen::is_legal_king_move(Move m, Bitboard valid_dests) const {
    Color active_color = m_position.active_color();

    if (m.from() != m_position.king_sq(active_color)) {
        return false;
    }

    bool danger = m_position.attack_table(invert(active_color)).read(m.to()) != 0;
    if (danger) {
        return false;
    }

    if (!valid_dests.is_set(m.to())) {
        return false;
    }

    bool valid_attack = m_position.attack_table(active_color).read(m.to()) & 1;
    if (!valid_attack) {
        return false;
    }

    bool empty_dest = m_position.board()[m.to()].is_empty();
    bool enemy_dest = !empty_dest && m_position.board()[m.to()].color() != active_color;

    if (m.flags() == MoveFlags::CaptureBit) {
        return enemy_dest;
    }

    if (m.flags() == MoveFlags::Normal) {
        return empty_dest;
    }

    return false;
}

bool MoveGen::is_legal_one_checker(Move m, u16 checkers) const {
    auto [valid_dests, non_checker_ray, has_ep] = valid_destinations_one_checker(checkers);

    if (m.from() == m_position.king_sq(m_active_color)) {
        return is_legal_king_move(m, non_checker_ray);
    } else {
        return is_legal_no_checkers(m, valid_dests, has_ep);
    }
}

bool MoveGen::is_legal_two_checkers(Move m, u16 checkers) const {
    Bitboard non_checker_ray = valid_destinations_two_checkers(checkers);

    return is_legal_king_move(m, non_checker_ray);
}

template<bool king_moves>
void MoveGen::generate_moves_to(MoveList& noisy,
                                MoveList& quiet,
                                Bitboard  valid_dests,
                                bool      can_ep) {
    Color active_color = m_position.active_color();

    Bitboard empty = m_position.board().get_empty_bitboard();
    Bitboard enemy = m_position.board().get_color_bitboard(invert(active_color));

    std::array<u16, 64> at = (m_position.attack_table(active_color) & m_pin_mask).to_mailbox();

    u16 king_mask = 1;
    u16 pawn_mask = m_position.piece_list(active_color).mask_eq(PieceType::Pawn);

    Bitboard pawn_active =
      m_position.attack_table(active_color).get_piece_mask_bitboard(pawn_mask) & valid_dests;
    Bitboard nonpawn_active =
      m_position.attack_table(active_color).get_piece_mask_bitboard(~pawn_mask) & valid_dests;
    Bitboard danger = m_position.attack_table(invert(active_color)).get_attacked_bitboard();

    u16 valid_plist = m_position.piece_list(active_color).mask_valid();
    if constexpr (!king_moves) {
        valid_plist &= ~king_mask;
    }
    u16 non_pawn_mask = valid_plist & ~pawn_mask;

    if (Square ep = m_position.en_passant(); can_ep && ep.is_valid()) {
        u16 ep_attackers_mask = at[ep.raw] & pawn_mask;
        if (ep_attackers_mask && !is_ep_clearance_pinned(ep_attackers_mask)) {
            write(noisy, ep, ep_attackers_mask, MoveFlags::EnPassant);
        }
    }

    // Undefended non-pawn captures
    write(noisy, at, nonpawn_active & enemy & ~danger, non_pawn_mask, MoveFlags::CaptureBit);
    // Defended non-pawn captures
    write(noisy, at, nonpawn_active & enemy & danger, non_pawn_mask & ~king_mask,
          MoveFlags::CaptureBit);

    Bitboard promo_zone{static_cast<u64>(0xFF) << (active_color == Color::White ? 56 : 0)};
    // Capture-with-promotion
    write(noisy, at, pawn_active & enemy & promo_zone, pawn_mask, MoveFlags::PromoQueenCapture);
    write(noisy, at, pawn_active & enemy & promo_zone, pawn_mask, MoveFlags::PromoKnightCapture);
    write(noisy, at, pawn_active & enemy & promo_zone, pawn_mask, MoveFlags::PromoRookCapture);
    write(noisy, at, pawn_active & enemy & promo_zone, pawn_mask, MoveFlags::PromoBishopCapture);
    // Non-promotion pawn captures
    write(noisy, at, pawn_active & enemy & ~promo_zone, pawn_mask, MoveFlags::CaptureBit);

    // Castling
    // TODO: FRC
    if constexpr (king_moves) {
        Square   king_sq   = m_position.king_sq(active_color);
        RookInfo rook_info = m_position.rook_info(active_color);
        if (rook_info.aside.is_valid()) {
            Bitboard clear =
              empty | Bitboard::from_square(king_sq) | Bitboard::from_square(rook_info.aside);
            u8 rank_empty = clear.front_rank(active_color);
            u8 rank_safe  = (~danger).front_rank(active_color);
            if ((rank_empty & 0x1F) == 0x1F && (rank_safe & 0x1C) == 0x1C) {
                quiet.push_back(Move{king_sq, rook_info.aside, MoveFlags::Castle});
            }
        }
        if (rook_info.hside.is_valid()) {
            Bitboard clear =
              empty | Bitboard::from_square(king_sq) | Bitboard::from_square(rook_info.hside);
            u8 rank_empty = clear.front_rank(active_color);
            u8 rank_safe  = (~danger).front_rank(active_color);
            if ((rank_empty & 0xF0) == 0xF0 && (rank_safe & 0x70) == 0x70) {
                quiet.push_back(Move{king_sq, rook_info.hside, MoveFlags::Castle});
            }
        }
    }

    // Undefended non-pawn quiets
    write(quiet, at, nonpawn_active & empty & ~danger, non_pawn_mask, MoveFlags::Normal);

    // Defended non-pawn quiets
    write(quiet, at, nonpawn_active & empty & danger, non_pawn_mask & ~king_mask,
          MoveFlags::Normal);

    // Pawn quiets
    {
        Square   king_sq      = m_position.king_sq(active_color);
        Bitboard pinned_pawns = m_pinned & ~Bitboard::file_mask(king_sq.file());

        Bitboard bb =
          m_position.board().bitboard_for(active_color, PieceType::Pawn) & ~pinned_pawns;
        auto [single_push, double_push, promo, single_shift, double_shift] =
          valid_pawns(active_color, bb, empty, valid_dests);

        write_pawn(noisy, promo, single_shift, MoveFlags::PromoQueen);
        write_pawn(quiet, promo, single_shift, MoveFlags::PromoKnight);
        write_pawn(quiet, promo, single_shift, MoveFlags::PromoRook);
        write_pawn(quiet, promo, single_shift, MoveFlags::PromoBishop);

        write_pawn(quiet, single_push, single_shift, MoveFlags::Normal);

        write_pawn(quiet, double_push, double_shift, MoveFlags::Normal);
    }
}

void MoveGen::generate_king_moves_to(MoveList& noisy, MoveList& quiet, Bitboard valid_dests) {
    Color active_color = m_position.active_color();

    Bitboard empty = m_position.board().get_empty_bitboard();
    Bitboard enemy = m_position.board().get_color_bitboard(invert(active_color));

    std::array<u16, 64> at = m_position.attack_table(active_color).to_mailbox();

    u16 king_mask = 1;

    Bitboard active =
      m_position.attack_table(active_color).get_piece_mask_bitboard(king_mask) & valid_dests;
    Bitboard danger = m_position.attack_table(invert(active_color)).get_attacked_bitboard();

    // Undefended captures
    write(noisy, at, active & enemy & ~danger, king_mask, MoveFlags::CaptureBit);
    // Undefended quiets
    write(quiet, at, active & empty & ~danger, king_mask, MoveFlags::Normal);
}

void MoveGen::generate_moves_one_checker(MoveList& noisy, MoveList& quiet, u16 checker) {
    auto [valid_dests, non_checker_ray, has_ep] = valid_destinations_one_checker(checker);

    generate_moves_to<false>(noisy, quiet, valid_dests, has_ep);
    generate_king_moves_to(noisy, quiet, non_checker_ray);
}

void MoveGen::generate_moves_two_checkers(MoveList& noisy, MoveList& quiet, u16 checkers) {
    Bitboard non_checker_ray = valid_destinations_two_checkers(checkers);

    generate_king_moves_to(noisy, quiet, non_checker_ray);
}

void MoveGen::write(MoveList& moves, Square dest, u16 piecemask, MoveFlags mf) {
    for (; piecemask != 0; piecemask = clear_lowest_bit(piecemask)) {
        PieceId id{static_cast<u8>(std::countr_zero(piecemask))};
        Square  src = m_position.piece_list_sq(m_active_color)[id];
        moves.push_back(Move{src, dest, mf});
    }
}

void MoveGen::write(
  MoveList& moves, const std::array<u16, 64>& at, Bitboard dest_bb, u16 piecemask, MoveFlags mf) {
    for (Square dest : dest_bb) {
        write(moves, dest, piecemask & at[dest.raw], mf);
    }
}

void MoveGen::write_pawn(MoveList& moves, Bitboard src_bb, i32 shift, MoveFlags mf) {
    for (Square src : src_bb) {
        Square dest{static_cast<u8>(src.raw + shift)};
        moves.push_back(Move{src, dest, mf});
    }
}

bool MoveGen::is_ep_clearance_pinned(u16 ep_attackers_mask) const {
    Color  active_color = m_position.active_color();
    Square king_sq      = m_position.king_sq(active_color);
    Square ep           = m_position.en_passant();
    Square victim_sq    = Square{static_cast<u8>(ep.raw + (active_color == Color::White ? -8 : 8))};

    assert(ep.is_valid());

    if (victim_sq.rank() != king_sq.rank()) {
        // Potential rook/queen and king are not aligned along the rank: Clearance pin impossible.
        return false;
    }

    if (std::popcount(ep_attackers_mask) > 1) {
        // Two valid en-passant pawns: Clearance pin impossible.
        return false;
    }

    u8     my_pawn_id = static_cast<u8>(std::countr_zero(ep_attackers_mask));
    Square my_pawn_sq = m_position.piece_list_sq(active_color)[my_pawn_id];

    // Handle rare case: Detect clearance pin.
    Position after_ep = m_position.move(Move{my_pawn_sq, ep, MoveFlags::EnPassant});
    return !after_ep.is_valid();
}

}

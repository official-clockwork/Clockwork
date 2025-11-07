#include "position.hpp"

#include "board.hpp"
#include "common.hpp"
#include "geometry.hpp"
#include "psqt_state.hpp"
#include "util/parse.hpp"
#include "util/types.hpp"
#include "zobrist.hpp"
#include <array>
#include <bit>
#include <iostream>
#include <sstream>

namespace Clockwork {

void Position::incrementally_remove_piece(bool         color,
                                          PieceId      id,
                                          Square       from,
                                          PsqtUpdates& updates) {
    remove_attacks(color, id);
    toggle_rays(from);

    // TODO: check if some speed left on the table for zobrist here
    Color     pcolor    = m_board[from].color();
    PieceType ptype     = m_board[from].ptype();
    u64       piece_key = Zobrist::piece_square_zobrist[static_cast<usize>(pcolor)]
                                                 [static_cast<usize>(ptype)][from.raw];
    m_hash_key ^= piece_key;
    if (ptype == PieceType::Pawn) {
        m_pawn_key ^= piece_key;
    } else {
        m_non_pawn_key[static_cast<usize>(pcolor)] ^= piece_key;
        if (ptype == PieceType::Rook || ptype == PieceType::Queen || ptype == PieceType::King) {
            m_major_key ^= piece_key;
        }
        if (ptype == PieceType::Knight || ptype == PieceType::Bishop || ptype == PieceType::King) {
            m_minor_key ^= piece_key;
        }
    }
    updates.removes.push_back({pcolor, ptype, from});
    m_board[from] = Place::empty();
}

void Position::incrementally_add_piece(bool color, Place p, Square to, PsqtUpdates& updates) {
    // TODO: check if some speed left on the table for zobrist here
    m_board[to]      = p;
    Color     pcolor = p.color();
    PieceType ptype  = p.ptype();
    u64       piece_key =
      Zobrist::piece_square_zobrist[static_cast<usize>(pcolor)][static_cast<usize>(ptype)][to.raw];
    m_hash_key ^= piece_key;
    if (ptype == PieceType::Pawn) {
        m_pawn_key ^= piece_key;
    } else {
        m_non_pawn_key[static_cast<usize>(pcolor)] ^= piece_key;
        if (ptype == PieceType::Rook || ptype == PieceType::Queen || ptype == PieceType::King) {
            m_major_key ^= piece_key;
        }
        if (ptype == PieceType::Knight || ptype == PieceType::Bishop || ptype == PieceType::King) {
            m_minor_key ^= piece_key;
        }
    }
    updates.adds.push_back({pcolor, ptype, to});

    m8x64 m = toggle_rays(to);
    add_attacks(color, p.id(), to, p.ptype(), m);
}

void Position::incrementally_mutate_piece(
  bool old_color, PieceId old_id, Square sq, bool new_color, Place p, PsqtUpdates& updates) {
    PieceType ptype = m_board[sq].ptype();

    // TODO: check if some speed left on the table for zobrist here
    u64 rem_piece_key = Zobrist::piece_square_zobrist[static_cast<usize>(m_board[sq].color())]
                                                     [static_cast<usize>(ptype)][sq.raw];
    m_hash_key ^= rem_piece_key;
    if (ptype == PieceType::Pawn) {
        m_pawn_key ^= rem_piece_key;
    } else {
        m_non_pawn_key[static_cast<usize>(m_board[sq].color())] ^= rem_piece_key;
        if (ptype == PieceType::Rook || ptype == PieceType::Queen || ptype == PieceType::King) {
            m_major_key ^= rem_piece_key;
        }
        if (ptype == PieceType::Knight || ptype == PieceType::Bishop || ptype == PieceType::King) {
            m_minor_key ^= rem_piece_key;
        }
    }
    updates.removes.push_back({m_board[sq].color(), ptype, sq});
    m_board[sq]       = p;
    ptype             = m_board[sq].ptype();
    u64 add_piece_key = Zobrist::piece_square_zobrist[static_cast<usize>(m_board[sq].color())]
                                                     [static_cast<usize>(ptype)][sq.raw];
    m_hash_key ^= add_piece_key;
    if (ptype == PieceType::Pawn) {
        m_pawn_key ^= add_piece_key;
    } else {
        m_non_pawn_key[static_cast<usize>(m_board[sq].color())] ^= add_piece_key;
        if (ptype == PieceType::Rook || ptype == PieceType::Queen || ptype == PieceType::King) {
            m_major_key ^= add_piece_key;
        }
        if (ptype == PieceType::Knight || ptype == PieceType::Bishop || ptype == PieceType::King) {
            m_minor_key ^= add_piece_key;
        }
    }
    updates.adds.push_back({p.color(), p.ptype(), sq});

    remove_attacks(old_color, old_id);
    add_attacks(new_color, p.id(), sq, p.ptype());
}

void Position::incrementally_move_piece(
  bool color, Square from, Square to, Place p, PsqtUpdates& updates) {
    remove_attacks(color, p.id());

    auto [src_ray_coords, src_ray_valid] = geometry::superpiece_rays(from);
    auto [dst_ray_coords, dst_ray_valid] = geometry::superpiece_rays(to);
    u8x64 src_ray_places                 = src_ray_coords.swizzle(m_board.to_vector());

    PieceType ptype = m_board[from].ptype();

    // TODO: check if some speed left on the table for zobrist here
    u64 rem_piece_key = Zobrist::piece_square_zobrist[static_cast<usize>(m_board[from].color())]
                                                     [static_cast<usize>(ptype)][from.raw];
    m_hash_key ^= rem_piece_key;
    if (ptype == PieceType::Pawn) {
        m_pawn_key ^= rem_piece_key;
    } else {
        m_non_pawn_key[static_cast<usize>(m_board[from].color())] ^= rem_piece_key;
        if (ptype == PieceType::Rook || ptype == PieceType::Queen || ptype == PieceType::King) {
            m_major_key ^= rem_piece_key;
        }
        if (ptype == PieceType::Knight || ptype == PieceType::Bishop || ptype == PieceType::King) {
            m_minor_key ^= rem_piece_key;
        }
    }
    updates.removes.push_back({m_board[from].color(), ptype, from});
    m_board[from]     = Place::empty();
    m_board[to]       = p;
    u64 add_piece_key = Zobrist::piece_square_zobrist[static_cast<usize>(m_board[to].color())]
                                                     [static_cast<usize>(ptype)][to.raw];
    m_hash_key ^= add_piece_key;
    if (ptype == PieceType::Pawn) {
        m_pawn_key ^= add_piece_key;
    } else {
        m_non_pawn_key[static_cast<usize>(m_board[to].color())] ^= add_piece_key;
        if (ptype == PieceType::Rook || ptype == PieceType::Queen || ptype == PieceType::King) {
            m_major_key ^= add_piece_key;
        }
        if (ptype == PieceType::Knight || ptype == PieceType::Bishop || ptype == PieceType::King) {
            m_minor_key ^= add_piece_key;
        }
    }
    updates.adds.push_back({p.color(), p.ptype(), to});

    u8x64 dst_ray_places = dst_ray_coords.swizzle(m_board.to_vector());

    m8x64 src_all_sliders     = geometry::slider_mask(src_ray_places);
    m8x64 dst_all_sliders     = geometry::slider_mask(dst_ray_places);
    m8x64 src_raymask         = geometry::superpiece_attacks(src_ray_places, src_ray_valid);
    m8x64 dst_raymask         = geometry::superpiece_attacks(dst_ray_places, dst_ray_valid);
    u8x64 src_visible_sliders = (src_raymask & src_all_sliders).mask(src_ray_places);
    u8x64 dst_visible_sliders = (dst_raymask & dst_all_sliders).mask(dst_ray_places);

    u8x64 src_slider_ids = geometry::slider_broadcast(src_visible_sliders & u8x64::splat(0x1F));
    u8x64 dst_slider_ids = geometry::slider_broadcast(dst_visible_sliders & u8x64::splat(0x1F));

    src_slider_ids = src_raymask.mask(geometry::flip_rays(src_slider_ids));  // flip rays
    dst_slider_ids = dst_raymask.mask(geometry::flip_rays(dst_slider_ids));  // flip rays
    dst_slider_ids |= dst_raymask.mask(u8x64::splat(0x20));  // pack information for efficiency

    u8x64 src_inv_perm = geometry::superpiece_inverse_rays_avx2(from);
    u8x64 dst_inv_perm = geometry::superpiece_inverse_rays_avx2(to);

    // Transform into board layout
    src_slider_ids = src_inv_perm.swizzle(src_slider_ids);
    dst_slider_ids = dst_inv_perm.swizzle(dst_slider_ids);

    // Recover color information
    u8x64 src_col = src_slider_ids.test(u8x64::splat(0x10)).to_vector();
    u8x64 dst_col = dst_slider_ids.test(u8x64::splat(0x10)).to_vector();
    // Recover ray mask information
    m8x64 ret = dst_slider_ids.test(u8x64::splat(0x20));

    src_slider_ids &= u8x64::splat(0x0F);
    dst_slider_ids &= u8x64::splat(0x0F);

    // AVX2 doesn't have a variable word shift, so were're doing it this way.
    // Index zero is invalid here (the king is never a slider), so 0 converts to 0.
    static const u8x16 BITS_LO{{0x00, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80,  //
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};
    static const u8x16 BITS_HI{{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //
                                0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80}};
    u8x64              src_at_lo = src_slider_ids.swizzle(BITS_LO);
    u8x64              src_at_hi = src_slider_ids.swizzle(BITS_HI);
    u8x64              dst_at_lo = dst_slider_ids.swizzle(BITS_LO);
    u8x64              dst_at_hi = dst_slider_ids.swizzle(BITS_HI);

    u8x64 src_color0 = src_col.zip_low_128lanes(src_col);
    u8x64 src_color1 = src_col.zip_high_128lanes(src_col);
    u8x64 dst_color0 = dst_col.zip_low_128lanes(dst_col);
    u8x64 dst_color1 = dst_col.zip_high_128lanes(dst_col);

    u16x64 src_color = std::bit_cast<u16x64>(std::array<u8x64, 2>{src_color0, src_color1});
    u16x64 dst_color = std::bit_cast<u16x64>(std::array<u8x64, 2>{dst_color0, dst_color1});

    u8x64 src_at0 = src_at_lo.zip_low_128lanes(src_at_hi);
    u8x64 src_at1 = src_at_lo.zip_high_128lanes(src_at_hi);
    u8x64 dst_at0 = dst_at_lo.zip_low_128lanes(dst_at_hi);
    u8x64 dst_at1 = dst_at_lo.zip_high_128lanes(dst_at_hi);

    u16x64 src_at = std::bit_cast<u16x64>(std::array<u8x64, 2>{src_at0, src_at1});
    u16x64 dst_at = std::bit_cast<u16x64>(std::array<u8x64, 2>{dst_at0, dst_at1});

    m_attack_table[0].raw ^= src_at.andnot(src_color) ^ dst_at.andnot(dst_color);
    m_attack_table[1].raw ^= (src_at & src_color) ^ (dst_at & dst_color);

    add_attacks(color, p.id(), to, p.ptype(), ret);
}

void Position::remove_attacks(bool color, PieceId id) {
    u16x64 mask = u16x64::splat(static_cast<u16>(~id.to_piece_mask().value()));
    m_attack_table[color].raw &= mask;
}

m8x64 Position::toggle_rays(Square sq) {
    auto [ray_coords, ray_valid] = geometry::superpiece_rays(sq);
    u8x64 ray_places             = ray_coords.swizzle(m_board.to_vector());

    m8x64 all_sliders     = geometry::slider_mask(ray_places);
    m8x64 raymask         = geometry::superpiece_attacks(ray_places, ray_valid);
    u8x64 visible_sliders = (raymask & all_sliders).mask(ray_places);

    u8x64 slider_ids = geometry::slider_broadcast(visible_sliders & u8x64::splat(0x1F));

    slider_ids = raymask.mask(geometry::flip_rays(slider_ids));  // flip rays
    slider_ids |= raymask.mask(u8x64::splat(0x20));              // pack information for efficiency

    u8x64 inv_perm = geometry::superpiece_inverse_rays_avx2(sq);

    // Transform into board layout
    slider_ids = inv_perm.swizzle(slider_ids);

    // Recover color information
    u8x64 col = slider_ids.test(u8x64::splat(0x10)).to_vector();
    // Recover ray mask information
    m8x64 ret = slider_ids.test(u8x64::splat(0x20));

    slider_ids &= u8x64::splat(0x0F);

    // AVX2 doesn't have a variable word shift, so were're doing it this way.
    // Index zero is invalid here (the king is never a slider), so 0 converts to 0.
    static const u8x16 BITS_LO{{0x00, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80,  //
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};
    static const u8x16 BITS_HI{{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //
                                0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80}};
    u8x64              at_lo = slider_ids.swizzle(BITS_LO);
    u8x64              at_hi = slider_ids.swizzle(BITS_HI);

    u8x64  color0 = col.zip_low_128lanes(col);
    u8x64  color1 = col.zip_high_128lanes(col);
    u16x64 color  = std::bit_cast<u16x64>(std::array<u8x64, 2>{color0, color1});

    u8x64  at0 = at_lo.zip_low_128lanes(at_hi);
    u8x64  at1 = at_lo.zip_high_128lanes(at_hi);
    u16x64 at  = std::bit_cast<u16x64>(std::array<u8x64, 2>{at0, at1});

    m_attack_table[0].raw ^= at.andnot(color);
    m_attack_table[1].raw ^= at & color;

    return ret;
}

void Position::add_attacks(bool color, PieceId id, Square sq, PieceType ptype) {
    switch (ptype) {
    case PieceType::None:
        return;
    case PieceType::Pawn:
    case PieceType::Knight:
    case PieceType::King:
        add_attacks(color, id, sq, ptype, m8x64::splat(true));
        return;
    case PieceType::Bishop:
    case PieceType::Rook:
    case PieceType::Queen: {
        auto [ray_coords, ray_valid] = geometry::superpiece_rays(sq);
        u8x64 ray_places             = ray_coords.swizzle(m_board.to_vector());
        m8x64 raymask                = geometry::superpiece_attacks(ray_places, ray_valid);

        u8x64 inv_perm  = geometry::superpiece_inverse_rays_avx2(sq);
        m8x64 boardmask = inv_perm.swizzle(raymask);

        add_attacks(color, id, sq, ptype, boardmask);
        return;
    }
    }
}

void Position::add_attacks(bool color, PieceId id, Square sq, PieceType ptype, m8x64 mask) {
    u8x64 moves = (mask & geometry::piece_moves_avx2(color, ptype, sq)).to_vector();

    u8x64  m0 = moves.zip_low_128lanes(moves);
    u8x64  m1 = moves.zip_high_128lanes(moves);
    u16x64 m  = std::bit_cast<u16x64>(std::array<u8x64, 2>{m0, m1});

    u16x64 bit = u16x64::splat(id.to_piece_mask().value());
    m_attack_table[color].raw |= m & bit;
}

template<bool UPDATE_PSQT>
Position Position::move(Move m, PsqtState* psqtState) const {
    Position    new_pos = *this;
    PsqtUpdates updates{};

    Square from  = m.from();
    Square to    = m.to();
    Place  src   = m_board[from];
    Place  dst   = m_board[to];
    bool   color = static_cast<bool>(m_active_color);

    // move() makes a legal, non null move, so we always swap sides.
    new_pos.m_hash_key ^= Zobrist::side_key;

    if (m_enpassant.is_valid()) {
        // Remove hash for ep square
        new_pos.m_hash_key ^= Zobrist::en_passant_zobrist[new_pos.m_enpassant.raw];
        new_pos.m_enpassant = Square::invalid();
    }

    // Compute old castle index for zobrist indexing and remove it
    usize old_castle_index =
      new_pos.m_rook_info[0].as_index() | (new_pos.m_rook_info[1].as_index() << 2);
    new_pos.m_hash_key ^= Zobrist::castling_zobrist[old_castle_index];

    const auto CHECK_SRC_CASTLING_RIGHTS = [&] {
        if (src.ptype() == PieceType::Rook) {
            new_pos.m_rook_info[color].unset(from);
        } else if (src.ptype() == PieceType::King) {
            new_pos.m_rook_info[color].clear();
        }
    };

    const auto CHECK_DST_CASTLING_RIGHTS = [&] {
        if (dst.ptype() == PieceType::Rook) {
            new_pos.m_rook_info[!color].unset(to);
        }
    };

    switch (m.flags()) {
    case MoveFlags::Normal:
        new_pos.incrementally_move_piece(color, from, to, src, updates);

        new_pos.m_piece_list_sq[color][src.id()] = to;

        if (src.ptype() == PieceType::Pawn) {
            new_pos.m_50mr = 0;
            if (from.raw - to.raw == 16 || to.raw - from.raw == 16) {
                Color  them = invert(m_active_color);
                Square ep   = Square{static_cast<u8>((from.raw + to.raw) / 2)};
                if (is_square_attacked_by(ep, them, PieceType::Pawn)) {
                    new_pos.m_enpassant = ep;
                    new_pos.m_hash_key ^= Zobrist::en_passant_zobrist[new_pos.m_enpassant.raw];
                }
            }
        } else {
            new_pos.m_50mr++;
            CHECK_SRC_CASTLING_RIGHTS();
        }
        break;
    case MoveFlags::CaptureBit:
        new_pos.incrementally_remove_piece(color, src.id(), from, updates);
        new_pos.incrementally_mutate_piece(!color, dst.id(), to, color, src, updates);

        new_pos.m_piece_list_sq[color][src.id()]  = to;
        new_pos.m_piece_list_sq[!color][dst.id()] = Square::invalid();
        new_pos.m_piece_list[!color][dst.id()]    = PieceType::None;

        new_pos.m_50mr = 0;
        CHECK_SRC_CASTLING_RIGHTS();
        CHECK_DST_CASTLING_RIGHTS();
        break;
    case MoveFlags::Castle: {
        bool    aside      = to.file() < from.file();
        PieceId king_id    = PieceId{0};  // == src.id()
        PieceId rook_id    = dst.id();
        Square  king_from  = from;
        Square  rook_from  = to;
        Square  king_to    = Square::from_file_and_rank(aside ? 2 : 6, from.rank());
        Square  rook_to    = Square::from_file_and_rank(aside ? 3 : 5, from.rank());
        Place   king_place = Place{m_active_color, PieceType::King, king_id};
        Place   rook_place = Place{m_active_color, PieceType::Rook, rook_id};

        // TODO: Optimize further (slider updates can be elided in some cases).
        new_pos.incrementally_remove_piece(color, king_id, king_from, updates);
        new_pos.incrementally_remove_piece(color, rook_id, rook_from, updates);
        new_pos.incrementally_add_piece(color, king_place, king_to, updates);
        new_pos.incrementally_add_piece(color, rook_place, rook_to, updates);

        new_pos.m_piece_list_sq[color][king_id] = king_to;
        new_pos.m_piece_list_sq[color][rook_id] = rook_to;

        new_pos.m_50mr++;
        new_pos.m_rook_info[color].clear();
        break;
    }
    case MoveFlags::EnPassant: {
        Square victim_sq = Square::from_file_and_rank(m_enpassant.file(), from.rank());
        Place  victim    = m_board[victim_sq];

        new_pos.incrementally_remove_piece(!color, victim.id(), victim_sq, updates);
        new_pos.incrementally_move_piece(color, from, to, src, updates);

        new_pos.m_piece_list_sq[color][src.id()]     = to;
        new_pos.m_piece_list_sq[!color][victim.id()] = Square::invalid();
        new_pos.m_piece_list[!color][victim.id()]    = PieceType::None;

        new_pos.m_50mr = 0;
        break;
    }
    case MoveFlags::PromoKnight:
    case MoveFlags::PromoBishop:
    case MoveFlags::PromoRook:
    case MoveFlags::PromoQueen: {
        Place new_place{m_active_color, *m.promo(), src.id()};

        new_pos.incrementally_move_piece(color, from, to, new_place, updates);

        new_pos.m_piece_list_sq[color][src.id()] = to;
        new_pos.m_piece_list[color][src.id()]    = *m.promo();

        new_pos.m_50mr = 0;
        break;
    }
    case MoveFlags::PromoKnightCapture:
    case MoveFlags::PromoBishopCapture:
    case MoveFlags::PromoRookCapture:
    case MoveFlags::PromoQueenCapture: {
        Place new_place{m_active_color, *m.promo(), src.id()};

        new_pos.incrementally_remove_piece(color, src.id(), from, updates);
        new_pos.incrementally_mutate_piece(!color, dst.id(), to, color, new_place, updates);

        new_pos.m_piece_list_sq[color][src.id()]  = to;
        new_pos.m_piece_list[color][src.id()]     = *m.promo();
        new_pos.m_piece_list_sq[!color][dst.id()] = Square::invalid();
        new_pos.m_piece_list[!color][dst.id()]    = PieceType::None;

        new_pos.m_50mr = 0;
        CHECK_DST_CASTLING_RIGHTS();
        break;
    }
    }

    // Calculate the new castling index for zobrist indexing and add it back in
    usize new_castle_index =
      new_pos.m_rook_info[0].as_index() | (new_pos.m_rook_info[1].as_index() << 2);
    new_pos.m_hash_key ^= Zobrist::castling_zobrist[new_castle_index];

    new_pos.m_active_color = invert(m_active_color);
    new_pos.m_ply++;

    if constexpr (UPDATE_PSQT) {
        psqtState->apply_updates(new_pos, updates);
    }

    return new_pos;
}

template Position Position::move<true>(Move m, PsqtState* psqtState) const;
template Position Position::move<false>(Move m, PsqtState* psqtState) const;

Position Position::null_move() const {
    Position new_pos = *this;
    new_pos.m_hash_key ^= Zobrist::side_key;

    if (m_enpassant.is_valid()) {
        // Remove hash for ep square
        new_pos.m_hash_key ^= Zobrist::en_passant_zobrist[new_pos.m_enpassant.raw];
        new_pos.m_enpassant = Square::invalid();
    }

    new_pos.m_active_color = invert(m_active_color);
    new_pos.m_ply++;

    return new_pos;
}

std::tuple<Wordboard, Bitboard> Position::calc_pin_mask() const {
    Square king_square = king_sq(m_active_color);

    auto [ray_coords, ray_valid] = geometry::superpiece_rays(king_square);
    u8x64 ray_places             = ray_coords.swizzle(m_board.to_vector());
    u8x64 inverse_perm           = geometry::superpiece_inverse_rays_avx2(king_square);

    // Ignore horse moves
    ray_valid &= m8x64{0xFEFEFEFEFEFEFEFE};

    m8x64 occupied = ray_valid.andnot(ray_places.zeros());

    u8x64 color_mask  = u8x64::splat(Place::COLOR_MASK);
    u8x64 enemy_color = u8x64::splat(m_active_color == Color::White ? Place::COLOR_MASK : 0);
    m8x64 enemy       = occupied & (ray_places & color_mask).eq(enemy_color);

    m8x64 closest      = occupied & geometry::superpiece_attacks(ray_places, ray_valid);
    m8x64 maybe_pinned = closest.andnot(enemy);

    // Find enemy sliders of the correct type
    m8x64 maybe_pinner1 = enemy & geometry::slider_mask(ray_places);

    // Find second-closest pieces along each ray
    m8x64 not_closest   = occupied.andnot(closest);
    m8x64 pin_raymask   = geometry::superpiece_attacks(not_closest.to_vector(), ray_valid);
    m8x64 maybe_pinner2 = not_closest & pin_raymask;

    // Pinners are second-closest pieces that are enemy sliders of the correct type.
    m8x64 pinner = maybe_pinner1 & maybe_pinner2;

// Does this ray have a pinner?
#if LPS_AVX512
    m8x64 no_pinner_mask{
      std::bit_cast<vm8x64>(std::bit_cast<u64x8>(pinner.to_vector()).zeros().to_vector())
        .to_bits()};
#else
    m8x64 no_pinner_mask = std::bit_cast<m8x64>(std::bit_cast<m64x8>(pinner).to_vector().zeros());
#endif
    m8x64 pinned = maybe_pinned.andnot(no_pinner_mask);

    u8x64 nonmasked_pinned_ids =
      geometry::lane_broadcast(pinned.mask(ray_places & u8x64::splat(0xF)));
    u8x64 pinned_ids = pin_raymask.mask(nonmasked_pinned_ids);
    // Transform into board layout
    pinned_ids = inverse_perm.swizzle(pinned_ids);

    u16 nonpinned_piece_mask = static_cast<u16>(
      ~((u64x8::splat(1) << (std::bit_cast<u64x8>(nonmasked_pinned_ids) & u64x8::splat(0xF)))
          .reduce_or())
      | 1);

    // AVX2 doesn't have a variable word shift, so were're doing it this way.
    // Index zero is invalid here (the king is never pinned), so 0 converts to 0.
    static const u8x16 BITS_LO{{0x00, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80,  //
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};
    static const u8x16 BITS_HI{{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //
                                0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80}};
    u8x64              at_lo = pinned_ids.swizzle(BITS_LO);
    u8x64              at_hi = pinned_ids.swizzle(BITS_HI);

    u16x64 nppm = u16x64::splat(nonpinned_piece_mask);

    u16x64 at = std::bit_cast<u16x64>(
      std::array<u8x64, 2>{at_lo.zip_low_128lanes(at_hi), at_lo.zip_high_128lanes(at_hi)});
    at |= nppm;

    u64 pinned_bb = at.neq(nppm).to_bits();

    return {Wordboard{at}, Bitboard{pinned_bb}};
}

const std::array<Wordboard, 2> Position::calc_attacks_slow() {
    std::array<std::array<PieceMask, 64>, 2> result{};
    for (usize i = 0; i < 64; i++) {
        Square sq{static_cast<u8>(i)};
        auto [white, black] = calc_attacks_slow(sq);
        result[0][i]        = white;
        result[1][i]        = black;
    }
    return std::bit_cast<std::array<Wordboard, 2>>(result);
}

const std::array<PieceMask, 2> Position::calc_attacks_slow(Square sq) {
    auto [ray_coords, ray_valid] = geometry::superpiece_rays(sq);
    u8x64 ray_places             = ray_coords.swizzle(m_board.to_vector());

    m8x64 color             = ray_places.test(u8x64::splat(0x10));
    m8x64 visible           = geometry::superpiece_attacks(ray_places, ray_valid);
    m8x64 attackers         = geometry::attackers_from_rays(ray_places);
    m8x64 visible_attackers = visible & attackers;
    m8x64 white_attackers   = ~color & visible_attackers;
    m8x64 black_attackers   = color & visible_attackers;

    usize white_attackers_count = white_attackers.popcount();
    usize black_attackers_count = black_attackers.popcount();
    u8x16 white_attackers_coord = white_attackers.compress(ray_coords).extract_aligned<u8x16, 0>();
    u8x16 black_attackers_coord = black_attackers.compress(ray_coords).extract_aligned<u8x16, 0>();
    return {
      PieceMask{geometry::find_set(white_attackers_coord, white_attackers_count,
                                   m_piece_list_sq[0].to_vector())},
      PieceMask{geometry::find_set(black_attackers_coord, black_attackers_count,
                                   m_piece_list_sq[1].to_vector())},
    };
}

Wordboard Position::create_attack_table_superpiece_mask(Square                   sq,
                                                        CreateSuperpieceMaskInfo cmi_arg) const {
    auto [ray_coords, ray_valid] = geometry::superpiece_rays(sq);
    u8x64 ray_places             = ray_coords.swizzle(m_board.to_vector());
    u8x64 inverse_perm           = geometry::superpiece_inverse_rays_avx2(sq);
    m8x64 ray_extent             = geometry::superpiece_attacks(ray_places, ray_valid);

    // Ordering needs to be consistent with CreateSuperpieceMaskInfo
    constexpr usize DIAG       = 1;
    constexpr usize ORTH       = 2;
    constexpr usize ORTH_NEAR  = 3;
    constexpr usize HORSE      = 4;
    constexpr usize WPAWN_NEAR = 5;
    constexpr usize BPAWN_NEAR = 6;
    static_assert(sizeof(CreateSuperpieceMaskInfo) == sizeof(u16x8));

    const u8x64 IDXS{{
      HORSE, ORTH_NEAR,  ORTH, ORTH, ORTH, ORTH, ORTH, ORTH,  // N
      HORSE, BPAWN_NEAR, DIAG, DIAG, DIAG, DIAG, DIAG, DIAG,  // NE
      HORSE, ORTH_NEAR,  ORTH, ORTH, ORTH, ORTH, ORTH, ORTH,  // E
      HORSE, WPAWN_NEAR, DIAG, DIAG, DIAG, DIAG, DIAG, DIAG,  // SE
      HORSE, ORTH_NEAR,  ORTH, ORTH, ORTH, ORTH, ORTH, ORTH,  // S
      HORSE, WPAWN_NEAR, DIAG, DIAG, DIAG, DIAG, DIAG, DIAG,  // SW
      HORSE, ORTH_NEAR,  ORTH, ORTH, ORTH, ORTH, ORTH, ORTH,  // W
      HORSE, BPAWN_NEAR, DIAG, DIAG, DIAG, DIAG, DIAG, DIAG,  // NW
    }};

    u16x8 cmi = std::bit_cast<u16x8>(cmi_arg);
    u8x16 lo  = (cmi & u16x8::splat(0xFF)).convert<u8>();
    u8x16 hi  = cmi.shr<8>().convert<u8>();

    u8x64 board_idxs = inverse_perm.swizzle(ray_extent.mask(IDXS));

    u8x64 result_lo = board_idxs.swizzle(lo);
    u8x64 result_hi = board_idxs.swizzle(hi);

    u8x64  at0 = result_lo.zip_low_128lanes(result_hi);
    u8x64  at1 = result_lo.zip_high_128lanes(result_hi);
    u16x64 at  = std::bit_cast<u16x64>(std::array<u8x64, 2>{at0, at1});

    return {at};
}

std::optional<Position> Position::parse(std::string_view str) {
    std::string        input{str};
    std::istringstream is{input};

    std::string board, color, castle, enpassant, irreversible_clock, ply;
    is >> board >> color >> castle >> enpassant >> irreversible_clock >> ply;

    return parse(board, color, castle, enpassant, irreversible_clock, ply);
}

std::optional<Position> Position::parse(std::string_view board,
                                        std::string_view color,
                                        std::string_view castle,
                                        std::string_view enpassant,
                                        std::string_view irreversible_clock,
                                        std::string_view ply) {
    Position result{};

    // Parse board
    {
        i32               place_index = 0;
        usize             i           = 0;
        std::array<u8, 2> id          = {1, 1};
        for (; place_index < 64 && i < board.size(); i++) {
            i32    file = place_index % 8;
            i32    rank = 7 - place_index / 8;
            Square sq   = Square::from_file_and_rank(file, rank);
            char   ch   = board[i];

            auto put_piece_raw = [&](Color color, PieceType ptype, u8 current_id) {
                result.m_board.mailbox[sq.raw] = Place{color, ptype, PieceId{current_id}};
                result.m_piece_list_sq[static_cast<usize>(color)].array[current_id] = sq;
                result.m_piece_list[static_cast<usize>(color)].array[current_id]    = ptype;
                place_index++;
            };

            auto put_piece = [&](Color color, PieceType ptype) -> bool {
                u8& current_id = id[static_cast<usize>(color)];
                if (current_id >= 0x10) {
                    return false;
                }
                put_piece_raw(color, ptype, current_id);
                current_id++;
                return true;
            };

            switch (ch) {
            case '/':
                if (file != 0 || place_index == 0) {
                    return std::nullopt;
                }
                break;
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
                place_index += ch - '0';
                break;
            case 'K':
                // TODO: Error if king already on board
                put_piece_raw(Color::White, PieceType::King, 0);
                break;
            case 'Q':
                if (!put_piece(Color::White, PieceType::Queen)) {
                    return std::nullopt;
                }
                break;
            case 'R':
                if (!put_piece(Color::White, PieceType::Rook)) {
                    return std::nullopt;
                }
                break;
            case 'B':
                if (!put_piece(Color::White, PieceType::Bishop)) {
                    return std::nullopt;
                }
                break;
            case 'N':
                if (!put_piece(Color::White, PieceType::Knight)) {
                    return std::nullopt;
                }
                break;
            case 'P':
                if (!put_piece(Color::White, PieceType::Pawn)) {
                    return std::nullopt;
                }
                break;
            case 'k':
                // TODO: Error if king already on board
                put_piece_raw(Color::Black, PieceType::King, 0);
                break;
            case 'q':
                if (!put_piece(Color::Black, PieceType::Queen)) {
                    return std::nullopt;
                }
                break;
            case 'r':
                if (!put_piece(Color::Black, PieceType::Rook)) {
                    return std::nullopt;
                }
                break;
            case 'b':
                if (!put_piece(Color::Black, PieceType::Bishop)) {
                    return std::nullopt;
                }
                break;
            case 'n':
                if (!put_piece(Color::Black, PieceType::Knight)) {
                    return std::nullopt;
                }
                break;
            case 'p':
                if (!put_piece(Color::Black, PieceType::Pawn)) {
                    return std::nullopt;
                }
                break;
            default:
                return std::nullopt;
            }
        }
        if (place_index != 64 || i != board.size()) {
            return std::nullopt;
        }
    }

    // Parse color
    if (color.size() != 1) {
        return std::nullopt;
    }
    switch (color[0]) {
    case 'b':
        result.m_active_color = Color::Black;
        break;
    case 'w':
        result.m_active_color = Color::White;
        break;
    default:
        return std::nullopt;
    }

    // Parse castling rights
    if (castle != "-") {
        auto verify_rook = [&](Color color, i32 file) -> Square {
            Square rook_sq    = Square::from_file_and_rank(file, color_backrank(color));
            Place  rook_place = result.m_board[rook_sq];
            if (rook_place.color() == color && rook_place.ptype() == PieceType::Rook) {
                return rook_sq;
            }
            return Square::invalid();
        };
        auto scan_for_rook = [&](Color color, i32 file, i32 direction) -> Square {
            while (file >= 0 && file <= 7) {
                Square sq    = Square::from_file_and_rank(file, color_backrank(color));
                Place  place = result.m_board[sq];
                if (place.color() == color) {
                    if (place.ptype() == PieceType::Rook) {
                        return sq;
                    }
                    if (place.ptype() == PieceType::King) {
                        return Square::invalid();
                    }
                }
                file += direction;
            }
            return Square::invalid();
        };
        for (char ch : castle) {
            if (ch == 'K') {
                result.m_rook_info[0].hside = scan_for_rook(Color::White, 7, -1);
            } else if (ch == 'Q') {
                result.m_rook_info[0].aside = scan_for_rook(Color::White, 0, +1);
            } else if (ch == 'k') {
                result.m_rook_info[1].hside = scan_for_rook(Color::Black, 7, -1);
            } else if (ch == 'q') {
                result.m_rook_info[1].aside = scan_for_rook(Color::Black, 0, +1);
            } else if (ch >= 'A' && ch <= 'H') {
                i32 rook_file = ch - 'A';
                i32 king_file = result.king_sq(Color::White).file();
                if (rook_file < king_file) {
                    result.m_rook_info[0].aside = verify_rook(Color::White, rook_file);
                } else {
                    result.m_rook_info[0].hside = verify_rook(Color::White, rook_file);
                }
            } else if (ch >= 'a' && ch <= 'h') {
                i32 rook_file = ch - 'a';
                i32 king_file = result.king_sq(Color::Black).file();
                if (rook_file < king_file) {
                    result.m_rook_info[1].aside = verify_rook(Color::Black, rook_file);
                } else {
                    result.m_rook_info[1].hside = verify_rook(Color::Black, rook_file);
                }
            } else {
                return std::nullopt;
            }
        }
    }

    // En passant
    if (enpassant != "-") {
        auto sq = Square::parse(enpassant);
        if (!sq) {
            return std::nullopt;
        }
        result.m_enpassant = *sq;
    }

    // Parse 50mr clock
    if (auto value = parse_number<i32>(irreversible_clock); value && *value <= 100) {
        result.m_50mr = static_cast<u16>(*value);
    } else {
        return std::nullopt;
    }

    // Parse game ply
    if (auto value = parse_number<i32>(ply); value && *value != 0 && *value < 10000) {
        result.m_ply = static_cast<u16>((*value - 1) * 2 + static_cast<i32>(result.m_active_color));
    } else {
        return std::nullopt;
    }

    result.m_attack_table = result.calc_attacks_slow();
    result.m_hash_key     = result.calc_hash_key_slow();
    result.m_pawn_key     = result.calc_pawn_key_slow();
    result.m_non_pawn_key = result.calc_non_pawn_key_slow();
    result.m_major_key    = result.calc_major_key_slow();
    result.m_minor_key    = result.calc_minor_key_slow();

    return result;
}

HashKey Position::calc_hash_key_slow() const {
    HashKey key = 0;

    // Iterate over all the pieces
    for (usize sq_idx = 0; sq_idx < 64; sq_idx++) {
        Place p = m_board.mailbox[sq_idx];
        if (p.is_empty()) {
            continue;
        }
        key ^= Zobrist::piece_square_zobrist[static_cast<usize>(p.color())]
                                            [static_cast<usize>(p.ptype())][sq_idx];
    }

    // Add ep if available
    if (m_enpassant != Square::invalid()) {
        key ^= Zobrist::en_passant_zobrist[m_enpassant.raw];
    }

    // Add castling
    // We may consider putting this as a separate function
    usize castle_index = m_rook_info[0].as_index() | (m_rook_info[1].as_index() << 2);
    key ^= Zobrist::castling_zobrist[castle_index];

    // Add stm
    if (m_active_color == Color::Black) {
        key ^= Zobrist::side_key;
    }

    return key;
}

HashKey Position::calc_pawn_key_slow() const {
    HashKey key = 0;
    for (usize sq_idx = 0; sq_idx < 64; sq_idx++) {
        Place p = m_board.mailbox[sq_idx];
        if (p.is_empty() || p.ptype() != PieceType::Pawn) {
            continue;
        }
        key ^= Zobrist::piece_square_zobrist[static_cast<usize>(p.color())]
                                            [static_cast<usize>(PieceType::Pawn)][sq_idx];
    }
    return key;
}

std::array<HashKey, 2> Position::calc_non_pawn_key_slow() const {
    std::array<HashKey, 2> key = {0, 0};
    for (usize sq_idx = 0; sq_idx < 64; sq_idx++) {
        Place p = m_board.mailbox[sq_idx];
        if (p.is_empty() || p.ptype() == PieceType::Pawn) {
            continue;
        }
        key[static_cast<usize>(p.color())] ^=
          Zobrist::piece_square_zobrist[static_cast<usize>(p.color())]
                                       [static_cast<usize>(p.ptype())][sq_idx];
    }
    return key;
}

HashKey Position::calc_major_key_slow() const {
    HashKey key = 0;
    for (usize sq_idx = 0; sq_idx < 64; sq_idx++) {
        Place p = m_board.mailbox[sq_idx];
        if (p.is_empty() || p.ptype() == PieceType::Pawn || p.ptype() == PieceType::Knight
            || p.ptype() == PieceType::Bishop || p.ptype() == PieceType::King) {
            continue;
        }
        key ^= Zobrist::piece_square_zobrist[static_cast<usize>(p.color())]
                                            [static_cast<usize>(p.ptype())][sq_idx];
    }
    return key;
}

HashKey Position::calc_minor_key_slow() const {
    HashKey key = 0;
    for (usize sq_idx = 0; sq_idx < 64; sq_idx++) {
        Place p = m_board.mailbox[sq_idx];
        if (p.is_empty()
            || (p.ptype() != PieceType::Knight && p.ptype() != PieceType::Bishop
                && p.ptype() != PieceType::King)) {
            continue;
        }
        key ^= Zobrist::piece_square_zobrist[static_cast<usize>(p.color())]
                                            [static_cast<usize>(p.ptype())][sq_idx];
    }
    return key;
}

std::ostream& operator<<(std::ostream& os, const Position& position) {
    i32  blanks      = 0;
    auto emit_blanks = [&] {
        if (blanks != 0) {
            os << blanks;
            blanks = 0;
        }
    };

    for (i32 rank = 7; rank >= 0; rank--) {
        for (i32 file = 0; file < 8; file++) {
            Square sq = Square::from_file_and_rank(file, rank);
            Place  p  = position.m_board.mailbox[sq.raw];

            if (p.is_empty()) {
                blanks++;
            } else {
                emit_blanks();
                os << p.to_char();
            }

            if (file == 7) {
                emit_blanks();
                if (rank != 0) {
                    os << '/';
                }
            }
        }
    }

    os << ' ' << color_char(position.m_active_color) << ' ';

    RookInfo white_rook_info = position.rook_info(Color::White);
    RookInfo black_rook_info = position.rook_info(Color::Black);
    if (white_rook_info.is_clear() && black_rook_info.is_clear()) {
        os << '-';
    }
    if (white_rook_info.hside.is_valid()) {
        os << static_cast<char>(g_frc ? white_rook_info.hside.file() + 'A' : 'K');
    }
    if (white_rook_info.aside.is_valid()) {
        os << static_cast<char>(g_frc ? white_rook_info.aside.file() + 'A' : 'Q');
    }
    if (black_rook_info.hside.is_valid()) {
        os << static_cast<char>(g_frc ? black_rook_info.hside.file() + 'a' : 'k');
    }
    if (black_rook_info.aside.is_valid()) {
        os << static_cast<char>(g_frc ? black_rook_info.aside.file() + 'a' : 'q');
    }

    if (position.m_enpassant.is_valid()) {
        os << ' ' << position.m_enpassant << ' ';
    } else {
        os << " - ";
    }

    os << position.m_50mr << ' ' << (position.m_ply / 2 + 1);

    return os;
}

bool Position::is_reversible(Move move) {
    return !(move.is_capture() || move.is_promotion() || move.is_castle()
             || (m_board[move.from()].ptype() == PieceType::Pawn));
}

u16 Position::get_50mr_counter() const {
    return m_50mr;
}

}  // namespace Clockwork

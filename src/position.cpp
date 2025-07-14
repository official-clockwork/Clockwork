#include "position.hpp"

#include <array>
#include <bit>
#include <iostream>
#include <sstream>

#include "board.hpp"
#include "common.hpp"
#include "geometry.hpp"
#include "util/parse.hpp"
#include "util/types.hpp"
#include "zobrist.hpp"

namespace Clockwork {

void Position::incrementally_remove_piece(bool color, PieceId id, Square from) {
    remove_attacks(color, id);
    toggle_rays(from);

    // TODO: check if some speed left on the table for zobrist here
    m_hash_key ^=
      Zobrist::piece_square_zobrist[static_cast<size_t>(m_board[from].color())]
                                   [static_cast<size_t>(m_board[from].ptype())][from.raw];
    m_board[from] = Place::empty();
}

void Position::incrementally_add_piece(bool color, Place p, Square to) {
    // TODO: check if some speed left on the table for zobrist here
    m_board[to] = p;
    m_hash_key ^= Zobrist::piece_square_zobrist[static_cast<size_t>(m_board[to].color())]
                                               [static_cast<size_t>(m_board[to].ptype())][to.raw];

    v512 m = toggle_rays(to);
    add_attacks(color, p.id(), to, p.ptype(), m);
}

void Position::incrementally_mutate_piece(
  bool old_color, PieceId old_id, Square sq, bool new_color, Place p) {
    // TODO: check if some speed left on the table for zobrist here
    m_hash_key ^= Zobrist::piece_square_zobrist[static_cast<size_t>(m_board[sq].color())]
                                               [static_cast<size_t>(m_board[sq].ptype())][sq.raw];
    m_board[sq] = p;
    m_hash_key ^= Zobrist::piece_square_zobrist[static_cast<size_t>(m_board[sq].color())]
                                               [static_cast<size_t>(m_board[sq].ptype())][sq.raw];

    remove_attacks(old_color, old_id);
    add_attacks(new_color, p.id(), sq, p.ptype());
}

void Position::incrementally_move_piece(bool color, Square from, Square to, Place p) {
    remove_attacks(color, p.id());

    auto [src_ray_coords, src_ray_valid] = geometry::superpiece_rays(from);
    auto [dst_ray_coords, dst_ray_valid] = geometry::superpiece_rays(to);
    v512 src_ray_places                  = v512::permute8(src_ray_coords, m_board.to_vec());

    // TODO: check if some speed left on the table for zobrist here
    m_hash_key ^=
      Zobrist::piece_square_zobrist[static_cast<size_t>(m_board[from].color())]
                                   [static_cast<size_t>(m_board[from].ptype())][from.raw];
    m_board[from] = Place::empty();
    m_board[to]   = p;
    m_hash_key ^= Zobrist::piece_square_zobrist[static_cast<size_t>(m_board[to].color())]
                                               [static_cast<size_t>(m_board[to].ptype())][to.raw];

    v512 dst_ray_places = v512::permute8(dst_ray_coords, m_board.to_vec());

    v512 src_all_sliders     = geometry::slider_mask(src_ray_places);
    v512 dst_all_sliders     = geometry::slider_mask(dst_ray_places);
    v512 src_raymask         = geometry::superpiece_attacks(src_ray_places, src_ray_valid);
    v512 dst_raymask         = geometry::superpiece_attacks(dst_ray_places, dst_ray_valid);
    v512 src_visible_sliders = src_raymask & src_all_sliders & src_ray_places;
    v512 dst_visible_sliders = dst_raymask & dst_all_sliders & dst_ray_places;

    v512 src_slider_ids = v512::sliderbroadcast(src_visible_sliders & v512::broadcast8(0x1F));
    v512 dst_slider_ids = v512::sliderbroadcast(dst_visible_sliders & v512::broadcast8(0x1F));

    src_slider_ids = v512{src_slider_ids.raw[1], src_slider_ids.raw[0]} & src_raymask;  // flip rays
    dst_slider_ids = v512{dst_slider_ids.raw[1], dst_slider_ids.raw[0]} & dst_raymask;  // flip rays
    dst_slider_ids |= dst_raymask & v512::broadcast8(0x20);  // pack information for efficiency

    v512 src_inv_perm = geometry::superpiece_inverse_rays_avx2(from);
    v512 dst_inv_perm = geometry::superpiece_inverse_rays_avx2(to);

    // Transform into board layout
    src_slider_ids = v512::permute8(src_inv_perm, src_slider_ids);
    dst_slider_ids = v512::permute8(dst_inv_perm, dst_slider_ids);

    // Recover color information
    v512 src_color = v512::eq8_vm(src_slider_ids & v512::broadcast8(0x10), v512::broadcast8(0x10));
    v512 dst_color = v512::eq8_vm(dst_slider_ids & v512::broadcast8(0x10), v512::broadcast8(0x10));
    // Recover ray mask information
    v512 ret = v512::eq8_vm(dst_slider_ids & v512::broadcast8(0x20), v512::broadcast8(0x20));

    // AVX2 doesn't have a variable word shift, so were're doing it this way.
    // Index zero is invalid here (the king is never a slider), so 0 converts to 0.
    static const v128 BITS_LO{std::array<u8, 16>{0x00, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80,
                                                 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};
    static const v128 BITS_HI{std::array<u8, 16>{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80}};
    v512              src_at_lo = v512::permute8(src_slider_ids, BITS_LO);
    v512              src_at_hi = v512::permute8(src_slider_ids, BITS_HI);
    v512              dst_at_lo = v512::permute8(dst_slider_ids, BITS_LO);
    v512              dst_at_hi = v512::permute8(dst_slider_ids, BITS_HI);

    v512 src_color0 = v512::unpacklo8(src_color, src_color);
    v512 src_color1 = v512::unpackhi8(src_color, src_color);
    v512 dst_color0 = v512::unpacklo8(dst_color, dst_color);
    v512 dst_color1 = v512::unpackhi8(dst_color, dst_color);

    v512 src_at0 = v512::unpacklo8(src_at_lo, src_at_hi);
    v512 src_at1 = v512::unpackhi8(src_at_lo, src_at_hi);
    v512 dst_at0 = v512::unpacklo8(dst_at_lo, dst_at_hi);
    v512 dst_at1 = v512::unpackhi8(dst_at_lo, dst_at_hi);

    m_attack_table[0].raw[0] ^=
      (v512::andnot(src_color0, src_at0)) ^ (v512::andnot(dst_color0, dst_at0));
    m_attack_table[0].raw[1] ^=
      (v512::andnot(src_color1, src_at1)) ^ (v512::andnot(dst_color1, dst_at1));
    m_attack_table[1].raw[0] ^= (src_color0 & src_at0) ^ (dst_color0 & dst_at0);
    m_attack_table[1].raw[1] ^= (src_color1 & src_at1) ^ (dst_color1 & dst_at1);

    add_attacks(color, p.id(), to, p.ptype(), ret);
}

void Position::remove_attacks(bool color, PieceId id) {
    v512 mask = v512::broadcast16(~id.to_piece_mask());
    m_attack_table[color].raw[0] &= mask;
    m_attack_table[color].raw[1] &= mask;
}

v512 Position::toggle_rays(Square sq) {
    auto [ray_coords, ray_valid] = geometry::superpiece_rays(sq);
    v512 ray_places              = v512::permute8(ray_coords, m_board.to_vec());

    v512 all_sliders     = geometry::slider_mask(ray_places);
    v512 raymask         = geometry::superpiece_attacks(ray_places, ray_valid);
    v512 visible_sliders = raymask & all_sliders & ray_places;

    v512 slider_ids = v512::sliderbroadcast(visible_sliders & v512::broadcast8(0x1F));

    slider_ids = v512{slider_ids.raw[1], slider_ids.raw[0]} & raymask;  // flip rays
    slider_ids |= raymask & v512::broadcast8(0x20);  // pack information for efficiency

    v512 inv_perm = geometry::superpiece_inverse_rays_avx2(sq);

    // Transform into board layout
    slider_ids = v512::permute8(inv_perm, slider_ids);

    // Recover color information
    v512 color = v512::eq8_vm(slider_ids & v512::broadcast8(0x10), v512::broadcast8(0x10));
    // Recover ray mask information
    v512 ret = v512::eq8_vm(slider_ids & v512::broadcast8(0x20), v512::broadcast8(0x20));

    // AVX2 doesn't have a variable word shift, so were're doing it this way.
    // Index zero is invalid here (the king is never a slider), so 0 converts to 0.
    static const v128 BITS_LO{std::array<u8, 16>{0x00, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80,
                                                 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};
    static const v128 BITS_HI{std::array<u8, 16>{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80}};
    v512              at_lo = v512::permute8(slider_ids, BITS_LO);
    v512              at_hi = v512::permute8(slider_ids, BITS_HI);

    v512 color0 = v512::unpacklo8(color, color);
    v512 color1 = v512::unpackhi8(color, color);

    v512 at0 = v512::unpacklo8(at_lo, at_hi);
    v512 at1 = v512::unpackhi8(at_lo, at_hi);

    m_attack_table[0].raw[1] ^= v512::andnot(color1, at1);
    m_attack_table[0].raw[0] ^= v512::andnot(color0, at0);
    m_attack_table[1].raw[0] ^= color0 & at0;
    m_attack_table[1].raw[1] ^= color1 & at1;

    return ret;
}

void Position::add_attacks(bool color, PieceId id, Square sq, PieceType ptype) {
    switch (ptype) {
    case PieceType::None:
        return;
    case PieceType::Pawn:
    case PieceType::Knight:
    case PieceType::King:
        add_attacks(color, id, sq, ptype, v512::broadcast8(0xFF));
        return;
    case PieceType::Bishop:
    case PieceType::Rook:
    case PieceType::Queen: {
        auto [ray_coords, ray_valid] = geometry::superpiece_rays(sq);
        v512 ray_places              = v512::permute8(ray_coords, m_board.to_vec());
        v512 raymask                 = geometry::superpiece_attacks(ray_places, ray_valid);

        v512 inv_perm  = geometry::superpiece_inverse_rays_avx2(sq);
        v512 boardmask = v512::permute8(inv_perm, raymask);

        add_attacks(color, id, sq, ptype, boardmask);
        return;
    }
    }
}

void Position::add_attacks(bool color, PieceId id, Square sq, PieceType ptype, v512 mask) {
    v512 moves = geometry::piece_moves_avx2(color, ptype, sq) & mask;

    v512 m0 = v512::unpacklo8(moves, moves);
    v512 m1 = v512::unpackhi8(moves, moves);

    v512 bit = v512::broadcast16(id.to_piece_mask());
    m_attack_table[color].raw[0] |= bit & m0;
    m_attack_table[color].raw[1] |= bit & m1;
}

Position Position::move(Move m) const {
    Position new_pos = *this;

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
    size_t old_castle_index =
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
        new_pos.incrementally_move_piece(color, from, to, src);

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
        new_pos.incrementally_remove_piece(color, src.id(), from);
        new_pos.incrementally_mutate_piece(!color, dst.id(), to, color, src);

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
        new_pos.incrementally_remove_piece(color, king_id, king_from);
        new_pos.incrementally_remove_piece(color, rook_id, rook_from);
        new_pos.incrementally_add_piece(color, king_place, king_to);
        new_pos.incrementally_add_piece(color, rook_place, rook_to);

        new_pos.m_piece_list_sq[color][king_id] = king_to;
        new_pos.m_piece_list_sq[color][rook_id] = rook_to;

        new_pos.m_50mr++;
        new_pos.m_rook_info[color].clear();
        break;
    }
    case MoveFlags::EnPassant: {
        Square victim_sq = Square::from_file_and_rank(m_enpassant.file(), from.rank());
        Place  victim    = m_board[victim_sq];

        new_pos.incrementally_remove_piece(!color, victim.id(), victim_sq);
        new_pos.incrementally_move_piece(color, from, to, src);

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

        new_pos.incrementally_move_piece(color, from, to, new_place);

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

        new_pos.incrementally_remove_piece(color, src.id(), from);
        new_pos.incrementally_mutate_piece(!color, dst.id(), to, color, new_place);

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
    size_t new_castle_index =
      new_pos.m_rook_info[0].as_index() | (new_pos.m_rook_info[1].as_index() << 2);
    new_pos.m_hash_key ^= Zobrist::castling_zobrist[new_castle_index];

    new_pos.m_active_color = invert(m_active_color);
    new_pos.m_ply++;

    return new_pos;
}

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
    v512 ray_places              = v512::permute8(ray_coords, m_board.to_vec());
    v512 inverse_perm            = geometry::superpiece_inverse_rays_avx2(king_square);

    // Ignore horse moves
    ray_valid &= v512::broadcast64(0xFFFFFFFFFFFFFF00);

    v512 occupied = v512::andnot(v512::eq8_vm(ray_places, v512::zero()), ray_valid);

    v512 color_mask  = v512::broadcast8(Place::COLOR_MASK);
    v512 enemy_color = v512::broadcast8(m_active_color == Color::White ? Place::COLOR_MASK : 0);
    v512 enemy       = occupied & v512::eq8_vm(ray_places & color_mask, enemy_color);

    v512 closest      = occupied & geometry::superpiece_attacks(ray_places, ray_valid);
    v512 maybe_pinned = v512::andnot(enemy, closest);

    // Find enemy sliders of the correct type
    v512 maybe_pinner1 = enemy & geometry::slider_mask(ray_places);

    // Find second-closest pieces along each ray
    v512 not_closest   = v512::andnot(closest, occupied);
    v512 pin_raymask   = geometry::superpiece_attacks(not_closest, ray_valid);
    v512 maybe_pinner2 = not_closest & pin_raymask;

    // Pinners are second-closest pieces that are enemy sliders of the correct type.
    v512 pinner = maybe_pinner1 & maybe_pinner2;

    // Does this ray have a pinner?
    v512 no_pinner_mask = v512::eq64_vm(pinner, v512::zero());
    v512 pinned         = v512::andnot(no_pinner_mask, maybe_pinned);

    v512 nonmasked_pinned_ids = v512::lanebroadcast(pinned & ray_places & v512::broadcast8(0xF));
    v512 pinned_ids           = pin_raymask & nonmasked_pinned_ids;
    // Transform into board layout
    pinned_ids = v512::permute8(inverse_perm, pinned_ids);

    u16 nonpinned_piece_mask =
      static_cast<u16>(~v512::reduceor64(v512::shl64(v512::broadcast64(1),
                                                     nonmasked_pinned_ids & v512::broadcast64(0xF)))
                       | 1);

    // AVX2 doesn't have a variable word shift, so were're doing it this way.
    // Index zero is invalid here (the king is never pinned), so 0 converts to 0.
    static const v128 BITS_LO{std::array<u8, 16>{0x00, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80,
                                                 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};
    static const v128 BITS_HI{std::array<u8, 16>{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80}};
    v512              at_lo = v512::permute8(pinned_ids, BITS_LO);
    v512              at_hi = v512::permute8(pinned_ids, BITS_HI);

    v512 nppm = v512::broadcast16(nonpinned_piece_mask);

    v512 at0 = v512::unpacklo8(at_lo, at_hi) | nppm;
    v512 at1 = v512::unpackhi8(at_lo, at_hi) | nppm;

    u64 pinned_bb = concat64(v512::neq16(at0, nppm), v512::neq16(at1, nppm));

    return {Wordboard{at0, at1}, Bitboard{pinned_bb}};
}

const std::array<Wordboard, 2> Position::calc_attacks_slow() {
    std::array<std::array<u16, 64>, 2> result{};
    for (usize i = 0; i < 64; i++) {
        Square sq{static_cast<u8>(i)};
        auto [white, black] = calc_attacks_slow(sq);
        result[0][i]        = white;
        result[1][i]        = black;
    }
    return std::bit_cast<std::array<Wordboard, 2>>(result);
}

const std::array<u16, 2> Position::calc_attacks_slow(Square sq) {
    auto [ray_coords, ray_valid] = geometry::superpiece_rays(sq);
    v512 ray_places              = v512::permute8(ray_coords, m_board.to_vec());

    u64  color             = v512::test8(ray_places, v512::broadcast8(0x10));
    v512 visible           = geometry::superpiece_attacks(ray_places, ray_valid);
    v512 attackers         = geometry::attackers_from_rays(ray_places);
    u64  visible_attackers = (visible & attackers).nonzero8();
    u64  white_attackers   = ~color & visible_attackers;
    u64  black_attackers   = color & visible_attackers;

    i32  white_attackers_count = std::popcount(white_attackers);
    i32  black_attackers_count = std::popcount(black_attackers);
    v128 white_attackers_coord = v512::compress8(white_attackers, ray_coords).to128();
    v128 black_attackers_coord = v512::compress8(black_attackers, ray_coords).to128();
    return {
      findset8(white_attackers_coord, white_attackers_count, m_piece_list_sq[0].to_vec()),
      findset8(black_attackers_coord, black_attackers_count, m_piece_list_sq[1].to_vec()),
    };
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
    // TODO: FRC, Error detection
    if (castle != "-") {
        for (char ch : castle) {
            switch (ch) {
            case 'K':
            case 'H':
                result.m_rook_info[0].hside = *Square::parse("h1");
                break;
            case 'Q':
            case 'A':
                result.m_rook_info[0].aside = *Square::parse("a1");
                break;
            case 'k':
            case 'h':
                result.m_rook_info[1].hside = *Square::parse("h8");
                break;
            case 'q':
            case 'a':
                result.m_rook_info[1].aside = *Square::parse("a8");
                break;
            default:
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

    return result;
}

HashKey Position::calc_hash_key_slow() const {
    HashKey key = 0;

    // Iterate over all the pieces
    for (size_t sq_idx = 0; sq_idx < 64; sq_idx++) {
        Place p = m_board.mailbox[sq_idx];
        if (p.is_empty()) {
            continue;
        }
        key ^= Zobrist::piece_square_zobrist[static_cast<size_t>(p.color())]
                                            [static_cast<size_t>(p.ptype())][sq_idx];
    }

    // Add ep if available
    if (m_enpassant != Square::invalid()) {
        key ^= Zobrist::en_passant_zobrist[m_enpassant.raw];
    }

    // Add castling
    // We may consider putting this as a separate function
    size_t castle_index = m_rook_info[0].as_index() | (m_rook_info[1].as_index() << 2);
    key ^= Zobrist::castling_zobrist[castle_index];

    // Add stm
    if (m_active_color == Color::Black) {
        key ^= Zobrist::side_key;
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

    // TODO: FRC
    RookInfo white_rook_info = position.rook_info(Color::White);
    RookInfo black_rook_info = position.rook_info(Color::Black);
    if (white_rook_info.is_clear() && black_rook_info.is_clear()) {
        os << '-';
    }
    if (white_rook_info.hside.is_valid()) {
        os << 'K';
    }
    if (white_rook_info.aside.is_valid()) {
        os << 'Q';
    }
    if (black_rook_info.hside.is_valid()) {
        os << 'k';
    }
    if (black_rook_info.aside.is_valid()) {
        os << 'q';
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

}

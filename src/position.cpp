#include "position.hpp"

#include <array>
#include <bit>
#include <iostream>
#include <sstream>

#include "board.hpp"
#include "geometry.hpp"
#include "util/types.hpp"

namespace Clockwork {

void Position::incrementally_remove_piece(bool color, PieceId id, Square from) {
    remove_attacks(color, id);
    toggle_rays(from);

    m_board[from] = Place::empty();
}

void Position::incrementally_add_piece(bool color, Place p, Square to) {
    m_board[to] = p;

    v512 m = toggle_rays(to);
    add_attacks(color, p.id(), to, p.ptype(), m);
}

void Position::incrementally_mutate_piece(
  bool old_color, PieceId old_id, Square sq, bool new_color, Place p) {
    m_board[sq] = p;

    remove_attacks(old_color, old_id);
    add_attacks(new_color, p.id(), sq, p.ptype());
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

    if (m_enpassant.is_valid()) {
        new_pos.m_enpassant = Square::invalid();
    }

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
        new_pos.incrementally_remove_piece(color, src.id(), from);
        new_pos.incrementally_add_piece(color, src, to);

        new_pos.m_piece_list_sq[color][src.id()] = to;

        if (src.ptype() == PieceType::Pawn) {
            new_pos.m_50mr = 0;
            if (from.raw - to.raw == 16 || to.raw - from.raw == 16) {
                new_pos.m_enpassant = Square{static_cast<u8>((from.raw + to.raw) / 2)};
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

        new_pos.incrementally_remove_piece(color, src.id(), from);
        new_pos.incrementally_remove_piece(!color, victim.id(), victim_sq);
        new_pos.incrementally_add_piece(color, src, to);

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

        new_pos.incrementally_remove_piece(color, src.id(), from);
        new_pos.incrementally_add_piece(color, new_place, to);

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

    new_pos.m_active_color = invert(m_active_color);
    new_pos.m_ply++;

    return new_pos;
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

    int  white_attackers_count = std::popcount(white_attackers);
    int  black_attackers_count = std::popcount(black_attackers);
    v128 white_attackers_coord = v512::compress8(white_attackers, ray_coords).to128();
    v128 black_attackers_coord = v512::compress8(black_attackers, ray_coords).to128();
    return {
      findset8(white_attackers_coord, white_attackers_count, m_piece_list_sq[0].to_vec()),
      findset8(black_attackers_coord, black_attackers_count, m_piece_list_sq[1].to_vec()),
    };
}

std::optional<Position> Position::parse(std::string_view str) {
    std::istringstream is{std::string{str}};

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
        int               place_index = 0;
        usize             i           = 0;
        std::array<u8, 2> id          = {1, 1};
        for (; place_index < 64 && i < board.size(); i++) {
            int    file = place_index % 8;
            int    rank = 7 - place_index / 8;
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
    if (int value = std::stoi(std::string{irreversible_clock}); value <= 100) {
        result.m_50mr = static_cast<u16>(value);
    } else {
        return std::nullopt;
    }

    // Parse game ply
    if (int value = std::stoi(std::string{ply}); value != 0 && value < 10000) {
        result.m_ply = static_cast<u16>((value - 1) * 2 + static_cast<int>(result.m_active_color));
    } else {
        return std::nullopt;
    }

    result.m_attack_table = result.calc_attacks_slow();

    return result;
}

std::ostream& operator<<(std::ostream& os, const Position& position) {
    int  blanks      = 0;
    auto emit_blanks = [&] {
        if (blanks != 0) {
            os << blanks;
            blanks = 0;
        }
    };

    for (int rank = 7; rank >= 0; rank--) {
        for (int file = 0; file < 8; file++) {
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

}

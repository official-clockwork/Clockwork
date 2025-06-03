#include "position.hpp"

#include <array>
#include <bit>
#include <sstream>
#include <iostream>

#include "board.hpp"
#include "geometry.hpp"
#include "util/types.hpp"

namespace Clockwork {

Position Position::move(Move m) const {
    Position new_pos = *this;

    const Square from  = m.from();
    const Square to    = m.to();
    const Place  src   = m_board[from];
    const Place  dst   = m_board[to];
    const usize  color = static_cast<usize>(m_active_color);

    if (m_enpassant.isValid())
        new_pos.m_enpassant = Square::invalid();

    const auto check_src_castling_rights = [&] {
        if (src.ptype() == PieceType::rook)
            new_pos.m_rook_info[color].unset(from);
        else if (src.ptype() == PieceType::king)
            new_pos.m_rook_info[color].clear();
    };

    const auto check_dst_castling_rights = [&] {
        if (dst.ptype() == PieceType::rook)
            new_pos.m_rook_info[!color].unset(to);
    };

    switch (m.flags())
    {
    case MoveFlags::normal :
        new_pos.m_board[from]                    = Place::empty();
        new_pos.m_board[to]                      = src;
        new_pos.m_piece_list_sq[color][src.id()] = to;
        if (src.ptype() == PieceType::pawn)
        {
            new_pos.m_50mr = 0;
            if (from.raw - to.raw == 16 || to.raw - from.raw == 16)
                new_pos.m_enpassant = Square{static_cast<u8>((from.raw + to.raw) / 2)};
        }
        else
        {
            new_pos.m_50mr++;
            check_src_castling_rights();
        }
        break;
    case MoveFlags::capture_bit :
        new_pos.m_board[from]                     = Place::empty();
        new_pos.m_board[to]                       = src;
        new_pos.m_piece_list_sq[color][src.id()]  = to;
        new_pos.m_piece_list_sq[!color][dst.id()] = Square::invalid();
        new_pos.m_piece_list[!color][dst.id()]    = PieceType::none;
        new_pos.m_50mr                            = 0;
        check_src_castling_rights();
        check_dst_castling_rights();
        break;
    case MoveFlags::castle : {
        const bool    aside     = to.file() < from.file();
        const PieceId king_id   = PieceId{0};  // == src.id()
        const PieceId rook_id   = dst.id();
        const Square  king_from = from;
        const Square  rook_from = to;
        const Square  king_to   = Square::fromFileAndRank(aside ? 2 : 6, from.rank());
        const Square  rook_to   = Square::fromFileAndRank(aside ? 3 : 5, from.rank());

        new_pos.m_board[king_from]              = Place::empty();
        new_pos.m_board[rook_from]              = Place::empty();
        new_pos.m_board[king_to]                = Place{m_active_color, PieceType::king, king_id};
        new_pos.m_board[rook_to]                = Place{m_active_color, PieceType::rook, rook_id};
        new_pos.m_piece_list_sq[color][king_id] = king_to;
        new_pos.m_piece_list_sq[color][rook_id] = rook_to;
        new_pos.m_50mr++;
        new_pos.m_rook_info[color].clear();
        break;
    }
    case MoveFlags::en_passant : {
        const Square victim_sq     = Square::fromFileAndRank(m_enpassant.file(), from.rank());
        const Place  victim        = m_board[victim_sq];
        new_pos.m_board[from]      = Place::empty();
        new_pos.m_board[victim_sq] = Place::empty();
        new_pos.m_board[to]        = src;
        new_pos.m_piece_list_sq[color][src.id()]     = to;
        new_pos.m_piece_list_sq[!color][victim.id()] = Square::invalid();
        new_pos.m_piece_list[!color][victim.id()]    = PieceType::none;
        new_pos.m_50mr                               = 0;
        break;
    }
    case MoveFlags::promo_knight :
    case MoveFlags::promo_bishop :
    case MoveFlags::promo_rook :
    case MoveFlags::promo_queen :
        new_pos.m_board[from]                    = Place::empty();
        new_pos.m_board[to]                      = Place{m_active_color, *m.promo(), src.id()};
        new_pos.m_piece_list_sq[color][src.id()] = to;
        new_pos.m_piece_list[color][src.id()]    = *m.promo();
        new_pos.m_50mr                           = 0;
        break;
    case MoveFlags::promo_knight_capture :
    case MoveFlags::promo_bishop_capture :
    case MoveFlags::promo_rook_capture :
    case MoveFlags::promo_queen_capture :
        new_pos.m_board[from]                     = Place::empty();
        new_pos.m_board[to]                       = Place{m_active_color, *m.promo(), src.id()};
        new_pos.m_piece_list_sq[color][src.id()]  = to;
        new_pos.m_piece_list[color][src.id()]     = *m.promo();
        new_pos.m_piece_list_sq[!color][dst.id()] = Square::invalid();
        new_pos.m_piece_list[!color][dst.id()]    = PieceType::none;
        new_pos.m_50mr                            = 0;
        check_dst_castling_rights();
        break;
    }

    new_pos.m_active_color = invert(m_active_color);
    new_pos.m_ply++;

    // TODO: Do this incrementally
    new_pos.m_attack_table = new_pos.calcAttacksSlow();

    return new_pos;
}

const std::array<Wordboard, 2> Position::calcAttacksSlow() {
    std::array<Wordboard, 2> result{};
    for (usize i = 0; i < 64; i++)
    {
        const Square sq{static_cast<u8>(i)};
        const auto [white, black] = calcAttacksSlow(sq);
        result[0].mailbox[i]      = white;
        result[1].mailbox[i]      = black;
    }
    return result;
}

const std::array<u16, 2> Position::calcAttacksSlow(Square sq) {
    const auto [ray_coords, ray_valid] = geometry::superpieceRays(sq);
    const v512 ray_places              = v512::permute8(ray_coords, m_board.toVec());

    const u64 occupied = ray_places.nonzero8();
    const u64 color    = ray_places.msb8();

    const u64 visible = geometry::superpieceAttacks(occupied, ray_valid) & occupied;

    const u64 attackers       = geometry::attackersFromRays(ray_places);
    const u64 white_attackers = ~color & visible & attackers;
    const u64 black_attackers = color & visible & attackers;

    const int  white_attackers_count = std::popcount(white_attackers);
    const int  black_attackers_count = std::popcount(black_attackers);
    const v128 white_attackers_coord = v512::compress8(white_attackers, ray_coords).to128();
    const v128 black_attackers_coord = v512::compress8(black_attackers, ray_coords).to128();
    return {
      findset8(white_attackers_coord, white_attackers_count, m_piece_list_sq[0].toVec()),
      findset8(black_attackers_coord, black_attackers_count, m_piece_list_sq[1].toVec()),
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
        for (; place_index < 64 && i < board.size(); i++)
        {
            const int    file = place_index % 8;
            const int    rank = 7 - place_index / 8;
            const Square sq   = Square::fromFileAndRank(file, rank);
            const char   ch   = board[i];

            const auto put_piece_raw = [&](Color color, PieceType ptype, u8 current_id) {
                result.m_board.mailbox[sq.raw] = Place{color, ptype, PieceId{current_id}};
                result.m_piece_list_sq[static_cast<usize>(color)].array[current_id] = sq;
                result.m_piece_list[static_cast<usize>(color)].array[current_id]    = ptype;
                place_index++;
            };

            const auto put_piece = [&](Color color, PieceType ptype) -> bool {
                u8& current_id = id[static_cast<usize>(color)];
                if (current_id >= 0x10)
                    return false;
                put_piece_raw(color, ptype, current_id);
                current_id++;
                return true;
            };

            switch (ch)
            {
            case '/' :
                if (file != 0 || place_index == 0)
                    return std::nullopt;
                break;
            case '1' :
            case '2' :
            case '3' :
            case '4' :
            case '5' :
            case '6' :
            case '7' :
            case '8' :
                place_index += ch - '0';
                break;
            case 'K' :
                // TODO: Error if king already on board
                put_piece_raw(Color::white, PieceType::king, 0);
                break;
            case 'Q' :
                if (!put_piece(Color::white, PieceType::queen))
                    return std::nullopt;
                break;
            case 'R' :
                if (!put_piece(Color::white, PieceType::rook))
                    return std::nullopt;
                break;
            case 'B' :
                if (!put_piece(Color::white, PieceType::bishop))
                    return std::nullopt;
                break;
            case 'N' :
                if (!put_piece(Color::white, PieceType::knight))
                    return std::nullopt;
                break;
            case 'P' :
                if (!put_piece(Color::white, PieceType::pawn))
                    return std::nullopt;
                break;
            case 'k' :
                // TODO: Error if king already on board
                put_piece_raw(Color::black, PieceType::king, 0);
                break;
            case 'q' :
                if (!put_piece(Color::black, PieceType::queen))
                    return std::nullopt;
                break;
            case 'r' :
                if (!put_piece(Color::black, PieceType::rook))
                    return std::nullopt;
                break;
            case 'b' :
                if (!put_piece(Color::black, PieceType::bishop))
                    return std::nullopt;
                break;
            case 'n' :
                if (!put_piece(Color::black, PieceType::knight))
                    return std::nullopt;
                break;
            case 'p' :
                if (!put_piece(Color::black, PieceType::pawn))
                    return std::nullopt;
                break;
            default :
                return std::nullopt;
            }
        }
        if (place_index != 64 || i != board.size())
            return std::nullopt;
    }

    // Parse color
    if (color.size() != 1)
        return std::nullopt;
    switch (color[0])
    {
    case 'b' :
        result.m_active_color = Color::black;
        break;
    case 'w' :
        result.m_active_color = Color::white;
        break;
    default :
        return std::nullopt;
    }

    // Parse castling rights
    // TODO: FRC, Error detection
    if (castle != "-")
    {
        for (const char ch : castle)
        {
            switch (ch)
            {
            case 'K' :
            case 'H' :
                result.m_rook_info[0].hside = *Square::parse("h1");
                break;
            case 'Q' :
            case 'A' :
                result.m_rook_info[0].aside = *Square::parse("a1");
                break;
            case 'k' :
            case 'h' :
                result.m_rook_info[1].hside = *Square::parse("h8");
                break;
            case 'q' :
            case 'a' :
                result.m_rook_info[1].aside = *Square::parse("a8");
                break;
            default :
                return std::nullopt;
            }
        }
    }

    // En passant
    if (enpassant != "-")
    {
        const auto sq = Square::parse(enpassant);
        if (!sq)
            return std::nullopt;
        result.m_enpassant = *sq;
    }

    // Parse 50mr clock
    if (const int value = std::stoi(std::string{irreversible_clock}); value <= 100)
    {
        result.m_50mr = static_cast<u16>(value);
    }
    else
    {
        return std::nullopt;
    }

    // Parse game ply
    if (const int value = std::stoi(std::string{ply}); value != 0 && value < 10000)
    {
        result.m_ply = static_cast<u16>((value - 1) * 2 + static_cast<int>(result.m_active_color));
    }
    else
    {
        return std::nullopt;
    }

    result.m_attack_table = result.calcAttacksSlow();

    return result;
}

std::ostream& operator<<(std::ostream& os, const Position& position) {
    int        blanks      = 0;
    const auto emit_blanks = [&] {
        if (blanks != 0)
        {
            os << blanks;
            blanks = 0;
        }
    };

    for (int rank = 7; rank >= 0; rank--)
    {
        for (int file = 0; file < 8; file++)
        {
            const Square sq = Square::fromFileAndRank(file, rank);
            const Place  p  = position.m_board.mailbox[sq.raw];

            if (p.isEmpty())
            {
                blanks++;
            }
            else
            {
                emit_blanks();
                os << p.toChar();
            }

            if (file == 7)
            {
                emit_blanks();
                if (rank != 0)
                    os << '/';
            }
        }
    }

    os << ' ' << color_char(position.m_active_color) << ' ';

    // TODO: FRC
    const RookInfo white_rook_info = position.rookInfo(Color::white);
    const RookInfo black_rook_info = position.rookInfo(Color::black);
    if (white_rook_info.isClear() && black_rook_info.isClear())
        os << '-';
    if (white_rook_info.hside.isValid())
        os << 'K';
    if (white_rook_info.aside.isValid())
        os << 'Q';
    if (black_rook_info.hside.isValid())
        os << 'k';
    if (black_rook_info.aside.isValid())
        os << 'q';

    if (position.m_enpassant.isValid())
        os << ' ' << position.m_enpassant << ' ';
    else
        os << " - ";

    os << position.m_50mr << ' ' << (position.m_ply / 2 + 1);

    return os;
}

}

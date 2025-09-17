#include "move.hpp"
#include "movegen.hpp"

#include "position.hpp"

namespace Clockwork {

std::optional<Move> Move::parse(std::string_view str, const Position& ctx) {
    if (str.size() != 4 && str.size() != 5) {
        return std::nullopt;
    }

    auto from = Square::parse(str.substr(0, 2));
    if (!from) {
        return std::nullopt;
    }

    auto to = Square::parse(str.substr(2, 2));
    if (!to) {
        return std::nullopt;
    }

    Place src = ctx.board()[*from];
    Place dst = ctx.board()[*to];

    PieceType ptype   = src.ptype();
    bool      capture = !dst.is_empty();

    if (src.color() != ctx.active_color()) {
        return std::nullopt;
    }

    if (str.size() == 4) {
        if (ptype == PieceType::Pawn) {
            if (ctx.en_passant() == *to) {
                return Move(*from, *to, MoveFlags::EnPassant);
            }
        }
        if (ptype == PieceType::King) {
            Square rook_aside = ctx.rook_info(ctx.active_color()).aside;
            Square rook_hside = ctx.rook_info(ctx.active_color()).hside;
            if (*to == rook_aside || *to == rook_hside) {
                return Move(*from, *to, MoveFlags::Castle);
            }
            if (from->file() == 4 && to->file() == 2) {
                return Move(*from, rook_aside, MoveFlags::Castle);
            }
            if (from->file() == 4 && to->file() == 6) {
                return Move(*from, rook_hside, MoveFlags::Castle);
            }
        }
        return Move(*from, *to, capture ? MoveFlags::CaptureBit : MoveFlags::Normal);
    }

    // This check needs to be here because castling is king captures rook in FRC.
    if (capture && dst.color() == ctx.active_color()) {
        return std::nullopt;
    }

    auto mf = [&]() -> std::optional<MoveFlags> {
        switch (str[4]) {
        case 'q':
            return capture ? MoveFlags::PromoQueenCapture : MoveFlags::PromoQueen;
        case 'n':
            return capture ? MoveFlags::PromoKnightCapture : MoveFlags::PromoKnight;
        case 'r':
            return capture ? MoveFlags::PromoRookCapture : MoveFlags::PromoRook;
        case 'b':
            return capture ? MoveFlags::PromoBishopCapture : MoveFlags::PromoBishop;
        default:
            return std::nullopt;
        }
    }();
    if (!mf) {
        return std::nullopt;
    }
    return Move(*from, *to, *mf);
}

std::optional<Move> Move::parseSan(std::string_view san, const Position& ctx) {
    Color stm = ctx.active_color();

    if (san.size() < 2) {
        return std::nullopt;
    }

    if (san.ends_with('#') || san.ends_with('+')) {
        san.remove_suffix(1);
    }

    if (san == "O-O") {
        Square rook_hside = ctx.rook_info(stm).hside;
        if (!rook_hside.is_valid()) {
            return std::nullopt;
        }
        return Move(ctx.king_sq(stm), rook_hside, MoveFlags::Castle);
    }

    if (san == "O-O-O") {
        Square rook_aside = ctx.rook_info(stm).aside;
        if (!rook_aside.is_valid()) {
            return std::nullopt;
        }
        return Move(ctx.king_sq(stm), rook_aside, MoveFlags::Castle);
    }

    PieceType promo = PieceType::None;
    if (san.size() >= 4 && san[san.size() - 2] == '=') {
        if (auto p = parse_piece_char(san.back())) {
            promo = *p;
        } else {
            return std::nullopt;
        }
        san.remove_suffix(2);
    }

    if (san.size() < 2) {
        return std::nullopt;
    }

    bool is_capture = san.size() >= 3 && san[san.size() - 3] == 'x';
    auto to         = Square::parse(san.substr(san.size() - 2));
    if (!to) {
        return std::nullopt;
    }
    san.remove_suffix(2 + is_capture);

    PieceType src_ptype = PieceType::None;
    switch (san.size()) {
    case 0:
        // e.g. e4
        if (is_capture) {
            return std::nullopt;
        }
        {
            i32    delta = stm == Color::White ? -8 : 8;
            Square push_src{static_cast<u8>(to->raw + delta)};
            Square double_src{static_cast<u8>(to->raw + delta * 2)};

            if (to->relative_sq(stm).rank() < 8 && ctx.board()[push_src].ptype() == PieceType::Pawn
                && ctx.board()[push_src].color() == stm) {
                if (auto mf = build_move_flags(false, false, false, promo)) {
                    return Move(push_src, *to, *mf);
                }
            } else if (to->relative_sq(stm).rank() == 3
                       && ctx.board()[double_src].ptype() == PieceType::Pawn
                       && ctx.board()[double_src].color() == stm) {
                if (auto mf = build_move_flags(false, false, false, promo)) {
                    return Move(double_src, *to, *mf);
                }
            }

            return std::nullopt;
        }

    case 1:
        if (san[0] >= 'a' && san[0] <= 'h') {
            // e.g. axb3
            if (!is_capture) {
                return std::nullopt;
            }
            src_ptype = PieceType::Pawn;
        } else if (auto p = parse_piece_char(san[0])) {
            // e.g. Bb3, Bxb3
            src_ptype = *p;
            san.remove_prefix(1);
        } else {
            return std::nullopt;
        }
        break;
    case 2:  // e.g. Qhxa3, Q3xb7, Qba4, Q6a3
    case 3:  // e.g. Qa1b2, Qa1xb2
        if (auto p = parse_piece_char(san[0])) {
            // e.g. Bb3, Bxb3
            src_ptype = *p;
        } else {
            return std::nullopt;
        }
        san.remove_prefix(1);
        break;
    default:
        return std::nullopt;
    }

    bool is_en_passant = src_ptype == PieceType::Pawn && is_capture && *to == ctx.en_passant();

    if (!is_en_passant && is_capture && ctx.board()[*to].is_empty()) {
        // Capturing with empty destination
        return std::nullopt;
    }

    MoveGen           movegen{ctx};
    std::vector<Move> candidates;
    PieceMask piece_mask = ctx.attack_table(stm).read(*to) & ctx.piece_list(stm).mask_eq(src_ptype);
    for (PieceId id : piece_mask) {
        Square from = ctx.piece_list_sq(stm)[id];
        if (from.to_string().find(san) != std::string::npos) {
            if (auto mf = build_move_flags(false, is_en_passant, is_capture, promo)) {
                Move move = Move(from, *to, *mf);
                if (movegen.is_legal(move)) {
                    candidates.push_back(move);
                }
            }
        }
    }

    if (candidates.size() != 1) {
        return std::nullopt;
    }
    return candidates[0];
}

}

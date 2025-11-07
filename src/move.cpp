#include "move.hpp"

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

}  // namespace Clockwork

#include "move.hpp"

#include "position.hpp"

namespace Clockwork {

std::optional<Move> Move::parse(std::string_view str, const Position& ctx) {
    if (str.size() != 4 && str.size() != 5)
        return std::nullopt;

    const auto from = Square::parse(str.substr(0, 2));
    if (!from)
        return std::nullopt;

    const auto to = Square::parse(str.substr(2, 2));
    if (!to)
        return std::nullopt;

    const Place src = ctx.board()[*from];
    const Place dst = ctx.board()[*to];

    const PieceType ptype   = src.ptype();
    const bool      capture = !dst.isEmpty();

    if (src.color() != ctx.activeColor())
        return std::nullopt;

    if (str.size() == 4)
    {
        if (ptype == PieceType::pawn)
        {
            if (ctx.enPassant() == *to)
                return Move(*from, *to, MoveFlags::en_passant);
        }
        if (ptype == PieceType::king)
        {
            // TOOD: FRC
            if (from->file() == 4 && to->file() == 2)
                return Move(*from, ctx.rookInfo(ctx.activeColor()).aside, MoveFlags::castle);
            if (from->file() == 4 && to->file() == 6)
                return Move(*from, ctx.rookInfo(ctx.activeColor()).hside, MoveFlags::castle);
        }
        return Move(*from, *to, capture ? MoveFlags::capture_bit : MoveFlags::normal);
    }

    // This check needs to be here because castling is king captures rook in FRC.
    if (capture && dst.color() == ctx.activeColor())
        return std::nullopt;

    const auto mf = [&]() -> std::optional<MoveFlags> {
        switch (str[4])
        {
        case 'q' :
            return capture ? MoveFlags::promo_queen_capture : MoveFlags::promo_queen;
        case 'n' :
            return capture ? MoveFlags::promo_knight_capture : MoveFlags::promo_knight;
        case 'r' :
            return capture ? MoveFlags::promo_rook_capture : MoveFlags::promo_rook;
        case 'b' :
            return capture ? MoveFlags::promo_bishop_capture : MoveFlags::promo_bishop;
        default :
            return std::nullopt;
        }
    }();
    if (!mf)
        return std::nullopt;
    return Move(*from, *to, *mf);
}

}

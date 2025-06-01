#pragma once

#include "util/types.hpp"
#include "square.hpp"
#include "common.hpp"

#include <optional>

namespace Clockwork {

static_assert(static_cast<u16>(PieceType::knight) == 1);

enum class MoveFlags : u16 {
    normal               = 0b0000 << 12,
    castle               = 0b0001 << 12,
    capture_bit          = 0b0100 << 12,
    en_passant           = 0b0110 << 12,
    promotion_bit        = 0b1000 << 12,
    promo_knight         = (0b1000 | (static_cast<u16>(PieceType::knight) - 1)) << 12,
    promo_bishop         = (0b1000 | (static_cast<u16>(PieceType::bishop) - 1)) << 12,
    promo_rook           = (0b1000 | (static_cast<u16>(PieceType::rook) - 1)) << 12,
    promo_queen          = (0b1000 | (static_cast<u16>(PieceType::queen) - 1)) << 12,
    promo_capture        = 0b1100 << 12,
    promo_knight_capture = (0b1100 | (static_cast<u16>(PieceType::knight) - 1)) << 12,
    promo_bishop_capture = (0b1100 | (static_cast<u16>(PieceType::bishop) - 1)) << 12,
    promo_rook_capture   = (0b1100 | (static_cast<u16>(PieceType::rook) - 1)) << 12,
    promo_queen_capture  = (0b1100 | (static_cast<u16>(PieceType::queen) - 1)) << 12,
};

struct Move {
    u16 raw;

    constexpr Move(Square from, Square to, MoveFlags flags = MoveFlags::normal) {
        raw = from.raw | (to.raw << 6) | static_cast<u16>(flags);
    }

    constexpr Square from() const { return Square(raw & 0x3F); }

    constexpr Square to() const { return Square((raw >> 6) & 0x3F); }

    constexpr MoveFlags flags() const { return MoveFlags{static_cast<u16>(raw & (0xF << 12))}; }

    constexpr bool is_capture() const { return raw & static_cast<u16>(MoveFlags::capture_bit); }

    constexpr bool is_promotion() const { return raw & static_cast<u16>(MoveFlags::promotion_bit); }

    constexpr std::optional<PieceType> promo() const {
        switch (flags())
        {
        case MoveFlags::promo_knight :
            return PieceType::knight;
        case MoveFlags::promo_bishop :
            return PieceType::bishop;
        case MoveFlags::promo_rook :
            return PieceType::rook;
        case MoveFlags::promo_queen :
            return PieceType::queen;
        default :
            return std::nullopt;
        }
    }

    friend std::ostream& operator<<(std::ostream& os, Move mv) {
        os << mv.from() << mv.to();
        if (auto promo = mv.promo())
        {
            os << piece_char(*promo);
        }
        return os;
    }
};

}
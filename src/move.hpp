#pragma once

#include "util/types.hpp"
#include "square.hpp"
#include "common.hpp"

#include <optional>

namespace Clockwork {

static_assert(static_cast<u16>(PieceType::Knight) == 1);

enum class MoveFlags : u16 {
    Normal             = 0b0000 << 12,
    Castle             = 0b0001 << 12,
    CaptureBit         = 0b0100 << 12,
    EnPassant          = 0b0110 << 12,
    PromotionBit       = 0b1000 << 12,
    PromoKnight        = (0b1000 | (static_cast<u16>(PieceType::Knight) - 1)) << 12,
    PromoBishop        = (0b1000 | (static_cast<u16>(PieceType::Bishop) - 1)) << 12,
    PromoRook          = (0b1000 | (static_cast<u16>(PieceType::Rook) - 1)) << 12,
    PromoQueen         = (0b1000 | (static_cast<u16>(PieceType::Queen) - 1)) << 12,
    PromoCapture       = 0b1100 << 12,
    PromoKnightCapture = (0b1100 | (static_cast<u16>(PieceType::Knight) - 1)) << 12,
    PromoBishopCapture = (0b1100 | (static_cast<u16>(PieceType::Bishop) - 1)) << 12,
    PromoRookCapture   = (0b1100 | (static_cast<u16>(PieceType::Rook) - 1)) << 12,
    PromoQueenCapture  = (0b1100 | (static_cast<u16>(PieceType::Queen) - 1)) << 12,
};

struct Move {
    u16 raw;

    constexpr Move(Square from, Square to, MoveFlags flags = MoveFlags::Normal) {
        raw = from.raw | (to.raw << 6) | static_cast<u16>(flags);
    }

    [[nodiscard]] constexpr Square from() const {
        return static_cast<Square>(raw & 0x3F);
    }

    [[nodiscard]] constexpr Square to() const {
        return static_cast<Square>((raw >> 6) & 0x3F);
    }

    [[nodiscard]] constexpr MoveFlags flags() const {
        return MoveFlags{static_cast<u16>(raw & (0xF << 12))};
    }

    [[nodiscard]] constexpr bool is_capture() const {
        return raw & static_cast<u16>(MoveFlags::CaptureBit);
    }

    [[nodiscard]] constexpr bool is_promotion() const {
        return raw & static_cast<u16>(MoveFlags::PromotionBit);
    }

    [[nodiscard]] constexpr std::optional<PieceType> promo() const {
        switch (flags()) {
        case MoveFlags::PromoKnight :
            return PieceType::Knight;
        case MoveFlags::PromoBishop :
            return PieceType::Bishop;
        case MoveFlags::PromoRook :
            return PieceType::Rook;
        case MoveFlags::PromoQueen :
            return PieceType::Queen;
        default :
            return std::nullopt;
        }
    }

    friend std::ostream& operator<<(std::ostream& os, Move mv) {
        os << mv.from() << mv.to();
        if (auto promo = mv.promo()) {
            os << piece_char(*promo);
        }
        return os;
    }
};

}
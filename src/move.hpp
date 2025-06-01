#pragma once

#include "util/types.hpp"
#include "square.hpp"
#include "common.hpp"

#include <optional>

namespace Clockwork {

enum class MoveFlags {
    // TODO: More flags
    normal,
    promo_knight,
    promo_bishop,
    promo_rook,
    promo_queen,
};

struct Move {
    u16 raw;

    constexpr Move(Square from, Square to, MoveFlags flags = MoveFlags::normal) {
        raw = from.raw | (to.raw << 6) | (static_cast<u16>(flags) << 12);
    }

    constexpr Square from() const {
        return Square(raw & 0x3F);
    }

    constexpr Square to() const {
        return Square((raw >> 6) & 0x3F);
    }

    constexpr MoveFlags flags() const {
        return MoveFlags(raw >> 12);
    }

    constexpr std::optional<Piece> promo() const {
        switch (flags()) {
            case MoveFlags::promo_knight: return Piece::knight;
            case MoveFlags::promo_bishop: return Piece::bishop;
            case MoveFlags::promo_rook: return Piece::rook;
            case MoveFlags::promo_queen: return Piece::queen;
            default: return std::nullopt;
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
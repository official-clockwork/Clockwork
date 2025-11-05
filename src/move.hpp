#pragma once

#include "common.hpp"
#include "square.hpp"
#include "util/types.hpp"
#include <optional>
#include <string_view>

namespace Clockwork {

struct Position;

static_assert(static_cast<u16>(PieceType::Knight) == 2);

enum class MoveFlags : u16 {
    Normal             = 0b0000 << 12,
    Castle             = 0b0001 << 12,
    CaptureBit         = 0b0100 << 12,
    EnPassant          = 0b0110 << 12,
    PromotionBit       = 0b1000 << 12,
    PromoKnight        = (0b1000 | (static_cast<u16>(PieceType::Knight) - 2)) << 12,
    PromoBishop        = (0b1000 | (static_cast<u16>(PieceType::Bishop) - 2)) << 12,
    PromoRook          = (0b1000 | (static_cast<u16>(PieceType::Rook) - 2)) << 12,
    PromoQueen         = (0b1000 | (static_cast<u16>(PieceType::Queen) - 2)) << 12,
    PromoCapture       = 0b1100 << 12,
    PromoKnightCapture = (0b1100 | (static_cast<u16>(PieceType::Knight) - 2)) << 12,
    PromoBishopCapture = (0b1100 | (static_cast<u16>(PieceType::Bishop) - 2)) << 12,
    PromoRookCapture   = (0b1100 | (static_cast<u16>(PieceType::Rook) - 2)) << 12,
    PromoQueenCapture  = (0b1100 | (static_cast<u16>(PieceType::Queen) - 2)) << 12,
};

struct Move {
    u16 raw          = 0;
    constexpr Move() = default;

    constexpr Move(Square from, Square to, MoveFlags flags = MoveFlags::Normal) {
        raw = static_cast<u16>(from.raw | (to.raw << 6) | static_cast<u16>(flags));
    }

    static constexpr Move none() {
        return {};
    }

    [[nodiscard]] constexpr Square from() const {
        return Square{static_cast<u8>(raw & 0x3F)};
    }

    [[nodiscard]] constexpr Square to() const {
        return Square{static_cast<u8>((raw >> 6) & 0x3F)};
    }

    [[nodiscard]] constexpr u16 from_to() const {
        return raw & 0xFFF;
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

    [[nodiscard]] constexpr bool is_castle() const {
        return flags() == MoveFlags::Castle;
    }

    [[nodiscard]] constexpr bool is_en_passant() const {
        return flags() == MoveFlags::EnPassant;
    }

    [[nodiscard]] constexpr std::optional<PieceType> promo() const {
        if (!is_promotion()) {
            return std::nullopt;
        }

        static_assert(static_cast<u16>(PieceType::Knight) == 2);
        return static_cast<PieceType>(((raw >> 12) & 0b0011) + 2);
    }

    static std::optional<Move> parse(std::string_view str, const Position& context);

    [[nodiscard]] constexpr bool operator==(const Move& other) const = default;

    [[nodiscard]] constexpr bool operator!=(const Move& other) const = default;

    friend std::ostream& operator<<(std::ostream& os, Move mv) {
        os << mv.from();

        if (!g_frc && mv.flags() == MoveFlags::Castle) {
            if (mv.to().file() < mv.from().file()) {
                os << 'c';
            } else {
                os << 'g';
            }
            os << mv.to().rank() + 1;
        } else {
            os << mv.to();
        }

        if (auto promo = mv.promo()) {
            os << piece_char(*promo);
        }
        return os;
    }
};

}  // namespace Clockwork

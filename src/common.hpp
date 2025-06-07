#pragma once

#include "util/types.hpp"

namespace Clockwork {

constexpr i32   MAX_PLY      = 256;
constexpr Value VALUE_INF    = 32501;
constexpr Value VALUE_MATED  = 32500;
constexpr Value VALUE_ISMATE = 32000;

enum class Color {
    White,
    Black
};

constexpr char color_char(Color color) {
    using enum Color;
    switch (color) {
    case White:
        return 'w';
    case Black:
        return 'b';
    }
    unreachable();
}

constexpr Color invert(Color color) {
    return static_cast<Color>(static_cast<i32>(color) ^ 1);
}

enum class PieceType : u8 {
    None,
    Pawn,
    Knight,
    Bishop,
    Rook,
    Queen,
    King
};

constexpr char piece_char(PieceType piece) {
    using enum PieceType;
    switch (piece) {
    case None:
        return '.';
    case Pawn:
        return 'p';
    case Knight:
        return 'n';
    case Bishop:
        return 'b';
    case Rook:
        return 'r';
    case Queen:
        return 'q';
    case King:
        return 'k';
    }
    unreachable();
}

constexpr bool is_slider(PieceType ptype) {
    return ptype >= PieceType::Bishop && ptype <= PieceType::Queen;
}

}

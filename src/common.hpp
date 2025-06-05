#pragma once

#include "util/types.hpp"

namespace Clockwork {

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
}

}

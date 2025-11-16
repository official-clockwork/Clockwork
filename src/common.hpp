#pragma once

#include "util/types.hpp"
#include <atomic>
#include <cmath>

namespace Clockwork {

inline std::atomic<bool> g_frc = false;

constexpr i32   MAX_PLY     = 256;
constexpr Value VALUE_INF   = 32501;
constexpr Value VALUE_MATED = 32500;
constexpr Value VALUE_WIN   = 32000;

constexpr bool is_mate_score(Value value) {
    return std::abs(value) >= VALUE_WIN;
}

constexpr bool is_being_mated_score(Value value) {
    return value <= -VALUE_WIN;
}

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

constexpr i32 color_backrank(Color color) {
    using enum Color;
    switch (color) {
    case White:
        return 0;
    case Black:
        return 7;
    }
    unreachable();
}

constexpr Color invert(Color color) {
    return static_cast<Color>(static_cast<i32>(color) ^ 1);
}

constexpr Color operator~(Color color) {
    return invert(color);
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

}  // namespace Clockwork

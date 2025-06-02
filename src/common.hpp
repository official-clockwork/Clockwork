#pragma once

namespace Clockwork {

enum class Color {
    White,
    Black
};

enum class PieceType {
    Pawn,
    Knight,
    Bishop,
    Rook,
    Queen,
    King,
    Empty,
};

constexpr char piece_char(PieceType piece) {
    using enum PieceType;
    switch (piece) {
    case Pawn :
        return 'p';
    case Knight :
        return 'n';
    case Bishop :
        return 'b';
    case Rook :
        return 'r';
    case Queen :
        return 'q';
    case King :
        return 'k';
    case Empty :
        return '.';
    }
}

}
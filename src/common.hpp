#pragma once

namespace Clockwork {

enum class Color {
    white,
    black
};

enum class PieceType {
    pawn,
    knight,
    bishop,
    rook,
    queen,
    king,
    empty,
};

constexpr char piece_char(PieceType piece) {
    using enum PieceType;
    switch (piece)
    {
    case pawn :
        return 'p';
    case knight :
        return 'n';
    case bishop :
        return 'b';
    case rook :
        return 'r';
    case queen :
        return 'q';
    case king :
        return 'k';
    case empty :
        return '.';
    }
}

}
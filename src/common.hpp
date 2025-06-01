
#include "util/types.hpp"

namespace Clockwork {

enum class Color { white, black };

enum class Piece {
    pawn,
    knight,
    bishop,
    rook,
    queen,
    king,
};

constexpr char piece_char(Piece piece) {
    switch (piece) {
        case Piece::pawn: return 'p';
        case Piece::knight: return 'n';
        case Piece::bishop: return 'b';
        case Piece::rook: return 'r';
        case Piece::queen: return 'q';
        case Piece::king: return 'k';
    }
}

}
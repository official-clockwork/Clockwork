#include <iostream>
#include <sstream>
#include <string_view>
#include <tuple>
#include <vector>

#include "position.hpp"
#include "test.hpp"

using namespace Clockwork;

int main() {
    Position pos = Position::parse("8/3k4/2R5/2n1r3/8/5K2/8/8 w - - 0 96").value();

    REQUIRE(pos.piece_count(Color::White, PieceType::Pawn) == 0);
    REQUIRE(pos.piece_count(Color::Black, PieceType::Pawn) == 0);
    REQUIRE(pos.piece_count(Color::White, PieceType::Queen) == 0);
    REQUIRE(pos.piece_count(Color::Black, PieceType::Queen) == 0);
    REQUIRE(pos.piece_count(Color::White, PieceType::Rook) == 1);
    REQUIRE(pos.piece_count(Color::Black, PieceType::Rook) == 1);
    REQUIRE(pos.piece_count(Color::White, PieceType::Knight) == 0);
    REQUIRE(pos.piece_count(Color::Black, PieceType::Knight) == 1);
    REQUIRE(pos.piece_count(Color::White, PieceType::King) == 1);
    REQUIRE(pos.piece_count(Color::Black, PieceType::King) == 1);

    REQUIRE(pos.piece_count<PieceType::Pawn>(Color::White) == 0);
    REQUIRE(pos.piece_count<PieceType::Pawn>(Color::Black) == 0);
    REQUIRE(pos.piece_count<PieceType::Queen>(Color::White) == 0);
    REQUIRE(pos.piece_count<PieceType::Queen>(Color::Black) == 0);
    REQUIRE(pos.piece_count<PieceType::Rook>(Color::White) == 1);
    REQUIRE(pos.piece_count<PieceType::Rook>(Color::Black) == 1);
    REQUIRE(pos.piece_count<PieceType::Knight>(Color::White) == 0);
    REQUIRE(pos.piece_count<PieceType::Knight>(Color::Black) == 1);
    REQUIRE(pos.piece_count<PieceType::King>(Color::White) == 1);
    REQUIRE(pos.piece_count<PieceType::King>(Color::Black) == 1);

    REQUIRE(
      (pos.piece_count<PieceType::Pawn, PieceType::Rook, PieceType::Queen>(Color::White) == 1));
    REQUIRE(
      (pos.piece_count<PieceType::Pawn, PieceType::Rook, PieceType::Queen>(Color::Black) == 1));

    return 0;
}

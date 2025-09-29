#include "move.hpp"
#include "movegen.hpp"
#include "position.hpp"
#include "test.hpp"
#include <iostream>
#include <string_view>
#include <tuple>
#include <vector>

using namespace Clockwork;

void game1() {
    std::vector<std::string_view> game1_record{{
      "e4",   "c5",     //
      "Nf3",  "e6",     //
      "d4",   "cxd4",   //
      "Nxd4", "Nc6",    //
      "Nb5",  "d6",     //
      "c4",   "Nf6",    //
      "N1c3", "a6",     //
      "Na3",  "d5",     //
      "cxd5", "exd5",   //
      "exd5", "Nb4",    //
      "Be2",  "Bc5",    //
      "O-O",  "O-O",    //
      "Bf3",  "Bf5",    //
      "Bg5",  "Re8",    //
      "Qd2",  "b5",     //
      "Rad1", "Nd3",    //
      "Nab1", "h6",     //
      "Bh4",  "b4",     //
      "Na4",  "Bd6",    //
      "Bg3",  "Rc8",    //
      "b3",   "g5",     //
      "Bxd6", "Qxd6",   //
      "g3",   "Nd7",    //
      "Bg2",  "Qf6",    //
      "a3",   "a5",     //
      "axb4", "axb4",   //
      "Qa2",  "Bg6",    //
      "d6",   "g4",     //
      "Qd2",  "Kg7",    //
      "f3",   "Qxd6",   //
      "fxg4", "Qd4+",   //
      "Kh1",  "Nf6",    //
      "Rf4",  "Ne4",    //
      "Qxd3", "Nf2+",   //
      "Rxf2", "Bxd3",   //
      "Rfd2", "Qe3",    //
      "Rxd3", "Rc1",    //
      "Nb2",  "Qf2",    //
      "Nd2",  "Rxd1+",  //
      "Nxd1", "Re1+",   //
    }};

    Position pos =
      Position::parse("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1").value();
    for (std::string_view move_str : game1_record) {
        std::cout << move_str << " : ";

        auto move = Move::parseSan(move_str, pos);
        REQUIRE(move);

        std::cout << *move << " : ";

        MoveGen movegen{pos};
        REQUIRE(movegen.is_legal(*move));

        pos = pos.move(*move);

        std::cout << pos << std::endl;
    }

    REQUIRE(pos.to_string() == "8/5pk1/7p/8/1p4P1/1P1R2P1/3N1qBP/3Nr2K w - - 1 41");
}

void cases() {
    std::vector<std::tuple<std::string_view, std::string_view, std::string_view>> cases{{
      {"7r/3r1p1p/6p1/1p6/2B5/5PP1/1Q5P/1K1k4 b - - 0 38", "bxc4",
       "7r/3r1p1p/6p1/8/2p5/5PP1/1Q5P/1K1k4 w - - 0 39"},
      {"2n1r1n1/1p1k1p2/6pp/R2pP3/3P4/8/5PPP/2R3K1 b - - 0 30", "Nge7",
       "2n1r3/1p1knp2/6pp/R2pP3/3P4/8/5PPP/2R3K1 w - - 1 31"},
      {"8/5p2/1kn1r1n1/1p1pP3/6K1/8/4R3/5R2 b - - 9 60", "Ngxe5+",
       "8/5p2/1kn1r3/1p1pn3/6K1/8/4R3/5R2 w - - 0 61"},
      {"r3k2r/pp1bnpbp/1q3np1/3p4/3N1P2/1PP1Q2P/P1B3P1/RNB1K2R b KQkq - 5 15", "Ng8",
       "r3k1nr/pp1bnpbp/1q4p1/3p4/3N1P2/1PP1Q2P/P1B3P1/RNB1K2R w KQkq - 6 16"},
    }};
    for (auto [before, san, after] : cases) {
        std::cout << before << " + " << san << ":";
        auto pos1 = Position::parse(before);
        REQUIRE(pos1);
        auto move = Move::parseSan(san, *pos1);
        REQUIRE(move);
        std::cout << *move << " = ";
        Position pos2 = pos1->move(*move);
        std::cout << pos2 << std::endl;
        REQUIRE(pos2.to_string() == after);
    }
}

int main() {
    game1();
    cases();
    return 0;
}

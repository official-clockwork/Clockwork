#include <iostream>
#include <sstream>
#include <string_view>
#include <tuple>
#include <vector>

#include "perft.hpp"
#include "position.hpp"
#include <cstdlib>
#include <iomanip>

using namespace Clockwork;

int main() {
    std::vector<std::tuple<std::string_view, std::vector<u64>>> cases{{
      {
        "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1",
        {{1, 48, 2039, 97862, 4085603, 193690690}},
      },
      {
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
        {{1, 20, 400, 8902, 197281, 4865609, 119060324}},
      },
      {
        "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1",
        {{1, 14, 191, 2812, 43238, 674624, 11030083}},
      },
      {
        "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1",
        {{1, 6, 264, 9467, 422333, 15833292}},
      },
      {
        "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8",
        {{1, 44, 1486, 62379, 2103487, 89941194}},
      },
      {
        "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10",
        {{1, 46, 2079, 89890, 3894594, 164075551}},
      },
    }};

    for (auto [fen, results] : cases) {
        Position position = *Position::parse(fen);
        std::cout << fen << ":" << std::endl;

        for (usize depth = 0; depth < results.size(); depth++) {
            u64 value = perft(position, depth);

            std::cout << depth << ":" << value << std::endl;

            if (value != results[depth]) {
                split_perft(position, depth);
                std::exit(1);
            }
        }
    }

    return 0;
}

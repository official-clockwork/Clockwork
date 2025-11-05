#include <bit>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string_view>
#include <tuple>
#include <vector>

#include "movegen.hpp"
#include "position.hpp"

using namespace Clockwork;

template<bool print>
u64 is_legal_perft(const Position& position, usize depth) {
    if (depth == 0) {
        return 1;
    }

    u64     result = 0;
    MoveGen movegen{position};

    for (u32 i = 0; i < 0x10000; i++) {
        Move m = std::bit_cast<Move>(static_cast<u16>(i));
        if (movegen.is_legal(m)) {
            Position child_position = position.move(m);
            u64      child          = is_legal_perft<false>(child_position, depth - 1);
            result += child;
            if constexpr (print) {
                std::cout << m << ": " << child << std::endl;
            }
        }
    }

    return result;
}

int main() {
    std::vector<std::tuple<std::string_view, std::vector<u64>>> cases{{
      {
        "8/8/3p4/1Pp4r/1K5k/5p2/4P1P1/1R6 w - c6 0 3",
        {{1, 7}},
      },
      {
        "8/2p5/3p4/KP5r/5p1k/8/4P1P1/1R6 b - - 1 1",
        {{1, 16, 265, 4199, 69665}},
      },
      {
        "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1R1K b kq - 1 1",
        {{1, 46}},
      },
      {
        "8/2p5/3p4/KP5r/1R3pPk/8/4P3/8 b - g3 0 1",
        {{1, 17}},
      },
      {
        "r3k2r/p1ppqpb1/1n2pnp1/3PN3/1p2P3/2N2Q1p/PPPBbPPP/R2K3R w kq - 0 2",
        {{1, 5}},
      },
      {
        "8/2p5/3p4/KP5r/1R3p1k/6P1/4P3/8 b - - 0 1",
        {{1, 4}},
      },
      {
        "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1",
        {{1, 48, 2039, 97862}},  //, 4085603
      },
      {
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
        {{1, 20, 400, 8902, 197281}},  //, 4865609
      },
      {
        "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1",
        {{1, 14, 191, 2812, 43238, 674624}},
      },
      {
        "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1",
        {{1, 6, 264, 9467, 422333}},
      },
      {
        "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8",
        {{1, 44, 1486, 62379}},  //, 2103487
      },
      {
        "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10",
        {{1, 46, 2079, 89890}},  //, 3894594
      },
    }};

    for (auto [fen, results] : cases) {
        Position position = *Position::parse(fen);
        std::cout << fen << ":" << std::endl;

        for (usize depth = 0; depth < results.size(); depth++) {
            u64 value = is_legal_perft<false>(position, depth);

            std::cout << depth << ":" << value << std::endl;

            if (value != results[depth]) {
                is_legal_perft<true>(position, depth);
                std::exit(1);
            }
        }
    }

    return 0;
}

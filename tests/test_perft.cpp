#include <iostream>
#include <sstream>
#include <string_view>
#include <tuple>
#include <vector>

#include <iomanip>

#include "position.hpp"
#include "perft.hpp"

using namespace Clockwork;

auto main() -> int {
    const std::vector<std::tuple<std::string_view, std::vector<u64>>> cases{{
      {"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1",
       {{1, 48, 2039, 97862, 4085603}}},
      {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
       {{1, 20, 400, 8902, 197281, 4865609}}},
    }};

    for (auto [fen, results] : cases)
    {
        const Position position = *Position::parse(fen);
        std::cout << fen << ":" << std::endl;

        for (usize depth = 0; depth < results.size(); depth++)
        {
            const u64 value = perft(position, depth);

            std::cout << depth << ":" << value << std::endl;

            if (value != results[depth])
            {
                splitPerft(position, depth);
                std::exit(1);
            }
        }
    }

    return 0;
}

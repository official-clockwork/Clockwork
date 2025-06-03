#include <iostream>
#include <sstream>
#include <string_view>
#include <tuple>
#include <vector>

#include "position.hpp"

#define REQUIRE(x) \
    if (!(x)) \
        std::exit(1);

using namespace Clockwork;

void roundtripClassical() {
    const std::vector<std::string_view> cases{{
      "7r/3r1p1p/6p1/1p6/2B5/5PP1/1Q5P/1K1k4 b - - 0 38",
      "r3k2r/pp1bnpbp/1q3np1/3p4/3N1P2/1PP1Q2P/P1B3P1/RNB1K2R b KQkq - 5 15",
      "8/5p2/1kn1r1n1/1p1pP3/6K1/8/4R3/5R2 b - - 9 60",
      "r4rk1/1Bp1qppp/2np1n2/1pb1p1B1/4P1b1/P1NP1N2/1PP1QPPP/R4RK1 b - b6 1 11",
    }};

    for (std::string_view fen : cases)
    {
        const Position position = *Position::parse(fen);

        std::ostringstream os;
        os << position;

        std::cout << fen << std::endl;
        std::cout << position << std::endl;

        REQUIRE(fen == os.str());
    }
}

auto main() -> int {
    roundtripClassical();
    return 0;
}

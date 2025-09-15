#include <iostream>
#include <sstream>
#include <string_view>
#include <tuple>
#include <vector>

#include "position.hpp"
#include "test.hpp"

using namespace Clockwork;

void roundtrip_classical_fens() {
    std::vector<std::string_view> cases{{
      "7r/3r1p1p/6p1/1p6/2B5/5PP1/1Q5P/1K1k4 b - - 0 38",
      "r3k2r/pp1bnpbp/1q3np1/3p4/3N1P2/1PP1Q2P/P1B3P1/RNB1K2R b KQkq - 5 15",
      "8/5p2/1kn1r1n1/1p1pP3/6K1/8/4R3/5R2 b - - 9 60",
      "r4rk1/1Bp1qppp/2np1n2/1pb1p1B1/4P1b1/P1NP1N2/1PP1QPPP/R4RK1 b - b6 1 11",
    }};

    g_frc = false;

    for (std::string_view fen : cases) {
        Position position = *Position::parse(fen);

        std::ostringstream os;
        os << position;

        std::cout << fen << std::endl;
        std::cout << position << std::endl;

        REQUIRE(fen == os.str());
    }
}

void roundtrip_dfrc_fens() {
    std::vector<std::string_view> cases{{
      "2r1kr2/8/8/8/8/8/8/1R2K1R1 w GBfc - 0 1",
      "rkr5/8/8/8/8/8/8/5RKR w HFca - 0 1",
      "2r3kr/8/8/8/8/8/8/2KRR3 w h - 3 2",
      "5rkr/8/8/8/8/8/8/RKR5 w CAhf - 0 1",
      "3rkr2/8/8/8/8/8/8/R3K2R w HAfd - 0 1",
      "4k3/8/8/8/8/8/8/4KR2 w F - 0 1",
      "4kr2/8/8/8/8/8/8/4K3 w f - 0 1",
      "4k3/8/8/8/8/8/8/2R1K3 w C - 0 1",
      "2r1k3/8/8/8/8/8/8/4K3 w c - 0 1",
    }};

    g_frc = true;

    for (std::string_view fen : cases) {
        Position position = *Position::parse(fen);

        std::ostringstream os;
        os << position;

        std::cout << fen << std::endl;
        std::cout << position << std::endl;

        REQUIRE(fen == os.str());
    }
}

void create_superpiece_mask() {
    Position pos = Position::parse("7r/3r1p1p/6p1/1p6/2B5/5PP1/1Q5P/1K1k4 b - - 0 38").value();
    Square   sq  = Square::parse("d5").value();

    CreateSuperpieceMaskInfo cmi;
    cmi.diag       = 0x01;
    cmi.orth       = 0x02;
    cmi.orth_near  = 0x04;
    cmi.knight     = 0x08;
    cmi.wpawn_near = 0x10;
    cmi.bpawn_near = 0x20;

    Wordboard wb = pos.create_attack_table_superpiece_mask(sq, cmi);

    std::array<u16, 64> expected_result{{
      0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00,  //
      0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00,  //
      0x00, 0x00, 0x08, 0x02, 0x08, 0x01, 0x00, 0x00,  //
      0x00, 0x08, 0x10, 0x04, 0x10, 0x08, 0x00, 0x00,  //
      0x00, 0x02, 0x04, 0x00, 0x04, 0x02, 0x02, 0x02,  //
      0x00, 0x08, 0x20, 0x04, 0x20, 0x08, 0x00, 0x00,  //
      0x00, 0x01, 0x08, 0x02, 0x08, 0x01, 0x00, 0x00,  //
      0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //
    }};

    REQUIRE(std::bit_cast<Wordboard>(expected_result) == wb);
}

int main() {
    roundtrip_classical_fens();
    roundtrip_dfrc_fens();
    create_superpiece_mask();
    return 0;
}

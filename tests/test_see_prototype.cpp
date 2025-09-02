#include "board.hpp"
#include "geometry.hpp"
#include "position.hpp"
#include "square.hpp"
#include "test.hpp"
#include "util/types.hpp"
#include <array>
#include <bit>
#include <iomanip>
#include <iostream>

using namespace Clockwork;

template<typename T>
void print8(T x) {
    std::cout << std::hex;
    auto y = std::bit_cast<std::array<u8, sizeof(T)>>(x);
    for (size_t i = 0; i < sizeof(T); i++) {
        if (i != 0 && (i % 8) == 0) {
            std::cout << "| ";
        }
        std::cout << std::setw(2) << std::setfill('0') << (int)y[i] << ' ';
    }
    std::cout << std::endl;
}

int main() {
    Position pos   = Position::parse("5rk1/5pp1/2r4p/5b2/2R5/6Q1/R1P1qPP1/5NK1 b - - 0 1").value();
    Move     move  = Move::parse("f5c2", pos).value();
    Square   focus = move.to();

    // Extract all possible attackers to our position

    auto [ray_coords, ray_valid] = geometry::superpiece_rays(focus);
    v512 ray_places              = v512::permute8(ray_coords, pos.board().to_vec());
    v512 ray_attackers           = ray_places & geometry::attackers_from_rays(ray_places);
    v512 ptypes                  = ray_attackers & ray_valid & v512::broadcast8(Place::PTYPE_MASK);

    print8(ray_coords);
    print8(ray_valid);
    print8(ray_places);
    print8(ray_attackers);
    print8(ptypes);

    u64 color     = v512::test8(ray_places, v512::broadcast8(Place::COLOR_MASK));
    u64 occupied  = v512::test8(ray_places, ray_valid);
    u64 attackers = v512::test8(ray_attackers, ray_valid);

    std::cout << std::setw(16) << std::setfill('0') << occupied << std::endl;
    std::cout << std::setw(16) << std::setfill('0') << attackers << std::endl;

    std::array<u64, 2> color_bits{{
      ~color,
      color,
    }};

    std::cout << std::setw(16) << std::setfill('0') << color_bits[0] << std::endl;
    std::cout << std::setw(16) << std::setfill('0') << color_bits[1] << std::endl;

    // Extract bitrays for each piece type
    alignas(64) std::array<u64, 8> ptype_bits{
      0,  // None
      v512::eq8(ptypes, v512::broadcast8(static_cast<u8>(PieceType::Pawn) << Place::PTYPE_SHIFT)),
      0x0101010101010101,  // Knight
      v512::eq8(ptypes, v512::broadcast8(static_cast<u8>(PieceType::Bishop) << Place::PTYPE_SHIFT)),
      v512::eq8(ptypes, v512::broadcast8(static_cast<u8>(PieceType::Rook) << Place::PTYPE_SHIFT)),
      v512::eq8(ptypes, v512::broadcast8(static_cast<u8>(PieceType::Queen) << Place::PTYPE_SHIFT)),
      v512::eq8(ptypes, v512::broadcast8(static_cast<u8>(PieceType::King) << Place::PTYPE_SHIFT)),
      0,  // Invalid
    };
    v512 ptype_vec{ptype_bits};

    std::cout << std::setw(16) << std::setfill('0') << ptype_bits[0] << std::endl;
    std::cout << std::setw(16) << std::setfill('0') << ptype_bits[1] << std::endl;
    std::cout << std::setw(16) << std::setfill('0') << ptype_bits[2] << std::endl;
    std::cout << std::setw(16) << std::setfill('0') << ptype_bits[3] << std::endl;
    std::cout << std::setw(16) << std::setfill('0') << ptype_bits[4] << std::endl;
    std::cout << std::setw(16) << std::setfill('0') << ptype_bits[5] << std::endl;
    std::cout << std::setw(16) << std::setfill('0') << ptype_bits[6] << std::endl;
    std::cout << std::setw(16) << std::setfill('0') << ptype_bits[7] << std::endl;

    Color stm = pos.active_color();

    while (true) {
        u64 current = geometry::closest(occupied) & color_bits[static_cast<usize>(stm)];
        if ((current & attackers) == 0) {
            break;
        }

        print8(ptype_vec & v512::broadcast64(current));
        std::cout << std::setw(2) << std::setfill('0')
                  << (ptype_vec & v512::broadcast64(current)).nonzero64() << std::endl;
        i32 next = std::countr_zero((ptype_vec & v512::broadcast64(current)).nonzero64());
        u64 br   = ptype_bits[static_cast<size_t>(next)] & current;

        std::cout << "current:        " << std::setw(16) << std::setfill('0') << current
                  << std::endl;
        std::cout << "occupied:       " << std::setw(16) << std::setfill('0') << occupied
                  << std::endl;
        std::cout << "next:           " << next << std::endl;
        std::cout << "br:             " << std::setw(16) << std::setfill('0') << br << std::endl;
        std::cout << "lowest_bit(br): " << std::setw(16) << std::setfill('0') << lowest_bit(br)
                  << std::endl;

        occupied ^= lowest_bit(br);

        PieceType ptype = static_cast<PieceType>(next);

        std::cout << color_char(stm) << ": " << piece_char(ptype) << std::endl;

        stm = ~stm;
    }

    return 0;
}

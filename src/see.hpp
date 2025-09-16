#pragma once

#include "common.hpp"
#include "geometry.hpp"
#include "move.hpp"
#include "position.hpp"
#include "util/types.hpp"
#include "util/vec.hpp"
#include <array>
#include <bit>
#include <cassert>
#include <tuple>

namespace Clockwork::SEE {

inline Value value(PieceType ptype) {
    constexpr std::array<Value, 7> TABLE{{0, 100, 300, 300, 500, 900, 10000}};
    return TABLE[static_cast<usize>(ptype)];
}

inline Value gain(const Position& pos, Move move) {
    if (move.is_castle()) {
        return 0;
    }
    if (move.is_en_passant()) {
        return value(PieceType::Pawn);
    }

    Value score = value(pos.board()[move.to()].ptype());
    if (move.is_promotion()) {
        score += value(*move.promo()) - value(PieceType::Pawn);
    }
    return score;
}

inline bool see(const Position& pos, Move move, Value threshold) {
    Square sq  = move.to();
    Color  stm = pos.active_color();

    Value score = gain(pos, move) - threshold;
    if (score < 0) {
        return false;
    }

    PieceType next = move.promo().value_or(pos.board()[move.from()].ptype());
    score -= value(next);
    stm = invert(stm);

    if (score >= 0) {
        return true;
    }

    // Extract all possible attackers to our position
    auto [ray_coords, ray_valid] = geometry::superpiece_rays(sq);
    v512 ray_places              = v512::permute8(ray_coords, pos.board().to_vec());
    v512 ray_attackers           = ray_places & geometry::attackers_from_rays(ray_places);
    v512 ptypes                  = ray_attackers & ray_valid & v512::broadcast8(Place::PTYPE_MASK);

    // Bitrays (not bitboards)
    u64 color     = v512::test8(ray_places, v512::broadcast8(Place::COLOR_MASK));
    u64 occupied  = v512::test8(ray_places, ray_valid);
    u64 attackers = v512::test8(ray_attackers, ray_valid);

    // Remove already moved piece
    occupied ^= v512::eq8(ray_coords, v512::broadcast8(move.from().raw));
    if (move.is_en_passant()) {
        occupied &= pos.active_color() == Color::Black ? 0xFFFFFFFFFFFFFFFD : 0xFFFFFFFDFFFFFFFF;
    }

    // Extract bitrays for each piece type
    std::array<u64, 8> ptype_bits{
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

    auto current_attackers = [&]() {
        return geometry::closest(occupied) & (stm == Color::Black ? color : ~color) & attackers;
    };

    while (u64 current = current_attackers()) {
        i32  next  = std::countr_zero((ptype_vec & v512::broadcast64(current)).nonzero64());
        auto ptype = static_cast<PieceType>(next);
        u64  br    = ptype_bits[static_cast<size_t>(next)] & current;
        occupied ^= lowest_bit(br);

        score = -score - 1 - value(ptype);
        stm   = invert(stm);

        if (score >= 0) {
            if (ptype == PieceType::King && current_attackers() != 0) {
                // We'd be in check if we actually did that.
                stm = invert(stm);
            }
            break;
        }
    }

    return stm != pos.active_color();
}

}

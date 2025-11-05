#pragma once

#include "common.hpp"
#include "geometry.hpp"
#include "move.hpp"
#include "position.hpp"
#include "util/types.hpp"
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
    u8x64 ray_places             = ray_coords.swizzle(pos.board().to_vector());
    u8x64 ray_attackers          = geometry::attackers_from_rays(ray_places).mask(ray_places);
    u8x64 ptypes                 = ray_valid.mask(ray_attackers & u8x64::splat(Place::PTYPE_MASK));

    // Bitrays (not bitboards)
    u64 color     = ray_places.test(u8x64::splat(Place::COLOR_MASK)).to_bits();
    u64 occupied  = (ray_places.nonzeros() & ray_valid).to_bits();
    u64 attackers = (ray_attackers.nonzeros() & ray_valid).to_bits();

    // Remove already moved piece
    occupied ^= ray_coords.eq(u8x64::splat(move.from().raw)).to_bits();
    if (move.is_en_passant()) {
        occupied &= pos.active_color() == Color::Black ? 0xFFFFFFFFFFFFFFFD : 0xFFFFFFFDFFFFFFFF;
    }

    // Extract bitrays for each piece type
    std::array<u64, 8> ptype_bits{
      0,  // None
      ptypes.eq(u8x64::splat(static_cast<u8>(PieceType::Pawn) << Place::PTYPE_SHIFT)).to_bits(),
      0x0101010101010101,  // Knight
      ptypes.eq(u8x64::splat(static_cast<u8>(PieceType::Bishop) << Place::PTYPE_SHIFT)).to_bits(),
      ptypes.eq(u8x64::splat(static_cast<u8>(PieceType::Rook) << Place::PTYPE_SHIFT)).to_bits(),
      ptypes.eq(u8x64::splat(static_cast<u8>(PieceType::Queen) << Place::PTYPE_SHIFT)).to_bits(),
      ptypes.eq(u8x64::splat(static_cast<u8>(PieceType::King) << Place::PTYPE_SHIFT)).to_bits(),
      0,  // Invalid
    };
    u64x8 ptype_vec{ptype_bits};

    auto current_attackers = [&]() {
        return geometry::closest(occupied) & (stm == Color::Black ? color : ~color) & attackers;
    };

    while (u64 current = current_attackers()) {
        i32  next  = std::countr_zero((ptype_vec & u64x8::splat(current)).nonzeros().to_bits());
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

}  // namespace Clockwork::SEE

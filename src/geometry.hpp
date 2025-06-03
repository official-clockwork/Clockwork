#pragma once

#include <utility>
#include <tuple>

#include "square.hpp"
#include "util/types.hpp"
#include "util/vec.hpp"

namespace Clockwork::geometry {

namespace internal {
// 00rrrfff → 0rrr0fff
forceinline constexpr auto expand_sq(Square sq) -> u8 {
    return sq.raw + (sq.raw & 0b111000);
}

// 0rrr0fff → 00rrrfff
template<typename V>
forceinline auto compress_coords(V list) -> std::tuple<V, typename V::Mask8> {
    typename V::Mask8 valid = V::testn8(list, V::broadcast8(0x88));
    V compressed = (list & V::broadcast8(0x07)) | V::shr16(list & V::broadcast8(0x70), 1);
    return {compressed, valid};
}
}  // namespace internal


inline std::tuple<v512, u64> superpiece_rays(Square sq) {
    static const v512 OFFSETS = v512{std::array<u8, 64>{
      0x1F, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70,  // N
      0x21, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,  // NE
      0x12, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,  // E
      0xF2, 0xF1, 0xE2, 0xD3, 0xC4, 0xB5, 0xA6, 0x97,  // SE
      0xE1, 0xF0, 0xE0, 0xD0, 0xC0, 0xB0, 0xA0, 0x90,  // S
      0xDF, 0xEF, 0xDE, 0xCD, 0xBC, 0xAB, 0x9A, 0x89,  // SW
      0xEE, 0xFF, 0xFE, 0xFD, 0xFC, 0xFB, 0xFA, 0xF9,  // W
      0x0E, 0x0F, 0x1E, 0x2D, 0x3C, 0x4B, 0x5A, 0x69,  // NW
    }};

    v512 sq_vec       = v512::broadcast8(internal::expand_sq(sq));
    v512 uncompressed = v512::add8(sq_vec, OFFSETS);
    return internal::compress_coords(uncompressed);
}

inline u64 superpiece_attacks(u64 occupied, u64 ray_valid) {
    u64 o = occupied | 0x8181818181818181;
    u64 x = o ^ (o - 0x0303030303030303);
    return x & ray_valid;
}

inline u64 attackers_from_rays(v512 ray_places) {
    constexpr u8 K  = 1 << 0;
    constexpr u8 WP = 1 << 1;
    constexpr u8 BP = 1 << 2;
    constexpr u8 N  = 1 << 3;
    constexpr u8 B  = 1 << 4;
    constexpr u8 R  = 1 << 5;
    constexpr u8 Q  = 1 << 6;

    constexpr u8 DIAG       = B | Q;
    constexpr u8 ORTH       = R | Q;
    constexpr u8 ORTH_NEAR  = R | Q | K;
    constexpr u8 HORSE      = N;
    constexpr u8 WPAWN_NEAR = B | Q | K | WP;
    constexpr u8 BPAWN_NEAR = B | Q | K | BP;

    static const v128 PTYPE_TO_BITS{
      std::array<u8, 16>{{0, WP, N, B, R, Q, K, 0, 0, BP, N, B, R, Q, K, 0}}};

    static const v512 ATTACKER_MASK = v512{std::array<u8, 64>{
      HORSE, ORTH_NEAR,  ORTH, ORTH, ORTH, ORTH, ORTH, ORTH,  // N
      HORSE, BPAWN_NEAR, DIAG, DIAG, DIAG, DIAG, DIAG, DIAG,  // NE
      HORSE, ORTH_NEAR,  ORTH, ORTH, ORTH, ORTH, ORTH, ORTH,  // E
      HORSE, WPAWN_NEAR, DIAG, DIAG, DIAG, DIAG, DIAG, DIAG,  // SE
      HORSE, ORTH_NEAR,  ORTH, ORTH, ORTH, ORTH, ORTH, ORTH,  // S
      HORSE, WPAWN_NEAR, DIAG, DIAG, DIAG, DIAG, DIAG, DIAG,  // SW
      HORSE, ORTH_NEAR,  ORTH, ORTH, ORTH, ORTH, ORTH, ORTH,  // W
      HORSE, BPAWN_NEAR, DIAG, DIAG, DIAG, DIAG, DIAG, DIAG,  // NW
    }};

    v512 bit_rays = v512::permute8(v512::shr16(ray_places, 4) & v512::broadcast8(0x0F),
                                   v512::from128(PTYPE_TO_BITS));
    return (bit_rays & ATTACKER_MASK).nonzero8();
}

}  // namespace rose::geometry

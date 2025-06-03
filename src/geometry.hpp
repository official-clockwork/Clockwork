#pragma once

#include <utility>
#include <tuple>

#include "square.hpp"
#include "util/types.hpp"
#include "util/vec.hpp"

namespace Clockwork::geometry {

namespace internal {
// 00rrrfff → 0rrr0fff
forceinline constexpr auto expandSq(Square sq) -> u8 { return sq.raw + (sq.raw & 0b111000); }

// 0rrr0fff → 00rrrfff
template<typename V>
forceinline auto compressCoords(V list) -> std::tuple<V, typename V::Mask8> {
    const typename V::Mask8 valid = V::testn8(list, V::broadcast8(0x88));
    const V compressed = (list & V::broadcast8(0x07)) | V::shr16(list & V::broadcast8(0x70), 1);
    return {compressed, valid};
}
}  // namespace internal


inline std::tuple<v512, u64> superpieceRays(Square sq) {
    static const v512 offsets = v512{std::array<u8, 64>{
      0x1F, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70,  // N
      0x21, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,  // NE
      0x12, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,  // E
      0xF2, 0xF1, 0xE2, 0xD3, 0xC4, 0xB5, 0xA6, 0x97,  // SE
      0xE1, 0xF0, 0xE0, 0xD0, 0xC0, 0xB0, 0xA0, 0x90,  // S
      0xDF, 0xEF, 0xDE, 0xCD, 0xBC, 0xAB, 0x9A, 0x89,  // SW
      0xEE, 0xFF, 0xFE, 0xFD, 0xFC, 0xFB, 0xFA, 0xF9,  // W
      0x0E, 0x0F, 0x1E, 0x2D, 0x3C, 0x4B, 0x5A, 0x69,  // NW
    }};

    const v512 sq_vec       = v512::broadcast8(internal::expandSq(sq));
    const v512 uncompressed = v512::add8(sq_vec, offsets);
    return internal::compressCoords(uncompressed);
}

inline u64 superpieceAttacks(u64 occupied, u64 ray_valid) {
    const u64 o = occupied | 0x8181818181818181;
    const u64 x = o ^ (o - 0x0303030303030303);
    return x & ray_valid;
}

inline u64 attackersFromRays(v512 ray_places) {
    constexpr u8 k  = 1 << 0;
    constexpr u8 wp = 1 << 1;
    constexpr u8 bp = 1 << 2;
    constexpr u8 n  = 1 << 3;
    constexpr u8 b  = 1 << 4;
    constexpr u8 r  = 1 << 5;
    constexpr u8 q  = 1 << 6;

    constexpr u8 diag       = b | q;
    constexpr u8 orth       = r | q;
    constexpr u8 orth_near  = r | q | k;
    constexpr u8 horse      = n;
    constexpr u8 wpawn_near = b | q | k | wp;
    constexpr u8 bpawn_near = b | q | k | bp;

    static const v128 ptype_to_bits{
      std::array<u8, 16>{{0, wp, n, b, r, q, k, 0, 0, bp, n, b, r, q, k, 0}}};

    static const v512 attacker_mask = v512{std::array<u8, 64>{
      horse, orth_near,  orth, orth, orth, orth, orth, orth,  // N
      horse, bpawn_near, diag, diag, diag, diag, diag, diag,  // NE
      horse, orth_near,  orth, orth, orth, orth, orth, orth,  // E
      horse, wpawn_near, diag, diag, diag, diag, diag, diag,  // SE
      horse, orth_near,  orth, orth, orth, orth, orth, orth,  // S
      horse, wpawn_near, diag, diag, diag, diag, diag, diag,  // SW
      horse, orth_near,  orth, orth, orth, orth, orth, orth,  // W
      horse, bpawn_near, diag, diag, diag, diag, diag, diag,  // NW
    }};

    const v512 bit_rays = v512::permute8(v512::shr16(ray_places, 4) & v512::broadcast8(0x0F),
                                         v512::from128(ptype_to_bits));
    return (bit_rays & attacker_mask).nonzero8();
}

}  // namespace rose::geometry

#pragma once

#include "common.hpp"
#include "square.hpp"
#include "util/types.hpp"
#include "util/vec.hpp"
#include <array>
#include <cassert>
#include <tuple>
#include <utility>

namespace Clockwork::geometry {

namespace internal {
// 00rrrfff → 0rrr0fff
forceinline constexpr u8 expand_sq(Square sq) {
    return sq.raw + (sq.raw & 0b111000);
}

// 0rrr0fff → 00rrrfff
template<typename V>
forceinline std::tuple<V, V> compress_coords(V list) {
    V valid      = V::eq8_vm(list & V::broadcast8(0x88), V::zero());
    V compressed = (list & V::broadcast8(0x07)) | (V::shr16(list, 1) & V::broadcast8(0x38));
    return {compressed, valid};
}
}  // namespace internal


inline std::tuple<v512, v512> superpiece_rays(Square sq) {
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

inline v512 superpiece_attacks(v512 ray_places, v512 ray_valid) {
    return v512::andnot(v512::eq8_vm(ray_places, v512::sub64(ray_places, v512::broadcast64(0x101))),
                        ray_valid);
}

inline u64 closest(u64 occupied) {
    u64 o = occupied | 0x8181818181818181;
    u64 x = o ^ (o - 0x0303030303030303);
    return x & occupied;
}

inline v512 attackers_from_rays(v512 ray_places) {
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
      std::array<u8, 16>{{0, 0, WP, BP, N, N, B, B, R, R, Q, Q, K, K, 0, 0}}};

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

    v512 bit_rays =
      v512::permute8(v512::shr16(ray_places, 4) & v512::broadcast8(0x0F), PTYPE_TO_BITS);
    return v512::gts8_vm(bit_rays & ATTACKER_MASK, v512::zero());
}

inline v512 slider_mask(v512 ray_places) {
    constexpr u8 R    = static_cast<u8>(PieceType::Rook) << 5;
    constexpr u8 B    = static_cast<u8>(PieceType::Bishop) << 5;
    constexpr u8 Q    = static_cast<u8>(PieceType::Queen) << 5;
    constexpr u8 NONE = 1;

    static const v512 ROOK_BISHOP_MASK = v512{std::array<u8, 64>{
      NONE, R, R, R, R, R, R, R,  // N
      NONE, B, B, B, B, B, B, B,  // NE
      NONE, R, R, R, R, R, R, R,  // E
      NONE, B, B, B, B, B, B, B,  // SE
      NONE, R, R, R, R, R, R, R,  // S
      NONE, B, B, B, B, B, B, B,  // SW
      NONE, R, R, R, R, R, R, R,  // W
      NONE, B, B, B, B, B, B, B,  // NW
    }};
    static const v512 QUEEN_MASK       = v512{std::array<u8, 64>{
      NONE, Q, Q, Q, Q, Q, Q, Q,  // N
      NONE, Q, Q, Q, Q, Q, Q, Q,  // NE
      NONE, Q, Q, Q, Q, Q, Q, Q,  // E
      NONE, Q, Q, Q, Q, Q, Q, Q,  // SE
      NONE, Q, Q, Q, Q, Q, Q, Q,  // S
      NONE, Q, Q, Q, Q, Q, Q, Q,  // SW
      NONE, Q, Q, Q, Q, Q, Q, Q,  // W
      NONE, Q, Q, Q, Q, Q, Q, Q,  // NW
    }};

    ray_places &= v512::broadcast8(0xE0);
    return v512::eq8_vm(ray_places, ROOK_BISHOP_MASK) | v512::eq8_vm(ray_places, QUEEN_MASK);
}

extern const std::array<v512, 64> SUPERPIECE_INVERSE_RAYS_AVX2_TABLE;

inline v512 superpiece_inverse_rays_avx2(Square sq) {
    return SUPERPIECE_INVERSE_RAYS_AVX2_TABLE[sq.raw];
}

extern const std::array<v512, 64> PIECE_MOVES_AVX2_TABLE;

inline v512 piece_moves_avx2(bool color, PieceType ptype, Square sq) {
    assert(ptype != PieceType::None);
    i32  index = ptype == PieceType::Pawn ? color : static_cast<i32>(ptype);
    v512 bit   = v512::broadcast8(static_cast<u8>(1 << index));
    v512 table = PIECE_MOVES_AVX2_TABLE[sq.raw];
    return v512::eq8_vm(table & bit, bit);
}

}

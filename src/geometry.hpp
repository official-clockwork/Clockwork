#pragma once

#include "common.hpp"
#include "square.hpp"
#include "util/types.hpp"
#include <array>
#include <bit>
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
forceinline std::tuple<u8x64, m8x64> compress_coords(u8x64 list) {
    m8x64 valid      = (list & u8x64::splat(0x88)).zeros();
    u8x64 compressed = (list & u8x64::splat(0x07)) | (list & u8x64::splat(0x70)).shr<1>();
    return {compressed, valid};
}

}  // namespace internal

forceinline std::tuple<u8x64, m8x64> superpiece_rays(Square sq) {
    static const u8x64 OFFSETS{{
      0x1F, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70,  // N
      0x21, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,  // NE
      0x12, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,  // E
      0xF2, 0xF1, 0xE2, 0xD3, 0xC4, 0xB5, 0xA6, 0x97,  // SE
      0xE1, 0xF0, 0xE0, 0xD0, 0xC0, 0xB0, 0xA0, 0x90,  // S
      0xDF, 0xEF, 0xDE, 0xCD, 0xBC, 0xAB, 0x9A, 0x89,  // SW
      0xEE, 0xFF, 0xFE, 0xFD, 0xFC, 0xFB, 0xFA, 0xF9,  // W
      0x0E, 0x0F, 0x1E, 0x2D, 0x3C, 0x4B, 0x5A, 0x69,  // NW
    }};

    u8x64 sq_vec       = u8x64::splat(internal::expand_sq(sq));
    u8x64 uncompressed = sq_vec + OFFSETS;
    return internal::compress_coords(uncompressed);
}

forceinline m8x64 superpiece_attacks(u8x64 ray_places, m8x64 ray_valid) {
    return ray_valid.andnot(
      ray_places.eq(std::bit_cast<u8x64>(std::bit_cast<u64x8>(ray_places) - u64x8::splat(0x101))));
}

forceinline u64 closest(u64 occupied) {
    u64 o = occupied | 0x8181818181818181;
    u64 x = o ^ (o - 0x0303030303030303);
    return x & occupied;
}

forceinline m8x64 attackers_from_rays(u8x64 ray_places) {
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

    static const u8x16 PTYPE_TO_BITS{{0, 0, WP, BP, N, N, B, B, R, R, Q, Q, K, K, 0, 0}};

    static const u8x64 ATTACKER_MASK{{
      HORSE, ORTH_NEAR,  ORTH, ORTH, ORTH, ORTH, ORTH, ORTH,  // N
      HORSE, BPAWN_NEAR, DIAG, DIAG, DIAG, DIAG, DIAG, DIAG,  // NE
      HORSE, ORTH_NEAR,  ORTH, ORTH, ORTH, ORTH, ORTH, ORTH,  // E
      HORSE, WPAWN_NEAR, DIAG, DIAG, DIAG, DIAG, DIAG, DIAG,  // SE
      HORSE, ORTH_NEAR,  ORTH, ORTH, ORTH, ORTH, ORTH, ORTH,  // S
      HORSE, WPAWN_NEAR, DIAG, DIAG, DIAG, DIAG, DIAG, DIAG,  // SW
      HORSE, ORTH_NEAR,  ORTH, ORTH, ORTH, ORTH, ORTH, ORTH,  // W
      HORSE, BPAWN_NEAR, DIAG, DIAG, DIAG, DIAG, DIAG, DIAG,  // NW
    }};

    u8x64 bit_rays = ray_places.shr<4>().swizzle(PTYPE_TO_BITS);
    return bit_rays.test(ATTACKER_MASK);
}

forceinline m8x64 slider_mask(u8x64 ray_places) {
    constexpr u8 R    = static_cast<u8>(PieceType::Rook) << 5;
    constexpr u8 B    = static_cast<u8>(PieceType::Bishop) << 5;
    constexpr u8 Q    = static_cast<u8>(PieceType::Queen) << 5;
    constexpr u8 NONE = 1;

    static const u8x64 ROOK_BISHOP_MASK{{
      NONE, R, R, R, R, R, R, R,  // N
      NONE, B, B, B, B, B, B, B,  // NE
      NONE, R, R, R, R, R, R, R,  // E
      NONE, B, B, B, B, B, B, B,  // SE
      NONE, R, R, R, R, R, R, R,  // S
      NONE, B, B, B, B, B, B, B,  // SW
      NONE, R, R, R, R, R, R, R,  // W
      NONE, B, B, B, B, B, B, B,  // NW
    }};
    static const u8x64 QUEEN_MASK{{
      NONE, Q, Q, Q, Q, Q, Q, Q,  // N
      NONE, Q, Q, Q, Q, Q, Q, Q,  // NE
      NONE, Q, Q, Q, Q, Q, Q, Q,  // E
      NONE, Q, Q, Q, Q, Q, Q, Q,  // SE
      NONE, Q, Q, Q, Q, Q, Q, Q,  // S
      NONE, Q, Q, Q, Q, Q, Q, Q,  // SW
      NONE, Q, Q, Q, Q, Q, Q, Q,  // W
      NONE, Q, Q, Q, Q, Q, Q, Q,  // NW
    }};

    ray_places &= u8x64::splat(0xE0);
    return ray_places.eq(ROOK_BISHOP_MASK) | ray_places.eq(QUEEN_MASK);
}

extern const std::array<u8x64, 64> SUPERPIECE_INVERSE_RAYS_AVX2_TABLE;

forceinline u8x64 superpiece_inverse_rays_avx2(Square sq) {
    return SUPERPIECE_INVERSE_RAYS_AVX2_TABLE[sq.raw];
}

extern const std::array<u8x64, 64> PIECE_MOVES_AVX2_TABLE;

forceinline m8x64 piece_moves_avx2(bool color, PieceType ptype, Square sq) {
    assert(ptype != PieceType::None);
    i32   index = ptype == PieceType::Pawn ? color : static_cast<i32>(ptype);
    u8x64 bit   = u8x64::splat(static_cast<u8>(1 << index));
    u8x64 table = PIECE_MOVES_AVX2_TABLE[sq.raw];
    return table.test(bit);
}

forceinline u8x64 slider_broadcast(u8x64 x) {
#if LPS_AVX512
    constexpr u8 NONE = 0xFF;
    u8x64        EXPAND_IDX{{
      NONE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //
      NONE, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,  //
      NONE, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,  //
      NONE, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,  //
      NONE, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,  //
      NONE, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28,  //
      NONE, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,  //
      NONE, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38,  //
    }};
    return EXPAND_IDX.swizzle(u8x64{_mm512_sad_epu8(x.raw, _mm512_setzero_si512())});
#elif LPS_AVX2
    constexpr u8 NONE = 0xFF;
    u8x32        EXPAND_IDX{{
      NONE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //
      NONE, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,  //
      NONE, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,  //
      NONE, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,  //
    }};
    u8x64        y;
    y.raw[0].raw = _mm256_sad_epu8(x.raw[0].raw, _mm256_setzero_si256());
    y.raw[1].raw = _mm256_sad_epu8(x.raw[1].raw, _mm256_setzero_si256());
    y.raw[0]     = EXPAND_IDX.swizzle(y.raw[0]);
    y.raw[1]     = EXPAND_IDX.swizzle(y.raw[1]);
    return y;
#else
    u64x8 y = std::bit_cast<u64x8>(x);
    y *= u64x8::splat(0x0101010101010101);
    y = y.shr<56>();
    y *= u64x8::splat(0x0101010101010100);
    return std::bit_cast<u8x64>(y);
#endif
}

forceinline u8x64 lane_broadcast(u8x64 x) {
#if LPS_AVX2
    u8x64 y;
    y.raw[0].raw = _mm256_sad_epu8(x.raw[0].raw, _mm256_setzero_si256());
    y.raw[1].raw = _mm256_sad_epu8(x.raw[1].raw, _mm256_setzero_si256());
    u8x32 EXPAND_IDX{{
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //
      0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,  //
      0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,  //
      0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,  //
    }};
    y.raw[0] = EXPAND_IDX.swizzle(y.raw[0]);
    y.raw[1] = EXPAND_IDX.swizzle(y.raw[1]);
    return y;
#else
    u64x8 y = std::bit_cast<u64x8>(x);
    y *= u64x8::splat(0x0101010101010101);
    y = y.shr<56>();
    y *= u64x8::splat(0x0101010101010101);
    return std::bit_cast<u8x64>(y);
#endif
}

forceinline u8x64 flip_rays(u8x64 x) {
    auto y = std::bit_cast<std::array<u8x32, 2>>(x);
    return std::bit_cast<u8x64>(std::array<u8x32, 2>{y[1], y[0]});
}

forceinline u16 find_set(u8x16 needle, usize needle_count, u8x16 haystack) {
    u16 result = 0;
    for (usize i = 0; i < needle_count; ++i) {
        result |= haystack.eq(u8x16::splat(needle.read(i))).to_bits();
    }
    return result;
}

}  // namespace Clockwork::geometry

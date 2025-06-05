#pragma once

#include <array>
#include <bit>
#include <iomanip>
#include <iostream>
#include <x86intrin.h>

#include "util/bit.hpp"

namespace Clockwork {

forceinline u32 concat32(u16 a, u16 b) {
    return static_cast<u32>(a) | (static_cast<u32>(b) << 16);
}
forceinline u64 concat64(u32 a, u32 b) {
    return static_cast<u64>(a) | (static_cast<u64>(b) << 32);
}

struct v128 {
    __m128i raw{};

    using Mask8  = u16;
    using Mask16 = u8;
    using Mask32 = u8;
    using Mask64 = u8;

    forceinline constexpr v128() = default;
    forceinline constexpr v128(__m128i raw) :
        raw(raw){};
    forceinline explicit v128(std::array<u8, 16> src) :
        raw(std::bit_cast<__m128i>(src)) {
    }
    forceinline explicit v128(std::array<u16, 8> src) :
        raw(std::bit_cast<__m128i>(src)) {
    }

    static forceinline v128 load(const void* src) {
        return {_mm_loadu_si128(reinterpret_cast<const __m128i*>(src))};
    }

    static forceinline v128 zero() {
        return {_mm_setzero_si128()};
    }

    static forceinline v128 broadcast8(u8 x) {
        return {_mm_set1_epi8(static_cast<i8>(x))};
    }

    static forceinline v128 blend8(v128 mask, v128 a, v128 b) {
        return {_mm_blendv_epi8(a.raw, b.raw, mask.raw)};
    }

    static forceinline v128 permute8(v128 index, v128 a) {
        return {_mm_shuffle_epi8(a.raw, index.raw)};
    }

    static forceinline v128 permute8(v128 index, v128 a, v128 b, v128 c, v128 d) {
        v128 w = permute8(index, a);
        v128 x = permute8(index, b);
        v128 y = permute8(index, c);
        v128 z = permute8(index, d);

        v128 mask0 = shl16(index, 2);
        v128 mask1 = shl16(index, 3);

        return blend8(mask0, blend8(mask1, w, x), blend8(mask1, y, z));
    }

    static forceinline v128 shl16(v128 a, int shift) {
        return {_mm_slli_epi16(a.raw, shift)};
    }

    static forceinline u16 eq8(v128 a, v128 b) {
        return static_cast<u16>(_mm_movemask_epi8(_mm_cmpeq_epi8(a.raw, b.raw)));
    }

    static forceinline u16 neq8(v128 a, v128 b) {
        return static_cast<u16>(~_mm_movemask_epi8(_mm_cmpeq_epi8(a.raw, b.raw)));
    }

    [[nodiscard]] forceinline u16 nonzero8() const {
        return neq8(*this, zero());
    }

    [[nodiscard]] forceinline bool operator==(const v128& other) const {
        __m128i t = _mm_xor_si128(raw, other.raw);
        return _mm_testz_si128(t, t);
    }
};
static_assert(sizeof(v128) == 16);

struct v256 {
    __m256i raw{};

    using Mask8  = u32;
    using Mask16 = u16;
    using Mask32 = u8;
    using Mask64 = u8;

    forceinline constexpr v256() = default;
    forceinline constexpr v256(__m256i raw) :
        raw(raw){};
    forceinline explicit v256(std::array<u8, 32> src) :
        raw(std::bit_cast<__m256i>(src)) {
    }
    forceinline explicit v256(std::array<u16, 16> src) :
        raw(std::bit_cast<__m256i>(src)) {
    }
    forceinline explicit v256(v128 a, v128 b) :
        raw(std::bit_cast<__m256i>(std::array<v128, 2>{a, b})){};

    static forceinline v256 zero() {
        return {_mm256_setzero_si256()};
    }

    static forceinline v256 broadcast8(u8 x) {
        return {_mm256_set1_epi8(static_cast<i8>(x))};
    }
    static forceinline v256 broadcast16(u16 x) {
        return {_mm256_set1_epi16(static_cast<i16>(x))};
    }
    static forceinline v256 broadcast64(u64 x) {
        return {_mm256_set1_epi64x(static_cast<i64>(x))};
    }
    static forceinline v256 broadcast128(v128 x) {
        return {_mm256_broadcastsi128_si256(x.raw)};
    }

    [[nodiscard]] forceinline v256 broadcast128lo() const {
        return {_mm256_permute2x128_si256(raw, raw, 0b00000000)};
    }
    [[nodiscard]] forceinline v256 broadcast128hi() const {
        return {_mm256_permute2x128_si256(raw, raw, 0b00010001)};
    }

    static forceinline v256 from128(v128 a) {
        return {_mm256_castsi128_si256(a.raw)};
    }

    [[nodiscard]] forceinline v128 to128() const {
        return {_mm256_castsi256_si128(raw)};
    }

    static forceinline v256 add8(v256 a, v256 b) {
        return {_mm256_add_epi8(a.raw, b.raw)};
    }

    static forceinline v256 blend8(v256 mask, v256 a, v256 b) {
        return {_mm256_blendv_epi8(a.raw, b.raw, mask.raw)};
    }

    static forceinline v256 sliderbroadcast(v256 a) {
        __m256i        x    = _mm256_sad_epu8(a.raw, _mm256_setzero_si256());
        constexpr char NONE = static_cast<char>(0xFF);
        const __m256i  EXPAND_IDX{_mm256_setr_epi8(NONE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                   NONE, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
                                                   NONE, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
                                                   NONE, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18)};
        return {_mm256_shuffle_epi8(x, EXPAND_IDX)};
    }

    static forceinline v256 permute8(v256 index, v128 a) {
        return {_mm256_shuffle_epi8(v256::broadcast128(a).raw, index.raw)};
    }

    static forceinline v256 permute8(v256 index, v256 a, v256 b) {
        v256 mask0 = shl16(index, 2);
        v256 mask1 = shl16(index, 3);

        v256 x = blend8(mask1, _mm256_shuffle_epi8(a.broadcast128lo().raw, index.raw),
                        _mm256_shuffle_epi8(a.broadcast128hi().raw, index.raw));
        v256 y = blend8(mask1, _mm256_shuffle_epi8(b.broadcast128lo().raw, index.raw),
                        _mm256_shuffle_epi8(b.broadcast128hi().raw, index.raw));

        return blend8(mask0, x, y);
    }

    static forceinline v256 shl16(v256 a, int shift) {
        return {_mm256_slli_epi16(a.raw, shift)};
    }

    static forceinline v256 shr16(v256 a, int shift) {
        return {_mm256_srli_epi16(a.raw, shift)};
    }

    static forceinline v256 sub64(v256 a, v256 b) {
        return {_mm256_sub_epi64(a.raw, b.raw)};
    }

    static forceinline u32 test8(v256 a, v256 b) {
        return v256::neq8(a & b, v256::zero());
    }

    static forceinline v256 unpacklo8(v256 a, v256 b) {
        return {_mm256_unpacklo_epi8(a.raw, b.raw)};
    }

    static forceinline v256 unpackhi8(v256 a, v256 b) {
        return {_mm256_unpackhi_epi8(a.raw, b.raw)};
    }

    static forceinline u32 eq8(v256 a, v256 b) {
        return eq8_vm(a, b).msb8();
    }

    static forceinline v256 eq8_vm(v256 a, v256 b) {
        return {_mm256_cmpeq_epi8(a.raw, b.raw)};
    }

    static forceinline v256 eq64_vm(v256 a, v256 b) {
        return {_mm256_cmpeq_epi64(a.raw, b.raw)};
    }

    static forceinline v256 gts8_vm(v256 a, v256 b) {
        return {_mm256_cmpgt_epi8(a.raw, b.raw)};
    }

    static forceinline u32 neq8(v256 a, v256 b) {
        return static_cast<u32>(~_mm256_movemask_epi8(_mm256_cmpeq_epi8(a.raw, b.raw)));
    }

    static forceinline u16 neq16(v256 a, v256 b) {
        return static_cast<u16>(_pext_u32(
          static_cast<u32>(~_mm256_movemask_epi8(_mm256_cmpeq_epi16(a.raw, b.raw))), 0xAAAAAAAA));
    }

    template<int offset>
    [[nodiscard]] forceinline v128 extract128() const {
        return {_mm256_extracti128_si256(raw, offset)};
    }
    [[nodiscard]] forceinline u32 msb8() const {
        return static_cast<u32>(_mm256_movemask_epi8(raw));
    }

    friend forceinline v256 operator&(v256 a, v256 b) {
        return {_mm256_and_si256(a.raw, b.raw)};
    }
    friend forceinline v256 operator|(v256 a, v256 b) {
        return {_mm256_or_si256(a.raw, b.raw)};
    }
    friend forceinline v256 operator^(v256 a, v256 b) {
        return {_mm256_xor_si256(a.raw, b.raw)};
    }
    static forceinline v256 andnot(v256 a, v256 b) {
        return {_mm256_andnot_si256(a.raw, b.raw)};
    }

    forceinline bool operator==(const v256& other) const {
        __m256i t = _mm256_xor_si256(raw, other.raw);
        return _mm256_testz_si256(t, t);
    }
};
static_assert(sizeof(v256) == 32);

struct v512 {
    std::array<v256, 2> raw{};

    using Mask8  = u64;
    using Mask16 = u32;
    using Mask32 = u16;
    using Mask64 = u8;

    forceinline constexpr v512() = default;
    forceinline constexpr explicit v512(v256 a, v256 b) :
        raw({a, b}){};
    forceinline explicit v512(std::array<u8, 64> src) :
        raw(std::bit_cast<std::array<v256, 2>>(src)) {
    }
    forceinline explicit v512(std::array<u16, 32> src) :
        raw(std::bit_cast<std::array<v256, 2>>(src)) {
    }

    static forceinline v512 zero() {
        return v512{v256::zero(), v256::zero()};
    }

    static forceinline v512 broadcast8(u8 x) {
        return v512{v256::broadcast8(x), v256::broadcast8(x)};
    }
    static forceinline v512 broadcast16(u16 x) {
        return v512{v256::broadcast16(x), v256::broadcast16(x)};
    }
    static forceinline v512 broadcast64(u64 x) {
        return v512{v256::broadcast64(x), v256::broadcast64(x)};
    }

    static forceinline v512 from128(v128 a) {
        return v512{v256::from128(a), v256::zero()};
    }

    [[nodiscard]] forceinline v128 to128() const {
        return raw[0].to128();
    }

    static forceinline v512 add8(v512 a, v512 b) {
        return v512{v256::add8(a.raw[0], b.raw[0]), v256::add8(a.raw[1], b.raw[1])};
    }

    static forceinline v512 compress8(u64 m, v512 a) {
        // TODO: Slow
        std::array<u8, 64> result{};
        auto               in = std::bit_cast<std::array<u8, 64>>(a);
        for (usize i = 0; m != 0; i++, m = clear_lowest_bit(m)) {
            result[i] = in[static_cast<usize>(std::countr_zero(m))];
        }
        return std::bit_cast<v512>(result);
    }

    static forceinline v512 sliderbroadcast(v512 a) {
        return v512{v256::sliderbroadcast(a.raw[0]), v256::sliderbroadcast(a.raw[1])};
    }

    static forceinline v512 permute8(v512 index, v512 a) {
        return v512{v256::permute8(index.raw[0], a.raw[0], a.raw[1]),
                    v256::permute8(index.raw[1], a.raw[0], a.raw[1])};
    }

    static forceinline v512 permute8(v512 index, v128 a) {
        return v512{v256::permute8(index.raw[0], a), v256::permute8(index.raw[1], a)};
    }

    static forceinline v512 shr16(v512 a, int shift) {
        return v512{v256::shr16(a.raw[0], shift), v256::shr16(a.raw[1], shift)};
    }

    static forceinline v512 sub64(v512 a, v512 b) {
        return v512{v256::sub64(a.raw[0], b.raw[0]), v256::sub64(a.raw[1], b.raw[1])};
    }

    static forceinline u64 test8(v512 a, v512 b) {
        return concat64(v256::test8(a.raw[0], b.raw[0]), v256::test8(a.raw[1], b.raw[1]));
    }

    static forceinline v512 unpacklo8(v512 a, v512 b) {
        return v512{v256::unpacklo8(a.raw[0], b.raw[0]), v256::unpacklo8(a.raw[1], b.raw[1])};
    }

    static forceinline v512 unpackhi8(v512 a, v512 b) {
        return v512{v256::unpackhi8(a.raw[0], b.raw[0]), v256::unpackhi8(a.raw[1], b.raw[1])};
    }

    static forceinline u64 eq8(v512 a, v512 b) {
        return concat64(v256::eq8(a.raw[0], b.raw[0]), v256::eq8(a.raw[1], b.raw[1]));
    }

    static forceinline v512 eq8_vm(v512 a, v512 b) {
        return v512{v256::eq8_vm(a.raw[0], b.raw[0]), v256::eq8_vm(a.raw[1], b.raw[1])};
    }

    static forceinline v512 eq64_vm(v512 a, v512 b) {
        return v512{v256::eq64_vm(a.raw[0], b.raw[0]), v256::eq64_vm(a.raw[1], b.raw[1])};
    }

    static forceinline v512 gts8_vm(v512 a, v512 b) {
        return v512{v256::gts8_vm(a.raw[0], b.raw[0]), v256::gts8_vm(a.raw[1], b.raw[1])};
    }

    static forceinline u64 neq8(v512 a, v512 b) {
        return concat64(v256::neq8(a.raw[0], b.raw[0]), v256::neq8(a.raw[1], b.raw[1]));
    }

    static forceinline u32 neq16(v512 a, v512 b) {
        return concat32(v256::neq16(a.raw[0], b.raw[0]), v256::neq16(a.raw[1], b.raw[1]));
    }

    static forceinline u64 testn8(v512 a, v512 b) {
        return (a & b).zero8();
    }

    [[nodiscard]] forceinline u64 msb8() const {
        return concat64(raw[0].msb8(), raw[1].msb8());
    }
    [[nodiscard]] forceinline u64 zero8() const {
        return eq8(*this, zero());
    }
    [[nodiscard]] forceinline u64 nonzero8() const {
        return neq8(*this, zero());
    }
    [[nodiscard]] forceinline u32 nonzero16() const {
        return neq16(*this, zero());
    }

    friend forceinline v512 operator&(v512 a, v512 b) {
        return v512{a.raw[0] & b.raw[0], a.raw[1] & b.raw[1]};
    }
    friend forceinline v512 operator|(v512 a, v512 b) {
        return v512{a.raw[0] | b.raw[0], a.raw[1] | b.raw[1]};
    }
    friend forceinline v512 operator^(v512 a, v512 b) {
        return v512{a.raw[0] ^ b.raw[0], a.raw[1] ^ b.raw[1]};
    }
    static forceinline v512 andnot(v512 a, v512 b) {
        return v512{v256::andnot(a.raw[0], b.raw[0]), v256::andnot(a.raw[1], b.raw[1])};
    }

    friend forceinline v512& operator&=(v512& a, v512 b) {
        return a = a & b;
    }

    friend forceinline v512& operator|=(v512& a, v512 b) {
        return a = a | b;
    }

    friend forceinline v512& operator^=(v512& a, v512 b) {
        return a = a ^ b;
    }

    forceinline auto operator==(const v512& other) const -> bool {
        return raw == other.raw;
    }
};
static_assert(sizeof(v512) == 64);

forceinline u16 findset8(v128 haystack, int haystack_len, v128 needles) {
    return static_cast<u16>(
      _mm_extract_epi16(_mm_cmpestrm(haystack.raw, haystack_len, needles.raw, 16, 0), 0));
}

}

#include "geometry.hpp"
#include <bit>

namespace Clockwork::geometry {

// clang-format off
// Offset arrangement is AVX2-specific (due to punpck-ordering).
constexpr std::array<u8, 64> AVX2_OFFSETS{{
    0210, 0211, 0212, 0213, 0214, 0215, 0216, 0217,
    0310, 0311, 0312, 0313, 0314, 0315, 0316, 0317,
    0230, 0231, 0232, 0233, 0234, 0235, 0236, 0237,
    0330, 0331, 0332, 0333, 0334, 0335, 0336, 0337,
    0250, 0251, 0252, 0253, 0254, 0255, 0256, 0257,
    0350, 0351, 0352, 0353, 0354, 0355, 0356, 0357,
    0270, 0271, 0272, 0273, 0274, 0275, 0276, 0277,
    0370, 0371, 0372, 0373, 0374, 0375, 0376, 0377,
}};
// clang-format on


const std::array<u8x64, 64> SUPERPIECE_INVERSE_RAYS_AVX2_TABLE = []() {
    // clang-format off
    constexpr u8 NONE = 0x80;
    constexpr std::array<u8, 256> BASE{{
        NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
        NONE, 0x2F, NONE, NONE, NONE, NONE, NONE, NONE, 0x27, NONE, NONE, NONE, NONE, NONE, NONE, 0x1F,
        NONE, NONE, 0x2E, NONE, NONE, NONE, NONE, NONE, 0x26, NONE, NONE, NONE, NONE, NONE, 0x1E, NONE,
        NONE, NONE, NONE, 0x2D, NONE, NONE, NONE, NONE, 0x25, NONE, NONE, NONE, NONE, 0x1D, NONE, NONE,
        NONE, NONE, NONE, NONE, 0x2C, NONE, NONE, NONE, 0x24, NONE, NONE, NONE, 0x1C, NONE, NONE, NONE,
        NONE, NONE, NONE, NONE, NONE, 0x2B, NONE, NONE, 0x23, NONE, NONE, 0x1B, NONE, NONE, NONE, NONE,
        NONE, NONE, NONE, NONE, NONE, NONE, 0x2A, 0x28, 0x22, 0x20, 0x1A, NONE, NONE, NONE, NONE, NONE,
        NONE, NONE, NONE, NONE, NONE, NONE, 0x30, 0x29, 0x21, 0x19, 0x18, NONE, NONE, NONE, NONE, NONE,
        NONE, 0x37, 0x36, 0x35, 0x34, 0x33, 0x32, 0x31, NONE, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
        NONE, NONE, NONE, NONE, NONE, NONE, 0x38, 0x39, 0x01, 0x09, 0x10, NONE, NONE, NONE, NONE, NONE,
        NONE, NONE, NONE, NONE, NONE, NONE, 0x3A, 0x00, 0x02, 0x08, 0x0A, NONE, NONE, NONE, NONE, NONE,
        NONE, NONE, NONE, NONE, NONE, 0x3B, NONE, NONE, 0x03, NONE, NONE, 0x0B, NONE, NONE, NONE, NONE,
        NONE, NONE, NONE, NONE, 0x3C, NONE, NONE, NONE, 0x04, NONE, NONE, NONE, 0x0C, NONE, NONE, NONE,
        NONE, NONE, NONE, 0x3D, NONE, NONE, NONE, NONE, 0x05, NONE, NONE, NONE, NONE, 0x0D, NONE, NONE,
        NONE, NONE, 0x3E, NONE, NONE, NONE, NONE, NONE, 0x06, NONE, NONE, NONE, NONE, NONE, 0x0E, NONE,
        NONE, 0x3F, NONE, NONE, NONE, NONE, NONE, NONE, 0x07, NONE, NONE, NONE, NONE, NONE, NONE, 0x0F,
    }};
    // clang-format on

    std::array<u8x64, 64> table;
    for (u8 sq = 0; sq < 64; sq++) {
        u8                 esq = internal::expand_sq(Square{sq});
        std::array<u8, 64> b;
        for (usize i = 0; i < 64; i++) {
            u8 value = BASE[AVX2_OFFSETS[i] - esq];
            b[i]     = value;
        }
        table[sq] = u8x64{b};
    }
    return table;
}();

const std::array<u8x64, 64> PIECE_MOVES_AVX2_TABLE = []() {
    // clang-format off
    constexpr u8 K = 1 << static_cast<i32>(PieceType::King);
    constexpr u8 Q = 1 << static_cast<i32>(PieceType::Queen);
    constexpr u8 B = 1 << static_cast<i32>(PieceType::Bishop);
    constexpr u8 R = 1 << static_cast<i32>(PieceType::Rook);
    constexpr u8 HORS = 1 << static_cast<i32>(PieceType::Knight);
    constexpr u8 ORTH = Q | R;
    constexpr u8 DIAG = Q | B;
    constexpr u8 OADJ = ORTH | K;
    constexpr u8 WPDG = DIAG | K | 0b01;
    constexpr u8 BPDG = DIAG | K | 0b10;

    constexpr std::array<u8, 256> BASE{{
           0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
           0, DIAG,    0,    0,    0,    0,    0,    0, ORTH,    0,    0,    0,    0,    0,    0, DIAG,
           0,    0, DIAG,    0,    0,    0,    0,    0, ORTH,    0,    0,    0,    0,    0, DIAG,    0,
           0,    0,    0, DIAG,    0,    0,    0,    0, ORTH,    0,    0,    0,    0, DIAG,    0,    0,
           0,    0,    0,    0, DIAG,    0,    0,    0, ORTH,    0,    0,    0, DIAG,    0,    0,    0,
           0,    0,    0,    0,    0, DIAG,    0,    0, ORTH,    0,    0, DIAG,    0,    0,    0,    0,
           0,    0,    0,    0,    0,    0, DIAG, HORS, ORTH, HORS, DIAG,    0,    0,    0,    0,    0,
           0,    0,    0,    0,    0,    0, HORS, BPDG, OADJ, BPDG, HORS,    0,    0,    0,    0,    0,
           0, ORTH, ORTH, ORTH, ORTH, ORTH, ORTH, OADJ,    0, OADJ, ORTH, ORTH, ORTH, ORTH, ORTH, ORTH,
           0,    0,    0,    0,    0,    0, HORS, WPDG, OADJ, WPDG, HORS,    0,    0,    0,    0,    0,
           0,    0,    0,    0,    0,    0, DIAG, HORS, ORTH, HORS, DIAG,    0,    0,    0,    0,    0,
           0,    0,    0,    0,    0, DIAG,    0,    0, ORTH,    0,    0, DIAG,    0,    0,    0,    0,
           0,    0,    0,    0, DIAG,    0,    0,    0, ORTH,    0,    0,    0, DIAG,    0,    0,    0,
           0,    0,    0, DIAG,    0,    0,    0,    0, ORTH,    0,    0,    0,    0, DIAG,    0,    0,
           0,    0, DIAG,    0,    0,    0,    0,    0, ORTH,    0,    0,    0,    0,    0, DIAG,    0,
           0, DIAG,    0,    0,    0,    0,    0,    0, ORTH,    0,    0,    0,    0,    0,    0, DIAG,
    }};
    // clang-format on

    std::array<u8x64, 64> table;
    for (u8 sq = 0; sq < 64; sq++) {
        u8                 esq = internal::expand_sq(Square{sq});
        std::array<u8, 64> b;
        for (usize i = 0; i < 64; i++) {
            b[i] = BASE[AVX2_OFFSETS[i] - esq];
        }
        table[sq] = u8x64{b};
    }
    return table;
}();

}  // namespace Clockwork::geometry

#include "bb_attacks.hpp"
#include <array>
#include <bit>

namespace Clockwork {

namespace {

constexpr std::array<std::array<Bitboard, 64>, 2> PAWN_ATTACKS = []() {
    std::array<std::array<Bitboard, 64>, 2> result{};

    for (const auto color : {Color::White, Color::Black}) {
        for (u8 square_idx = 0; square_idx < 64; square_idx++) {
            const auto square = Square{square_idx};
            const auto bit    = Bitboard::from_square(square);

            auto& bb = result[static_cast<usize>(color)][square_idx];

            bb |= bit.shift_relative(color, Direction::NorthWest);
            bb |= bit.shift_relative(color, Direction::NorthEast);
        }
    }

    return result;
}();

constexpr std::array<Bitboard, 64> KNIGHT_ATTACKS = []() {
    std::array<Bitboard, 64> result{};

    for (u8 square_idx = 0; square_idx < 64; square_idx++) {
        const auto square = Square{square_idx};
        const auto bit    = Bitboard::from_square(square);

        auto& bb = result[square_idx];

        bb |= bit.shift(Direction::North).shift(Direction::NorthWest);
        bb |= bit.shift(Direction::North).shift(Direction::NorthEast);
        bb |= bit.shift(Direction::South).shift(Direction::SouthWest);
        bb |= bit.shift(Direction::South).shift(Direction::SouthEast);
        bb |= bit.shift(Direction::West).shift(Direction::NorthWest);
        bb |= bit.shift(Direction::West).shift(Direction::SouthWest);
        bb |= bit.shift(Direction::East).shift(Direction::NorthEast);
        bb |= bit.shift(Direction::East).shift(Direction::SouthEast);
    }

    return result;
}();

constexpr std::array<Bitboard, 64> KING_ATTACKS = []() {
    std::array<Bitboard, 64> result{};

    for (u8 square_idx = 0; square_idx < 64; square_idx++) {
        const auto square = Square{square_idx};
        const auto bit    = Bitboard::from_square(square);

        auto& bb = result[square_idx];

        bb |= bit.shift(Direction::North);
        bb |= bit.shift(Direction::South);
        bb |= bit.shift(Direction::West);
        bb |= bit.shift(Direction::East);
        bb |= bit.shift(Direction::NorthWest);
        bb |= bit.shift(Direction::NorthEast);
        bb |= bit.shift(Direction::SouthWest);
        bb |= bit.shift(Direction::SouthEast);
    }

    return result;
}();

constexpr Bitboard DIAG = Bitboard{0x8040201008040201};

constexpr Bitboard FILE_A = Bitboard{0x0101010101010101};
constexpr Bitboard FILE_H = Bitboard{0x8080808080808080};

struct BishopMasks {
    u64 bit;
    u64 diag;
    u64 anti_diag;
    u64 flipped;
};

constexpr std::array<BishopMasks, 64> BISHOP_MASKS = []() {
    std::array<BishopMasks, 64> result{};

    constexpr std::array<u64, 15> DIAGS = []() {
        std::array<u64, 15> result{};

        for (usize i = 0; i < 15; i++) {
            if (i > 7) {
                result[i] = DIAG.value() >> (8 * (i - 7));
            } else {
                result[i] = DIAG.value() << (8 * (7 - i));
            }
        }

        return result;
    }();

    for (u8 square_idx = 0; square_idx < 64; square_idx++) {
        const auto square = Square{square_idx};
        const auto bit    = Bitboard::from_square(square).value();

        const auto file = static_cast<usize>(square.file());
        const auto rank = static_cast<usize>(square.rank());

        auto& masks = result[square_idx];

        masks.bit       = bit;
        masks.diag      = bit ^ DIAGS[7 + file - rank];
        masks.anti_diag = bit ^ __builtin_bswap64(DIAGS[file + rank]);
        masks.flipped   = __builtin_bswap64(bit);
    }

    return result;
}();

constexpr std::array<usize, 64> RANK_SHIFTS = []() {
    std::array<usize, 64> result{};

    for (usize square_idx = 0; square_idx < 64; square_idx++) {
        result[square_idx] = (square_idx & 0b111000) + 1;
    }

    return result;
}();

constexpr std::array<std::array<Bitboard, 64>, 64> RANK_ATTACKS = []() {
    std::array<std::array<Bitboard, 64>, 64> result{};

    constexpr std::array<Bitboard, 64> WEST = []() {
        std::array<Bitboard, 64> result{};

        for (u8 square_idx = 0; square_idx < 64; square_idx++) {
            const auto square = Square{square_idx};
            const auto bit    = Bitboard::from_square(square).value();

            result[square_idx] = Bitboard{(bit - 1) & (u64{0xFF} << (square_idx & 0b111000))};
        }

        return result;
    }();

    constexpr std::array<Bitboard, 64> EAST = [&WEST]() {
        std::array<Bitboard, 64> result{};

        for (u8 square_idx = 0; square_idx < 64; square_idx++) {
            const auto square = Square{square_idx};
            const auto bit    = Bitboard::from_square(square).value();

            result[square_idx] =
              Bitboard{bit ^ WEST[square_idx].value() ^ (u64{0xFF} << (square_idx & 0b111000))};
        }

        return result;
    }();

    for (u8 square_idx = 0; square_idx < 64; square_idx++) {
        const auto square = Square{square_idx};
        const auto file   = static_cast<usize>(square.file());

        for (u64 i = 0; i < 64; i++) {
            const auto occ = i << 1;

            auto& bb = result[square_idx][i];

            const auto east = EAST[file]
                            ^ EAST[static_cast<usize>(
                              std::countr_zero((EAST[file].value() & occ) | (u64{1} << 63)))];
            const auto west =
              WEST[file]
              ^ WEST[static_cast<usize>(std::countl_zero((WEST[file].value() & occ) | 1) ^ 63)];

            bb = (east | west) << static_cast<i32>(square_idx - file);
        }
    }

    return result;
}();

constexpr std::array<std::array<Bitboard, 64>, 64> FILE_ATTACKS = []() {
    std::array<std::array<Bitboard, 64>, 64> result{};

    for (u8 square_idx = 0; square_idx < 64; square_idx++) {
        const auto square = Square{square_idx};
        const auto rank   = static_cast<usize>(square.rank());

        for (u64 occ = 0; occ < 64; occ++) {
            const auto rank_attacks = RANK_ATTACKS[7 - rank][occ].value();
            result[square_idx][occ] =
              Bitboard{((rank_attacks * DIAG.value()) & FILE_H.value()) >> (7 - square.file())};
        }
    }

    return result;
}();

}

Bitboard pawn_attacks(Square square, Color color) {
    return PAWN_ATTACKS[static_cast<usize>(color)][square.raw];
}

Bitboard knight_attacks(Square square) {
    return KNIGHT_ATTACKS[square.raw];
}

Bitboard bishop_attacks(Square square, Bitboard occupancy) {
    const auto& masks = BISHOP_MASKS[square.raw];

    auto diag_attacks = occupancy.value() & masks.diag;
    auto flipped_diag = __builtin_bswap64(diag_attacks);
    diag_attacks -= masks.bit;
    flipped_diag -= masks.flipped;
    diag_attacks ^= __builtin_bswap64(flipped_diag);
    diag_attacks &= masks.diag;

    auto anti_diag_attacks = occupancy.value() & masks.anti_diag;
    auto flipped_anti_diag = __builtin_bswap64(anti_diag_attacks);
    anti_diag_attacks -= masks.bit;
    flipped_anti_diag -= masks.flipped;
    anti_diag_attacks ^= __builtin_bswap64(flipped_anti_diag);
    anti_diag_attacks &= masks.anti_diag;

    return Bitboard{diag_attacks | anti_diag_attacks};
}

Bitboard rook_attacks(Square square, Bitboard occupancy) {
    const auto flip    = ((occupancy.value() >> square.file()) & FILE_A.value()) * DIAG.value();
    const auto file_sq = (flip >> 57) & 0x3F;
    const auto file_attacks = FILE_ATTACKS[square.raw][file_sq];

    const auto rank_sq      = (occupancy.value() >> RANK_SHIFTS[square.raw]) & 0x3F;
    const auto rank_attacks = RANK_ATTACKS[square.raw][rank_sq];

    return file_attacks | rank_attacks;
}

Bitboard queen_attacks(Square square, Bitboard occupancy) {
    return bishop_attacks(square, occupancy) | rook_attacks(square, occupancy);
}

Bitboard king_attacks(Square square) {
    return KING_ATTACKS[square.raw];
}

}

#pragma once

#include "common.hpp"
#include "square.hpp"
#include "util/types.hpp"
#include <array>

namespace Clockwork {


struct alignas(64) ZobristInfo {
    u64x8 m_raw = {};

    // Structure:
    // [0] - full zobrist key
    // [1] - pawn key
    // [2] - non-pawn key for white
    // [3] - non-pawn key for black
    // [4] - major piece key
    // [5] - minor piece key
    // [6-7] - padding

    // Constructor from the individual components
    constexpr ZobristInfo(HashKey piece_key,
                          HashKey pawn_key,
                          HashKey non_pawn_key_white,
                          HashKey non_pawn_key_black,
                          HashKey major_key,
                          HashKey minor_key) :
        m_raw{{
          piece_key, pawn_key, non_pawn_key_white, non_pawn_key_black, major_key, minor_key,
          0ull,  // padding
          0ull   // padding
        }} {
    }

    // Alternative constructor that takes std::array for the nonpawn keys
    constexpr ZobristInfo(HashKey                piece_key,
                          HashKey                pawn_key,
                          std::array<HashKey, 2> non_pawn_keys,
                          HashKey                major_key,
                          HashKey                minor_key) :
        ZobristInfo(piece_key, pawn_key, non_pawn_keys[0], non_pawn_keys[1], major_key, minor_key) {
    }

    // Default constructor
    constexpr ZobristInfo() :
        m_raw{} {
    }

    // Operator ==
    constexpr bool operator==(const ZobristInfo& other) const {
        return m_raw == other.m_raw;
    }

    inline void toggle(const Color color, const PieceType ptype, const Square sq);

    inline void update_fullkey(HashKey update) {
        m_raw ^= u64x8({update, 0, 0, 0, 0, 0, 0, 0});
    }

    [[nodiscard]] HashKey full_key() const {
        return m_raw.read(0);
    }
    [[nodiscard]] HashKey pawn_key() const {
        return m_raw.read(1);
    }
    [[nodiscard]] HashKey non_pawn_key(Color color) const {
        return m_raw.read(2 + static_cast<usize>(color));
    }
    [[nodiscard]] HashKey major_key() const {
        return m_raw.read(4);
    }
    [[nodiscard]] HashKey minor_key() const {
        return m_raw.read(5);
    }
};

class Zobrist {
public:
    Zobrist() = delete;

    static std::array<std::array<std::array<HashKey, 64>, 7>, 2> piece_square_zobrist;
    static std::array<HashKey, 64>                               en_passant_zobrist;
    static std::array<HashKey, 16>                               castling_zobrist;
    static HashKey                                               side_key;

    // ZobristInfo update table
    static std::array<std::array<std::array<ZobristInfo, 64>, 7>, 2> piece_zobrist_info;

    static void init_zobrist_keys();
};

inline void ZobristInfo::toggle(const Color color, const PieceType ptype, const Square sq) {
    const ZobristInfo& info =
      Zobrist::piece_zobrist_info[static_cast<usize>(color)][static_cast<usize>(ptype)][sq.raw];
    m_raw ^= info.m_raw;
}


// Make sure to align to u64x8


}  // namespace Clockwork

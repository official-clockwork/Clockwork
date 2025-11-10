#pragma once

#include "bitboard.hpp"
#include "square.hpp"
#include "util/bit.hpp"
#include "util/types.hpp"
#include <algorithm>
#include <array>
#include <bit>
#include <cmath>

namespace Clockwork::rays {

using Table = std::array<std::array<Bitboard, 64>, 64>;

template<typename F>
[[nodiscard]] consteval Table generate_rays(F f) {
    Table result{};

    for (u8 a = 0; a < 64; a++) {
        for (u8 b = 0; b < 64; b++) {
            if (a == b) {
                continue;
            }

            Bitboard bb{};

            Square a_sq{a};
            Square b_sq{b};

            bb |= Bitboard::from_square(a_sq);
            bb |= Bitboard::from_square(b_sq);

            auto [b_file, b_rank] = b_sq.to_file_and_rank();
            auto [a_file, a_rank] = a_sq.to_file_and_rank();
            int file_diff         = b_file - a_file;
            int rank_diff         = b_rank - a_rank;

            if (file_diff == 0 || rank_diff == 0 || file_diff == rank_diff
                || file_diff == -rank_diff) {
                int file_delta = sign(file_diff);
                int rank_delta = sign(rank_diff);

                f(bb, a_file, a_rank, b_file, b_rank, rank_delta, file_delta);
            }

            result[a][b] = bb;
        }
    }

    return result;
}

inline constexpr Table INCLUSIVE_TABLE = generate_rays(
  [](Bitboard& bb, int a_file, int a_rank, int b_file, int b_rank, int rank_delta, int file_delta) {
      int file = a_file;
      int rank = a_rank;
      for (; file != b_file || rank != b_rank; file += file_delta, rank += rank_delta) {
          bb |= Bitboard::from_square(Square::from_file_and_rank(file, rank));
      }
  });

[[nodiscard]] constexpr Bitboard inclusive(Square a, Square b) {
    return INCLUSIVE_TABLE[a.raw][b.raw];
}

inline constexpr Table EXCLUSIVE_TABLE = generate_rays(
  [](Bitboard& bb, int a_file, int a_rank, int b_file, int b_rank, int rank_delta, int file_delta) {
      int        file  = a_file;
      int        rank  = a_rank;
      const auto first = Bitboard::from_square(Square::from_file_and_rank(file, rank));
      Bitboard   last{};
      for (; file != b_file || rank != b_rank; file += file_delta, rank += rank_delta) {
          const auto x = Bitboard::from_square(Square::from_file_and_rank(file, rank));
          last         = x;
          bb |= x;
      }
      bb &= ~first;
      bb &= ~last;
  });

[[nodiscard]] constexpr Bitboard exclusive(Square a, Square b) {
    return EXCLUSIVE_TABLE[a.raw][b.raw];
}

inline constexpr Table INFINITE_EXCLUSIVE_TABLE = generate_rays(
  [](Bitboard& bb, int a_file, int a_rank, int b_file, int b_rank, int rank_delta, int file_delta) {
      int file = a_file;
      int rank = a_rank;
      for (; file >= 0 && file <= 7 && rank >= 0 && rank <= 7;
           file += file_delta, rank += rank_delta) {
          bb |= Bitboard::from_square(Square::from_file_and_rank(file, rank));
      }
      file = a_file;
      rank = a_rank;
      for (; file >= 0 && file <= 7 && rank >= 0 && rank <= 7;
           file -= file_delta, rank -= rank_delta) {
          bb |= Bitboard::from_square(Square::from_file_and_rank(file, rank));
      }
      bb &= ~Bitboard::from_square(Square::from_file_and_rank(a_file, a_rank));
      bb &= ~Bitboard::from_square(Square::from_file_and_rank(b_file, b_rank));
  });

[[nodiscard]] constexpr Bitboard infinite_exclusive(Square a, Square b) {
    return INFINITE_EXCLUSIVE_TABLE[a.raw][b.raw];
}

}  // namespace Clockwork::rays

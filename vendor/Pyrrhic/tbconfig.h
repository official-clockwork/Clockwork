/*
 * (c) 2015 basil, all rights reserved,
 * Modifications Copyright (c) 2016-2019 by Jon Dart
 * Modifications Copyright (c) 2020-2026 by Andrew Grant
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#include "../../src/bb_attacks.hpp"
#include <bit>

#define PYRRHIC_POPCOUNT(x) (std::popcount(x))
#define PYRRHIC_LSB(x) (std::countr_zero(x))
#define PYRRHIC_POPLSB(x)                      \
    ([](Clockwork::u64* y) {                   \
        const auto lsb = std::countr_zero(*y); \
        *y &= *y - 1;                          \
        return lsb;                            \
    }(x))

#define PYRRHIC_PAWN_ATTACKS(sq, c)                                             \
    (Clockwork::pawn_attacks(Clockwork::Square{static_cast<Clockwork::u8>(sq)}, \
                             static_cast<Clockwork::Color>(!c))                 \
       .value())
#define PYRRHIC_KNIGHT_ATTACKS(sq)                                                         \
    (Clockwork::knight_attacks(Clockwork::Square{static_cast<Clockwork::u8>(sq)}).value())
#define PYRRHIC_BISHOP_ATTACKS(sq, occ)                                           \
    (Clockwork::bishop_attacks(Clockwork::Square{static_cast<Clockwork::u8>(sq)}, \
                               Clockwork::Bitboard{occ})                          \
       .value())
#define PYRRHIC_ROOK_ATTACKS(sq, occ)                                           \
    (Clockwork::rook_attacks(Clockwork::Square{static_cast<Clockwork::u8>(sq)}, \
                             Clockwork::Bitboard{occ})                          \
       .value())
#define PYRRHIC_QUEEN_ATTACKS(sq, occ)                                           \
    (Clockwork::queen_attacks(Clockwork::Square{static_cast<Clockwork::u8>(sq)}, \
                              Clockwork::Bitboard{occ})                          \
       .value())
#define PYRRHIC_KING_ATTACKS(sq)                                                         \
    (Clockwork::king_attacks(Clockwork::Square{static_cast<Clockwork::u8>(sq)}).value())

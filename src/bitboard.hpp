#pragma once

#include <bit>

#include "common.hpp"
#include "square.hpp"
#include "util/bit.hpp"
#include "util/types.hpp"

namespace Clockwork {

struct Bitboard {
    u64 raw;

    static Bitboard from_square(Square sq) {
        return Bitboard{static_cast<u64>(1) << sq.raw};
    }

    // Rank closest to player
    [[nodiscard]] u8 front_rank(Color color) const {
        int color_shift = color == Color::White ? 0 : 56;
        return static_cast<u8>(raw >> color_shift);
    }

    struct Iterator {
    public:
        Iterator& operator++() {
            bb = clear_lowest_bit(bb);
            return *this;
        }

        Square operator*() const {
            return Square{static_cast<u8>(std::countr_zero(bb))};
        }

        bool operator==(const Iterator&) const = default;

    private:
        friend class Bitboard;
        explicit constexpr Iterator(u64 bb) :
            bb(bb) {
        }
        u64 bb;
    };

    [[nodiscard]] Iterator begin() const {
        return Iterator{raw};
    }
    [[nodiscard]] Iterator end() const {
        return Iterator{0};
    }

    friend Bitboard operator~(Bitboard a) {
        return Bitboard{~a.raw};
    }
    friend Bitboard operator&(Bitboard a, Bitboard b) {
        return Bitboard{a.raw & b.raw};
    }
    friend Bitboard operator|(Bitboard a, Bitboard b) {
        return Bitboard{a.raw | b.raw};
    }

    friend Bitboard operator>>(Bitboard a, int shift) {
        return Bitboard{a.raw >> shift};
    }
    friend Bitboard operator<<(Bitboard a, int shift) {
        return Bitboard{a.raw << shift};
    }

    friend Bitboard& operator&=(Bitboard& a, Bitboard b) {
        return a = a & b;
    }
    friend Bitboard& operator|=(Bitboard& a, Bitboard b) {
        return a = a | b;
    }
};

}

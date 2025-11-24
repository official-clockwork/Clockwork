#pragma once

#include <bit>
#include <cassert>

#include "common.hpp"
#include "square.hpp"
#include "util/bit.hpp"
#include "util/types.hpp"

namespace Clockwork {

enum class Direction {
    North,
    NorthEast,
    East,
    SouthEast,
    South,
    SouthWest,
    West,
    NorthWest,
};

struct Bitboard {
public:
    constexpr Bitboard() = default;

    constexpr explicit Bitboard(u64 raw) :
        m_raw(raw) {
    }

    static constexpr Bitboard from_square(Square sq) {
        return Bitboard{static_cast<u64>(1) << sq.raw};
    }

    static constexpr Bitboard squares_of_color(Color c) {
        return c == Color::White ? Bitboard{0x55AA55AA55AA55AA} : Bitboard{0xAA55AA55AA55AA55};
    }

    static constexpr Bitboard file_mask(i32 file) {
        assert(file >= 0 && file <= 7);
        return Bitboard{static_cast<u64>(0x0101010101010101) << file};
    }

    static constexpr Bitboard rank_mask(i32 rank) {
        assert(rank >= 0 && rank <= 7);
        return Bitboard{static_cast<u64>(0xFF) << (8 * rank)};
    }

    static constexpr Bitboard central_files() {
        return file_mask(2) | file_mask(3) | file_mask(4) | file_mask(5);
    }

    [[nodiscard]] static Bitboard fill_verticals(const Bitboard mask) {
        Bitboard result = mask | (mask >> 8);
        result |= result >> 16;
        result |= result >> 32;
        return (result & Bitboard::rank_mask(0)) * Bitboard::file_mask(0);
    }

    [[nodiscard]] bool empty() const {
        return m_raw == 0;
    }

    [[nodiscard]] usize popcount() const {
        return static_cast<usize>(std::popcount(m_raw));
    }

    [[nodiscard]] i32 ipopcount() const {
        return static_cast<i32>(std::popcount(m_raw));
    }

    [[nodiscard]] Square msb() const {
        return Square{static_cast<u8>(std::countl_zero(m_raw))};
    }

    [[nodiscard]] Square lsb() const {
        return Square{static_cast<u8>(std::countr_zero(m_raw))};
    }

    // Rank closest to player
    [[nodiscard]] u8 front_rank(Color color) const {
        i32 color_shift = color == Color::White ? 0 : 56;
        return static_cast<u8>(m_raw >> color_shift);
    }

    [[nodiscard]] Bitboard shift(Direction dir) const {
        constexpr u64 FILE_A = file_mask(0).m_raw;
        constexpr u64 FILE_H = file_mask(7).m_raw;
        switch (dir) {
        case Direction::North:
            return Bitboard{m_raw << 8};
        case Direction::NorthEast:
            return Bitboard{(m_raw & ~FILE_H) << 9};
        case Direction::East:
            return Bitboard{(m_raw & ~FILE_H) << 1};
        case Direction::SouthEast:
            return Bitboard{(m_raw & ~FILE_H) >> 7};
        case Direction::South:
            return Bitboard{m_raw >> 8};
        case Direction::SouthWest:
            return Bitboard{(m_raw & ~FILE_A) >> 9};
        case Direction::West:
            return Bitboard{(m_raw & ~FILE_A) >> 1};
        case Direction::NorthWest:
            return Bitboard{(m_raw & ~FILE_A) << 7};
        }
    }

    [[nodiscard]] Bitboard shift_relative(Color perspective, Direction dir) const {
        if (perspective == Color::Black) {
            dir = static_cast<Direction>((static_cast<u32>(dir) + 4) % 8);
        }
        return shift(dir);
    }

    [[nodiscard]] Bitboard shift_relative(Color perspective, Direction dir, const i32 times) const {
        if (perspective == Color::Black) {
            dir = static_cast<Direction>((static_cast<u32>(dir) + 4) % 8);
        }
        Bitboard result = *this;
        for (i32 i = 0; i < times; i++) {
            result = result.shift(dir);
        }
        return result;
    }

    [[nodiscard]] u64 value() const {
        return m_raw;
    }

    [[nodiscard]] bool is_set(Square sq) const {
        return (m_raw >> sq.raw) & 1;
    }

    void clear(Square sq) {
        m_raw &= ~from_square(sq).m_raw;
    }

    void set(Square sq) {
        m_raw |= from_square(sq).m_raw;
    }

    void set(Square sq, bool value) {
        if (value) {
            set(sq);
        } else {
            clear(sq);
        }
    }

    struct Iterator {
    public:
        Iterator& operator++() {
            m_bb = clear_lowest_bit(m_bb);
            return *this;
        }

        Square operator*() const {
            return Square{static_cast<u8>(std::countr_zero(m_bb))};
        }

        bool operator==(const Iterator&) const = default;

    private:
        friend struct Bitboard;

        explicit constexpr Iterator(u64 bb) :
            m_bb(bb) {
        }

        u64 m_bb;
    };

    [[nodiscard]] Iterator begin() const {
        return Iterator{m_raw};
    }

    [[nodiscard]] Iterator end() const {
        return Iterator{0};
    }

    bool operator==(const Bitboard&) const = default;

    friend constexpr Bitboard operator~(Bitboard a) {
        return Bitboard{~a.m_raw};
    }
    friend constexpr Bitboard operator&(Bitboard a, Bitboard b) {
        return Bitboard{a.m_raw & b.m_raw};
    }
    friend constexpr Bitboard operator|(Bitboard a, Bitboard b) {
        return Bitboard{a.m_raw | b.m_raw};
    }
    friend constexpr Bitboard operator^(Bitboard a, Bitboard b) {
        return Bitboard{a.m_raw ^ b.m_raw};
    }

    friend constexpr Bitboard operator*(Bitboard a, Bitboard b) {
        return Bitboard{a.m_raw * b.m_raw};
    }

    friend constexpr Bitboard operator>>(Bitboard a, i32 shift) {
        return Bitboard{a.m_raw >> shift};
    }
    friend constexpr Bitboard operator<<(Bitboard a, i32 shift) {
        return Bitboard{a.m_raw << shift};
    }

    friend constexpr Bitboard& operator&=(Bitboard& a, Bitboard b) {
        return a = a & b;
    }
    friend constexpr Bitboard& operator|=(Bitboard& a, Bitboard b) {
        return a = a | b;
    }
    friend constexpr Bitboard& operator^=(Bitboard& a, Bitboard b) {
        return a = a ^ b;
    }

private:
    u64 m_raw = 0;
};

}  // namespace Clockwork

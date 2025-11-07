#pragma once

#include "bitboard.hpp"
#include "common.hpp"
#include "square.hpp"
#include "util/types.hpp"
#include <array>
#include <bit>
#include <cstring>
#include <iosfwd>
#include <lps/lps.hpp>

namespace Clockwork {

struct PieceMask;

struct PieceId {
    u8 raw;

    constexpr PieceId(u8 raw) :
        raw(raw) {
        assert(raw < 0x10);
    }

    constexpr static PieceId king() {
        return {0};
    }

    [[nodiscard]] constexpr PieceMask to_piece_mask() const;
};

static_assert(sizeof(PieceId) == sizeof(u8));

struct PieceMask {
public:
    constexpr PieceMask() :
        m_raw(0) {
    }

    constexpr explicit PieceMask(u16 raw) :
        m_raw(raw) {
    }

    constexpr static PieceMask king() {
        // The king always has a PieceId of 0.
        return PieceMask{0x0001};
    }

    [[nodiscard]] bool empty() const {
        return m_raw == 0;
    }

    [[nodiscard]] usize popcount() const {
        return static_cast<usize>(std::popcount(m_raw));
    }

    [[nodiscard]] PieceId msb() const {
        return PieceId{static_cast<u8>(std::countl_zero(m_raw))};
    }

    [[nodiscard]] PieceId lsb() const {
        return PieceId{static_cast<u8>(std::countr_zero(m_raw))};
    }

    [[nodiscard]] u16 value() const {
        return m_raw;
    }

    [[nodiscard]] bool is_set(PieceId id) const {
        return (m_raw >> id.raw) & 1;
    }

    void clear(PieceId id) {
        m_raw &= ~id.to_piece_mask().m_raw;
    }

    void set(PieceId id) {
        m_raw |= id.to_piece_mask().m_raw;
    }

    void set(PieceId id, bool value) {
        if (value) {
            set(id);
        } else {
            clear(id);
        }
    }

    struct Iterator {
    public:
        Iterator& operator++() {
            m_raw = clear_lowest_bit(m_raw);
            return *this;
        }

        PieceId operator*() const {
            return PieceId{static_cast<u8>(std::countr_zero(m_raw))};
        }

        bool operator==(const Iterator&) const = default;

    private:
        friend struct PieceMask;

        explicit constexpr Iterator(u16 raw) :
            m_raw(raw) {
        }

        u16 m_raw;
    };

    [[nodiscard]] Iterator begin() const {
        return Iterator{m_raw};
    }

    [[nodiscard]] Iterator end() const {
        return Iterator{0};
    }

    bool operator==(const PieceMask&) const = default;

    friend constexpr PieceMask operator~(PieceMask a) {
        return PieceMask{static_cast<u16>(~a.m_raw)};
    }
    friend constexpr PieceMask operator&(PieceMask a, PieceMask b) {
        return PieceMask{static_cast<u16>(a.m_raw & b.m_raw)};
    }
    friend constexpr PieceMask operator|(PieceMask a, PieceMask b) {
        return PieceMask{static_cast<u16>(a.m_raw | b.m_raw)};
    }

    friend constexpr PieceMask& operator&=(PieceMask& a, PieceMask b) {
        return a = a & b;
    }
    friend constexpr PieceMask& operator|=(PieceMask& a, PieceMask b) {
        return a = a | b;
    }

private:
    u16 m_raw = 0;
};

[[nodiscard]] constexpr PieceMask PieceId::to_piece_mask() const {
    return PieceMask{static_cast<u16>(1 << raw)};
}

static_assert(sizeof(PieceMask) == sizeof(u16));

struct Place {
    u8 raw = 0;

    static constexpr Place empty() {
        return {};
    }

    static constexpr i32 PTYPE_SHIFT = 5;
    static constexpr u8  PTYPE_MASK  = 0xE0;
    static constexpr i32 COLOR_SHIFT = 4;
    static constexpr u8  COLOR_MASK  = 0x10;
    static constexpr u8  ID_MASK     = 0x0F;

    constexpr Place() = default;

    constexpr Place(Color color, PieceType pt, PieceId id) {
        raw = static_cast<u8>((static_cast<i32>(color) << COLOR_SHIFT)
                              | (static_cast<i32>(pt) << PTYPE_SHIFT) | id.raw);
    }

    [[nodiscard]] constexpr bool is_empty() const {
        return raw == 0;
    }
    [[nodiscard]] constexpr Color color() const {
        return static_cast<Color>((raw & COLOR_MASK) != 0);
    }
    [[nodiscard]] constexpr PieceType ptype() const {
        return static_cast<PieceType>((raw & PTYPE_MASK) >> PTYPE_SHIFT);
    }
    [[nodiscard]] constexpr PieceId id() const {
        return PieceId{static_cast<u8>(raw & ID_MASK)};
    }

    [[nodiscard]] constexpr char to_char() const {
        constexpr std::array<std::string_view, 2> STR{{".PNBRQK", ".pnbrqk"}};
        return STR[static_cast<usize>(color())][static_cast<usize>(ptype())];
    }

    bool operator==(const Place&) const = default;
};

static_assert(sizeof(Place) == sizeof(u8));

struct Byteboard {
    std::array<Place, 64> mailbox;

    [[nodiscard]] u8x64 to_vector() const {
        return std::bit_cast<u8x64>(mailbox);
    }

    [[nodiscard]] Bitboard get_empty_bitboard() const {
        return Bitboard{to_vector().zeros().to_bits()};
    }

    [[nodiscard]] Bitboard get_occupied_bitboard() const {
        return Bitboard{to_vector().nonzeros().to_bits()};
    }

    [[nodiscard]] Bitboard get_color_bitboard(Color color) const {
        u64  color_bb = static_cast<u64>(0) - static_cast<u64>(color);
        auto vec      = to_vector();
        return Bitboard{~vec.test(u8x64::splat(0x10)).to_bits() ^ color_bb}
             & get_occupied_bitboard();
    }

    [[nodiscard]] Bitboard bitboard_for(Color color, PieceType ptype) const {
        Place p{color, ptype, PieceId{0}};
        return Bitboard{(to_vector() & u8x64::splat(0xF0)).eq(u8x64::splat(p.raw)).to_bits()};
    }

    [[nodiscard]] Bitboard bitboard_for(PieceType ptype) const {
        Place p{Color::White, ptype, PieceId{0}};
        return Bitboard{
          (to_vector() & u8x64::splat(Place::PTYPE_MASK)).eq(u8x64::splat(p.raw)).to_bits()};
    }

    constexpr Place& operator[](Square sq) {
        return mailbox[sq.raw];
    }

    constexpr Place operator[](Square sq) const {
        return mailbox[sq.raw];
    }

    bool operator==(const Byteboard& other) const = default;
};

static_assert(sizeof(Byteboard) == 64);

struct Wordboard {
    u16x64 raw = u16x64::zero();

    [[nodiscard]] std::array<PieceMask, 64> to_mailbox() const {
        return std::bit_cast<std::array<PieceMask, 64>>(raw);
    }

    [[nodiscard]] Bitboard get_attacked_bitboard() const {
        return Bitboard{raw.nonzeros().to_bits()};
    }

    [[nodiscard]] Bitboard get_piece_mask_bitboard(PieceMask piece_mask) const {
        u16x64 pm = u16x64::splat(piece_mask.value());
        return Bitboard{raw.test(pm).to_bits()};
    }

    [[nodiscard]] usize count_matching_mask(PieceMask piece_mask) const {
        u16x64 pm = u16x64::splat(piece_mask.value());
        return (raw & pm).nonzeros_count();
    }

    [[nodiscard]] PieceMask read(Square sq) const {
        PieceMask value;
        std::memcpy(&value, reinterpret_cast<const char*>(&raw) + sq.raw * sizeof(PieceMask),
                    sizeof(PieceMask));
        return value;
    }

    [[nodiscard]] i32 popcount() const {
        i32 count = 0;
        for (u64 x : std::bit_cast<std::array<u64, 16>>(*this)) {
            count += std::popcount(x);
        }
        return count;
    }

    friend inline Wordboard operator&(const Wordboard& a, const Wordboard& b) {
        return Wordboard{a.raw & b.raw};
    }

    bool                 operator==(const Wordboard& other) const = default;
    friend std::ostream& operator<<(std::ostream& os, const Wordboard& at);
};

static_assert(sizeof(Wordboard) == 128);

}  // namespace Clockwork

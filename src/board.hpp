#pragma once

#include <array>
#include <bit>
#include <iosfwd>

#include "common.hpp"
#include "square.hpp"
#include "util/types.hpp"
#include "util/vec.hpp"


namespace Clockwork {

struct PieceId {
    u8 raw;

    constexpr PieceId(u8 raw) :
        raw(raw) {
        assert(raw < 0x10);
    }

    [[nodiscard]] constexpr u16 to_piece_mask() const {
        return static_cast<u16>(1 << raw);
    }
};

static_assert(sizeof(PieceId) == sizeof(u8));

struct Place {
    u8 raw = 0;

    static constexpr Place empty() {
        return {};
    }

    constexpr Place() = default;

    constexpr Place(Color color, PieceType pt, PieceId id) {
        raw =
          static_cast<u8>((static_cast<int>(color) << 7) | (static_cast<int>(pt) << 4) | id.raw);
    }

    [[nodiscard]] constexpr bool is_empty() const {
        return raw == 0;
    }
    [[nodiscard]] constexpr Color color() const {
        return static_cast<Color>((raw & 0x80) != 0);
    }
    [[nodiscard]] constexpr PieceType ptype() const {
        return static_cast<PieceType>((raw >> 4) & 0x7);
    }
    [[nodiscard]] constexpr PieceId id() const {
        return PieceId{static_cast<u8>(raw & 0xF)};
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

    [[nodiscard]] v512 to_vec() const {
        return std::bit_cast<v512>(mailbox);
    }

    [[nodiscard]] u64 get_empty_bitboard() const {
        return to_vec().zero8();
    }

    [[nodiscard]] u64 get_color_bitboard(Color color) const {
        auto vec = to_vec();
        return ~(vec.msb8() ^ static_cast<u64>(-static_cast<i64>(color))) & vec.nonzero8();
    }

    [[nodiscard]] u64 bitboard_for(Color color, PieceType ptype) const {
        Place p{color, ptype, PieceId{0}};
        return v512::eq8(to_vec() & v512::broadcast8(0xF0), v512::broadcast8(p.raw));
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
    std::array<u16, 64> mailbox;

    [[nodiscard]] std::array<v512, 2> to_vec() const {
        return std::bit_cast<std::array<v512, 2>>(mailbox);
    }

    [[nodiscard]] u64 get_attacked_bitboard() const {
        const auto VEC = to_vec();
        return concat64(VEC[0].nonzero16(), VEC[1].nonzero16());
    }

    constexpr u16& operator[](Square sq) {
        return mailbox[sq.raw];
    }

    constexpr u16 operator[](Square sq) const {
        return mailbox[sq.raw];
    }

    bool                 operator==(const Wordboard& other) const = default;
    friend std::ostream& operator<<(std::ostream& os, const Wordboard& at);
};

static_assert(sizeof(Wordboard) == 128);

}

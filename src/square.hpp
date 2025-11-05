#pragma once

#include "util/types.hpp"
#include <cassert>
#include <compare>
#include <optional>
#include <ostream>
#include <string_view>
#include <tuple>

namespace Clockwork {

struct Square {
    u8 raw = 0x80;

    static constexpr Square invalid() {
        return {0x80};
    }

    static constexpr Square from_file_and_rank(i32 file, i32 rank) {
        assert(file >= 0 && file < 8);
        assert(rank >= 0 && rank < 8);
        return Square{static_cast<u8>(rank * 8 + file)};
    }

    static constexpr std::optional<Square> parse(std::string_view str) {
        if (str.size() != 2) {
            return std::nullopt;
        }
        if (str[0] < 'a' or str[0] > 'h') {
            return std::nullopt;
        }
        i32 file = str[0] - 'a';
        if (str[1] < '1' or str[1] > '8') {
            return std::nullopt;
        }
        i32 rank = str[1] - '1';
        return from_file_and_rank(file, rank);
    }

    [[nodiscard]] constexpr i32 file() const {
        return raw % 8;
    }

    [[nodiscard]] constexpr i32 rank() const {
        return raw / 8;
    }

    [[nodiscard]] constexpr Color color() const {
        return ((file() + rank()) & 1) ? Color::White : Color::Black;
    }

    [[nodiscard]] constexpr Square relative_sq(Color c) const {
        return c == Color::White ? *this : flip_vertical();
    }

    [[nodiscard]] constexpr std::tuple<i32, i32> to_file_and_rank() const {
        return {file(), rank()};
    }

    [[nodiscard]] constexpr bool is_valid() const {
        return (raw & 0x80) == 0;
    }

    constexpr Square flip_horizontal() const {
        return Square{static_cast<u8>(raw ^ 7)};
    }

    constexpr Square flip_vertical() const {
        return Square{static_cast<u8>(raw ^ 56)};
    }

    template<Color color>
    constexpr Square push() const {
        return Square{static_cast<u8>(raw + (color == Color::White ? 8 : -8))};
    }

    friend std::ostream& operator<<(std::ostream& os, Square sq) {
        char file = static_cast<char>('a' + sq.file());
        return os << file << sq.rank() + 1;
    }

    const std::strong_ordering operator<=>(const Square&) const = default;
};
}  // namespace Clockwork

#pragma once

#include <cassert>
#include <compare>
#include <optional>
#include <ostream>
#include <string_view>

#include "util/types.hpp"

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

    [[nodiscard]] constexpr bool is_valid() const {
        return (raw & 0x80) == 0;
    }

    friend std::ostream& operator<<(std::ostream& os, Square sq) {
        char file = static_cast<char>('a' + sq.file());
        return os << file << sq.rank() + 1;
    }

    const std::strong_ordering operator<=>(const Square&) const = default;
};
}

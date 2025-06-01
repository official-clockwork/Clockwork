#pragma once

#include "util/types.hpp"

#include <string_view>
#include <cassert>
#include <ostream>
#include <optional>

namespace Clockwork {

struct Square {
    u8 raw;

    static constexpr Square fromFileAndRank(int file, int rank) {
        assert(file >= 0 && file < 8);
        assert(rank >= 0 && rank < 8);
        return Square{static_cast<u8>(rank * 8 + file)};
    }

    static constexpr std::optional<Square> parse(std::string_view str) {
        if (str.size() != 2)
            return std::nullopt;
        if (str[0] < 'a' or str[0] > 'h')
            return std::nullopt;
        const int file = str[0] - 'a';
        if (str[1] < '1' or str[1] > '8')
            return std::nullopt;
        const int rank = str[1] - '1';
        return fromFileAndRank(file, rank);
    }

    constexpr usize file() const { return raw % 8; }

    constexpr usize rank() const { return raw / 8; }

    friend std::ostream& operator<<(std::ostream& os, Square sq) {
        char file = static_cast<char>('a' + sq.file());
        return os << file << sq.rank() + 1;
    }
};

}
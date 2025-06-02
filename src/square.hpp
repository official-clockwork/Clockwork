#pragma once

#include "util/types.hpp"

#include <string_view>
#include <cassert>
#include <ostream>
#include <optional>

namespace Clockwork {

struct Square {
    u8 raw;

    constexpr Square(u8 r) :
        raw{r} {
    }

    static constexpr Square from_file_and_rank(int file, int rank) {
        assert(file >= 0 && file < 8);
        assert(rank >= 0 && rank < 8);
        return Square{static_cast<u8>(rank * 8 + file)};
    }

    static constexpr std::optional<Square> parse(std::string_view str) {
        if (str.size() != 2)
            return std::nullopt;
        if (str[0] < 'a' or str[0] > 'h')
            return std::nullopt;
        const int FILE = str[0] - 'a';
        if (str[1] < '1' or str[1] > '8')
            return std::nullopt;
        const int RANK = str[1] - '1';
        return from_file_and_rank(FILE, RANK);
    }

    [[nodiscard]] constexpr usize file() const {
        return raw % 8;
    }

    [[nodiscard]] constexpr usize rank() const {
        return raw / 8;
    }

    friend std::ostream& operator<<(std::ostream& os, Square sq) {
        char file = static_cast<char>('a' + sq.file());
        return os << file << sq.rank() + 1;
    }
};

}

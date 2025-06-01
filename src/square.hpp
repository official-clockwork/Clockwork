#pragma once

#include "util/types.hpp"

#include <string_view>
#include <cassert>
#include <ostream>

namespace Clockwork {

struct Square {
    u8 raw;

    constexpr explicit Square(u8 value ) : raw(value) {}

    constexpr explicit Square(std::string_view sv) {
        // TODO: Better error handling?
        assert(sv.size() == 2);
        char file = sv[0];
        assert(file >= 'a' && file <= 'h');
        char rank = sv[1];
        assert(rank >= '1' && rank <= '8');
        raw = 8 * (file - 'a') + rank - '1';
    }

    constexpr usize file() const { return raw / 8; }

    constexpr usize rank() const { return raw % 8; }

    friend std::ostream& operator<<(std::ostream& os, Square sq) {
        char file = 'a' + sq.file();
        return os << file << sq.rank() + 1;
    }

};

}
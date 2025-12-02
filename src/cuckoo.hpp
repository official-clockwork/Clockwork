#pragma once

#include "move.hpp"
#include "util/types.hpp"
#include <array>
#include <optional>

namespace Clockwork {

class Cuckoo {
public:
    Cuckoo() = delete;

    static constexpr usize TABLE_SIZE = 8192;

    // zobrist keys for the repetition detection table.
    static std::array<HashKey, TABLE_SIZE> keys;
    // moves for the repetition detection table.
    static std::array<Move, TABLE_SIZE> moves;

    // first hash function
    [[nodiscard]] static constexpr usize h1(HashKey key) {
        return static_cast<usize>(key & 0x1FFF);
    }

    // second hash function
    [[nodiscard]] static constexpr usize h2(HashKey key) {
        return static_cast<usize>((key >> 16) & 0x1FFF);
    }

    // initialize the cuckoo hash tables
    static void init();
};

}  // namespace Clockwork

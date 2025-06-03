#include <iostream>

#include "movegen.hpp"
#include "position.hpp"
#include "util/types.hpp"

namespace Clockwork {

template<bool print>
static u64 core(const Position& position, usize depth) {
    if (depth == 0)
        return 1;

    u64 result = 0;

    MoveList moves;
    MoveGen  movegen{position};
    movegen.generate_moves(moves);

    for (Move m : moves) {
        Position new_position = position.move(m);
        if (!new_position.is_valid())
            continue;

        u64 child = core<false>(new_position, depth - 1);

        if constexpr (print)
            std::cout << m << ": " << child << std::endl;

        result += child;
    }

    return result;
}

u64 perft(const Position& position, usize depth) {
    return core<false>(position, depth);
}

void split_perft(const Position& position, usize depth) {
    u64 total = core<true>(position, depth);
    std::cout << "total: " << total << std::endl;
}

}

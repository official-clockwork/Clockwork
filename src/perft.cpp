#include <chrono>
#include <iomanip>
#include <iostream>

#include "movegen.hpp"
#include "position.hpp"
#include "util/types.hpp"

namespace Clockwork {

template<bool print>
static u64 core(const Position& position, usize depth) {
    if (depth == 0) {
        return 1;
    }

    u64 result = 0;

    MoveList moves;
    MoveGen  movegen{position};
    movegen.generate_moves(moves);

    for (Move m : moves) {
        Position new_position = position.move(m);
        if (!new_position.is_valid()) {
            continue;
        }

        u64 child = core<false>(new_position, depth - 1);

        if constexpr (print) {
            std::cout << m << ": " << child << std::endl;
        }

        result += child;
    }

    return result;
}

u64 perft(const Position& position, usize depth) {
    return core<false>(position, depth);
}

void split_perft(const Position& position, usize depth) {
    auto start_time = std::chrono::steady_clock::now();

    u64 total = core<true>(position, depth);
    std::cout << "total: " << total << std::endl;

    std::chrono::duration<f64> elapsed    = std::chrono::steady_clock::now() - start_time;
    f64                        elapsed_ms = elapsed.count() * 1000.0;
    f64                        mnps       = static_cast<f64>(total) / elapsed.count() / 1000000.0;

    std::ios state(nullptr);
    state.copyfmt(std::cout);

    std::cout << std::setprecision(1) << std::fixed;
    std::cout << "perft to depth " << depth << " complete in " << elapsed_ms << "ms (" << mnps
              << " Mnps)" << std::endl;

    std::cout.copyfmt(state);
}

}

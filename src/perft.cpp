#include "movegen.hpp"
#include "position.hpp"
#include "util/ios_fmt_guard.hpp"
#include "util/types.hpp"
#include <chrono>
#include <iostream>

namespace Clockwork {

template<bool print>
static u64 core(const Position& position, usize depth) {
    if (depth == 0) {
        return 1;
    }

    u64 result = 0;

    MoveList moves;
    MoveGen  movegen{position};
    movegen.generate_moves(moves, moves);

    if (!print && depth == 1) {
        return moves.size();
    }

    for (Move m : moves) {
        assert(movegen.is_legal(m));

        Position new_position = position.move(m);

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
    IosFmtGuard guard{std::cout};

    auto start_time = std::chrono::steady_clock::now();

    u64 total = core<true>(position, depth);
    std::cout << "total: " << total << std::endl;

    std::chrono::duration<f64> elapsed    = std::chrono::steady_clock::now() - start_time;
    f64                        elapsed_ms = elapsed.count() * 1000.0;
    f64                        mnps       = static_cast<f64>(total) / elapsed.count() / 1000000.0;

    std::cout << std::setprecision(1) << std::fixed;
    std::cout << "perft to depth " << depth << " complete in " << elapsed_ms << "ms (" << mnps
              << " Mnps)" << std::endl;
}

}  // namespace Clockwork

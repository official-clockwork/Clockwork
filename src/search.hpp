#pragma once
#include "move.hpp"
#include "position.hpp"
#include "uci.hpp"
#include "util/types.hpp"

namespace Clockwork {

namespace Search {
struct Stack {
    Move* pv;
    i32   ply;
};

class Worker {
public:
    Worker() = default;
    void launch_search(Position root_position, UCI::SearchSettings settings);

private:
    Move iterative_deepening(Position root_position, UCI::SearchSettings settings);

    Value search(Position& pos, Stack* ss, Value alpha, Value beta, Depth depth);
    Value evaluate(const Position& pos);
};
}
}

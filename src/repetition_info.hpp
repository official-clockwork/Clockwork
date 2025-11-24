#pragma once

#include "common.hpp"
#include "util/types.hpp"
#include <utility>
#include <vector>

namespace Clockwork {

struct Position;

class RepetitionInfo {
public:
    RepetitionInfo() = default;

    void push(HashKey key, bool is_reversible);
    void pop();
    void reset();

    bool detect_repetition(usize root_ply);
    bool has_game_cycle(const Position& pos, usize ply);

private:
    std::vector<std::pair<HashKey, bool>> m_repetition_table;
};

}  // namespace Clockwork

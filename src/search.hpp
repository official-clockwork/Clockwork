#pragma once
#include "move.hpp"
#include "position.hpp"
#include "uci.hpp"
#include "util/types.hpp"

namespace Clockwork {

namespace Search {
struct Stack {
    Move* pv;
};

struct SearchLimits {
    time::TimePoint hard_time_limit;
    u64             soft_node_limit;
    u64             hard_node_limit;
    Depth           depth_limit;
};

class Worker {
public:
    u64 search_nodes;
    Worker() = default;
    void launch_search(Position            root_position,
                       RepetitionInfo      repetition_info,
                       UCI::SearchSettings settings);

private:
    bool            m_stopped;
    time::TimePoint m_search_start;
    SearchLimits    m_search_limits;
    RepetitionInfo  m_repetition_info;
    Move            iterative_deepening(Position root_position);

    Value search(Position& pos, Stack* ss, Value alpha, Value beta, Depth depth, i32 ply);
    Value quiesce(Position& pos, Stack* ss, Value alpha, Value beta, i32 ply);
    Value evaluate(const Position& pos);
    void  check_tm_hard_limit();
};
}
}

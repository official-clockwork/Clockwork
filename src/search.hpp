#pragma once
#include "history.hpp"
#include "move.hpp"
#include "position.hpp"
#include "tt.hpp"
#include "uci.hpp"
#include "util/types.hpp"


namespace Clockwork {

namespace Search {
struct Stack {
    Move* pv;
    Move  killer;
};

struct SearchLimits {
    time::TimePoint hard_time_limit;
    u64             soft_node_limit;
    u64             hard_node_limit;
    Depth           depth_limit;
};

struct ThreadData {
    History history;
};

class Worker {
public:
    u64 search_nodes;
    Worker(TT& tt, ThreadData& td);
    void launch_search(Position            root_position,
                       RepetitionInfo      repetition_info,
                       UCI::SearchSettings settings);

private:
    bool            m_stopped;
    time::TimePoint m_search_start;
    SearchLimits    m_search_limits;
    RepetitionInfo  m_repetition_info;
    TT&             m_tt;
    ThreadData&     m_td;

    Move iterative_deepening(Position root_position);

    template<bool PV_NODE>
    Value search(Position& pos, Stack* ss, Value alpha, Value beta, Depth depth, i32 ply);
    Value quiesce(Position& pos, Stack* ss, Value alpha, Value beta, i32 ply);
    Value evaluate(const Position& pos);
    bool  check_tm_hard_limit();
};
}
}

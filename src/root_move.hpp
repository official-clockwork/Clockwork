#pragma once

#include "move.hpp"
#include "util/static_vector.hpp"
#include "util/types.hpp"

namespace Clockwork::Search {

enum class WDL {
    None,
    Win,
    Draw,
    Loss,
};

struct PV {
public:
    void clear() {
        m_pv.clear();
    }

    void set(Move move) {
        m_pv.clear();
        m_pv.push_back(move);
    }

    void set(Move move, const PV& child_pv_line) {
        m_pv.clear();
        m_pv.push_back(move);
        m_pv.append(child_pv_line.m_pv);
    }

    Move first_move() const {
        return m_pv.empty() ? Move::none() : m_pv[0];
    }

    friend std::ostream& operator<<(std::ostream& os, const PV& pv);

private:
    StaticVector<Move, MAX_PLY + 1> m_pv;
};

struct RootMove {
    explicit RootMove(Move move) {
        pv.set(move);
    }

    Value score          = -VALUE_INF;
    Value window_score   = -VALUE_INF;
    Value previous_score = -VALUE_INF;
    Value display_score  = -VALUE_INF;

    bool upperbound = false;
    bool lowerbound = false;

    WDL tb_wdl  = WDL::None;
    i32 tb_rank = 0;

    Value tb_min_score = -VALUE_INF;
    Value tb_max_score = VALUE_INF;

    PV pv;

    Depth searched_depth = 1;
    Depth seldepth       = 0;

    void set_tb_status(WDL wdl, i32 rank) {
        tb_wdl  = wdl;
        tb_rank = rank;

        switch (wdl) {
        case WDL::Win:
            tb_min_score = VALUE_TB_WIN;
            break;
        case WDL::Draw:
            tb_min_score = 0;
            tb_max_score = 0;
            break;
        case WDL::Loss:
            tb_max_score = -VALUE_TB_WIN;
            break;
        default:
            break;
        }
    }
};

}

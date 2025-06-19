#pragma once
#include "history.hpp"
#include "move.hpp"
#include "position.hpp"
#include "repetition_info.hpp"
#include "tt.hpp"
#include "util/types.hpp"
#include <mutex>
#include <condition_variable>


namespace Clockwork {


namespace Search {

struct SearchSettings {
    Color stm        = Color::White;
    i32   depth      = 0;
    i64   w_time     = -1;
    i64   b_time     = -1;
    i64   w_inc      = -1;
    i64   b_inc      = -1;
    i64   move_time  = -1;
    u64   hard_nodes = 0;
    u64   soft_nodes = 0;
};

// Forward declare for Searcher
class Worker;

enum ThreadType {
    MAIN      = 1,
    SECONDARY = 0
};

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

class Searcher {
public:
    SearchLimits            search_limits;
    RepetitionInfo          repetition_info;
    SearchSettings          settings;
    Position                root_position;

    Searcher();
    void launch_search(Position            root_position,
                       RepetitionInfo      repetition_info,
                       SearchSettings settings);
    void stop_searching();
    void wait_for_search_finished();
    void wait_for_workers_finished();
    void initialize(int thread_count);
    void exit();

    void reset();
    u64 node_count();

private:
    TT                                       m_tt;
    std::vector<std::unique_ptr<Worker>>     m_workers;

};

class Worker {
public:
    u64                     search_nodes;
    Worker(TT& tt, Searcher *searcher, ThreadType thread_type);
    void launch_search(Position            root_position,
                       RepetitionInfo      repetition_info,
                       SearchSettings settings);
    void exit();
    void idle();
    void wait_for_search_finished();
    void start_searching();

    void set_stopped(bool stopped) {
        m_stopped = stopped;
    }
    void set_searching(bool searching) {
        m_searching = searching;
    }
    void set_exiting(bool exiting) {
        m_exiting = exiting;
    }
    void reset_thread_data() {
        m_td = {};
    }

    [[nodiscard]] std::mutex& get_mutex() {
        return m_mutex;
    };
    [[nodiscard]] std::condition_variable& get_cv() {
        return m_cv;
    }
    [[nodiscard]] ThreadType thread_type() {
        return m_thread_type;
    }
    [[nodiscard]] bool is_main() {
        return m_thread_type == ThreadType::MAIN;
    }

private:
    time::TimePoint         m_search_start;
    TT&                     m_tt;
    Searcher*               m_searcher;
    std::thread             m_thread;
    ThreadType              m_thread_type;
    SearchLimits            m_search_limits;
    RepetitionInfo          m_repetition_info;
    ThreadData              m_td;
    std::mutex              m_mutex;
    std::condition_variable m_cv;
    bool                    m_searching;
    bool                    m_stopped;
    bool                    m_exiting;
    
    Move iterative_deepening(Position root_position);

    template<bool PV_NODE>
    Value search(Position& pos, Stack* ss, Value alpha, Value beta, Depth depth, i32 ply);
    Value quiesce(Position& pos, Stack* ss, Value alpha, Value beta, i32 ply);
    Value evaluate(const Position& pos);
    bool  check_tm_hard_limit();
};
}
}
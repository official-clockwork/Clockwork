#pragma once
#include "history.hpp"
#include "move.hpp"
#include "position.hpp"
#include "repetition_info.hpp"
#include "tt.hpp"
#include "util/types.hpp"
#include <barrier>
#include <memory>
#include <shared_mutex>
#include <thread>

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
class alignas(128) Worker;

enum class ThreadType {
    MAIN      = 1,
    SECONDARY = 0,
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
    SearchLimits   search_limits;
    SearchSettings settings;
    TT             tt;

    // We use a shared_mutex to ensure proper mutual thread exclusion.and avoid races.
    // The UCI thread only ever obtains exclusive access (using std::unique_lock);
    // search threads only ever obtain shared access (using std::shared_lock).
    // This ensures that the two classes of thread never step on each other.
    std::shared_mutex mutex;

    using BarrierPtr = std::unique_ptr<std::barrier<>>;
    BarrierPtr idle_barrier;
    BarrierPtr started_barrier;

    Searcher();
    ~Searcher();
    void set_position(const Position& root_position, const RepetitionInfo& repetition_info);
    void launch_search(SearchSettings settings);
    void stop_searching();
    void wait();
    void initialize(int thread_count);
    void exit();

    u64  node_count();
    void reset();
    void resize_tt(size_t mb) {
        tt.resize(mb);
    }

private:
    std::vector<std::unique_ptr<Worker>> m_workers;
};

class alignas(128) Worker {
public:
    Position       root_position;
    RepetitionInfo repetition_info;

    Worker(Searcher& searcher, ThreadType thread_type);
    ~Worker();

    void exit();

    void prepare();
    void start_searching();

    void set_stopped() {
        m_stopped = true;
    }
    void reset_thread_data() {
        m_td = {};
    }

    [[nodiscard]] ThreadType thread_type() const {
        return m_thread_type;
    }
    [[nodiscard]] u64 search_nodes() const {
        return m_search_nodes.load(std::memory_order_relaxed);
    }

private:
    void thread_main();

    void increment_search_nodes() {
        m_search_nodes.fetch_add(1, std::memory_order_relaxed);
    }

    std::atomic<u64>  m_search_nodes;
    time::TimePoint   m_search_start;
    Searcher&         m_searcher;
    std::thread       m_thread;
    ThreadType        m_thread_type;
    SearchLimits      m_search_limits;
    ThreadData        m_td;
    std::atomic<bool> m_stopped;
    std::atomic<bool> m_exiting;

    template<bool IS_MAIN>
    Move iterative_deepening(const Position& root_position);
    template<bool IS_MAIN, bool PV_NODE>
    Value search(const Position& pos, Stack* ss, Value alpha, Value beta, Depth depth, i32 ply);
    template<bool IS_MAIN>
    Value quiesce(const Position& pos, Stack* ss, Value alpha, Value beta, i32 ply);
    Value evaluate(const Position& pos);
    bool  check_tm_hard_limit();
};

}
}

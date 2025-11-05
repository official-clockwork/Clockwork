#pragma once

#include "history.hpp"
#include "move.hpp"
#include "position.hpp"
#include "psqt_state.hpp"
#include "repetition_info.hpp"
#include "tt.hpp"
#include "util/static_vector.hpp"
#include "util/types.hpp"
#include <barrier>
#include <iosfwd>
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

struct PV {
public:
    void clear() {
        m_pv.clear();
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

struct Stack {
    Value          static_eval = 0;
    Move           killer      = Move::none();
    Move           excluded_move;
    ContHistEntry* cont_hist_entry = nullptr;
    i32            fail_high_count = 0;
    PV             pv;
};

struct SearchLimits {
    time::TimePoint hard_time_limit;
    time::TimePoint soft_time_limit;
    u64             soft_node_limit;
    u64             hard_node_limit;
    Depth           depth_limit;
};

struct ThreadData {
    History                history;
    std::vector<PsqtState> psqt_states;

    PsqtState& push_psqt_state() {
        psqt_states.push_back(psqt_states.back());
        return psqt_states.back();
    }

    void pop_psqt_state() {
        psqt_states.pop_back();
    }
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
    void initialize(size_t thread_count);
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

    [[nodiscard]] Value get_draw_score() const {
        return (search_nodes() & 3) - 2;  // Randomize between -2 and +2
    }

private:
    void thread_main();

    void increment_search_nodes() {
        m_search_nodes.fetch_add(1, std::memory_order_relaxed);
    }

    std::atomic<u64>         m_search_nodes;
    time::TimePoint          m_search_start;
    time::TimePoint          m_last_info_time;
    Searcher&                m_searcher;
    std::thread              m_thread;
    ThreadType               m_thread_type;
    SearchLimits             m_search_limits;
    ThreadData               m_td;
    std::atomic<bool>        m_stopped;
    std::atomic<bool>        m_exiting;
    std::array<u64, 64 * 64> m_node_counts;
    Depth                    m_seldepth;
    bool                     m_in_nmp_verification = false;

    template<bool IS_MAIN>
    Move iterative_deepening(const Position& root_position);
    template<bool IS_MAIN, bool PV_NODE>
    Value search(
      const Position& pos, Stack* ss, Value alpha, Value beta, Depth depth, i32 ply, bool cutnode);
    template<bool IS_MAIN>
    Value quiesce(const Position& pos, Stack* ss, Value alpha, Value beta, i32 ply);
    Value evaluate(const Position& pos);
    bool  check_tm_hard_limit();
};

}  // namespace Search
}  // namespace Clockwork

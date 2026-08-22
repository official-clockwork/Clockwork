#pragma once

#include "history.hpp"
#include "move.hpp"
#include "position.hpp"
#include "psqt_state.hpp"
#include "repetition_info.hpp"
#include "root_move.hpp"
#include "tt.hpp"
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
    usize multipv    = 1;
    bool  silent     = false;
    bool  datagen    = false;
    bool  tb_enabled = false;
};

// Forward declare for Searcher
class alignas(128) Worker;

enum class ThreadType {
    MAIN      = 1,
    SECONDARY = 0,
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

    std::vector<RootMove> root_moves;

    PsqtState& push_psqt_state() {
        psqt_states.push_back(psqt_states.back());
        return psqt_states.back();
    }

    void pop_psqt_state() {
        psqt_states.pop_back();
    }

    RootMove& pv_move() {
        return root_moves[0];
    }

    const RootMove& pv_move() const {
        return root_moves[0];
    }

    Value root_score() const {
        return pv_move().score;
    }
};

class Searcher {
public:
    SearchLimits   search_limits;
    SearchSettings settings;
    TT             tt;

    // Root moves are duplicated here to avoid probing DTZ tables once for every thread,
    // which is costly with many threads and DTZ tables on an HDD (TCEC).
    std::vector<RootMove> root_moves;
    usize                 multipv;
    bool                  tb_root   = false;
    bool                  probe_wdl = false;

    // We use a shared_mutex to ensure proper mutual thread exclusion.and avoid races.
    // The UCI thread only ever obtains exclusive access (using std::unique_lock);
    // search threads only ever obtain shared access (using std::shared_lock).
    // This ensures that the two classes of thread never step on each other.
    std::shared_mutex mutex;

    using BarrierPtr           = std::unique_ptr<std::barrier<>>;
    BarrierPtr idle_barrier    = nullptr;
    BarrierPtr started_barrier = nullptr;

    Searcher();
    ~Searcher();
    void  set_position(const Position& root_position, const RepetitionInfo& repetition_info);
    void  launch_search(SearchSettings settings);
    void  stop_searching();
    void  wait();
    Value wait_for_score();
    void  initialize(size_t thread_count);
    void  exit();

    u64  node_count();
    u64  tb_hit_count();
    void reset();
    void resize_tt(size_t mb) {
        tt.resize(mb, m_workers.size());
    }

private:
    void init_root_moves(const Position& root_position, RepetitionInfo& repetition_info);

    std::vector<unique_ptr_huge_page<Worker>> m_workers;
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
    [[nodiscard]] u64 tb_hits() const {
        return m_tb_hits.load(std::memory_order_relaxed);
    }

    [[nodiscard]] const ThreadData& get_thread_data() const {
        return m_td;
    }

    [[nodiscard]] Value get_draw_score() const {
        return (search_nodes() & 3) - 2;  // Randomize between -2 and +2
    }

private:
    void thread_main();

    void increment_search_nodes() {
        m_search_nodes.fetch_add(1, std::memory_order_relaxed);
    }

    void increment_tb_hits() {
        m_tb_hits.fetch_add(1, std::memory_order_relaxed);
    }

    std::atomic<u64>         m_search_nodes;
    std::atomic<u64>         m_tb_hits;
    time::TimePoint          m_search_start;
    time::TimePoint          m_last_info_time;
    Searcher&                m_searcher;
    std::thread              m_thread;
    ThreadType               m_thread_type;
    SearchLimits             m_search_limits;
    ThreadData               m_td;
    usize                    m_pv_idx;
    usize                    m_pv_start;
    usize                    m_pv_end;
    std::atomic<bool>        m_stopped;
    std::atomic<bool>        m_exiting;
    std::array<u64, 64 * 64> m_node_counts;
    Depth                    m_root_depth;
    Depth                    m_seldepth;
    bool                     m_in_nmp_verification = false;

    template<bool IS_MAIN>
    Move iterative_deepening(const Position& root_position);
    template<bool IS_MAIN, bool PV_NODE>
    Value search(
      const Position& pos, Stack* ss, Value alpha, Value beta, Depth depth, i32 ply, bool cutnode);
    template<bool IS_MAIN, bool PV_NODE>
    Value quiesce(const Position& pos, Stack* ss, Value alpha, Value beta, i32 ply);
    Value evaluate(const Position& pos);
    Value adj_shuffle(const Position& pos, Value value);
    bool  check_tm_hard_limit();

    void print_info_lines();
    void print_info_line(usize pv_idx);

    RootMove& get_root_move(Move move) {
        for (auto& root_move : m_td.root_moves) {
            if (root_move.pv.first_move() == move) {
                return root_move;
            }
        }

        assert(false && "Failed to find root move");
        std::terminate();
    }

    bool is_legal_root_move(Move move) const {
        for (usize i = m_pv_idx; i < m_pv_end; ++i) {
            const auto& root_move = m_td.root_moves[i];
            if (root_move.pv.first_move() == move) {
                return true;
            }
        }

        return false;
    }
};

}  // namespace Search
}  // namespace Clockwork

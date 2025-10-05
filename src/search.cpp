#include "search.hpp"
#include "board.hpp"
#include "common.hpp"
#include "evaluation.hpp"
#include "history.hpp"
#include "movegen.hpp"
#include "movepick.hpp"
#include "see.hpp"
#include "tm.hpp"
#include "tuned.hpp"
#include "uci.hpp"
#include "util/types.hpp"
#include <algorithm>
#include <array>
#include <atomic>
#include <cmath>
#include <iostream>
#include <limits>
#include <mutex>
#include <numeric>

namespace Clockwork {
namespace Search {
static Value mated_in(i32 ply) {
    return -VALUE_MATED + ply;
}

std::ostream& operator<<(std::ostream& os, const PV& pv) {
    for (Move m : pv.m_pv) {
        os << m << ' ';
    }
    return os;
}

Searcher::Searcher() :
    idle_barrier(std::make_unique<std::barrier<>>(1)),
    started_barrier(std::make_unique<std::barrier<>>(1)) {
}

Searcher::~Searcher() {
    exit();
}

void Searcher::set_position(const Position& root_position, const RepetitionInfo& repetition_info) {
    std::unique_lock lock_guard{mutex};

    for (auto& worker : m_workers) {
        worker->root_position   = root_position;
        worker->repetition_info = repetition_info;
    }
}

void Searcher::launch_search(SearchSettings settings_) {
    {
        std::unique_lock lock_guard{mutex};

        settings = settings_;

        for (auto& worker : m_workers) {
            worker->prepare();
        }
    }
    idle_barrier->arrive_and_wait();
    started_barrier->arrive_and_wait();
}

void Searcher::stop_searching() {
    for (auto& worker : m_workers) {
        worker->set_stopped();
    }
}

void Searcher::wait() {
    // Wait for ability to acquire exclusive access to mutex.
    std::unique_lock lock_guard{mutex};
}

void Searcher::initialize(size_t thread_count) {
    if (m_workers.size() == thread_count) {
        return;
    }
    {
        std::unique_lock lock_guard{mutex};
        for (auto& worker : m_workers) {
            worker->exit();
        }
        idle_barrier->arrive_and_wait();
        m_workers.clear();
    }

    idle_barrier    = std::make_unique<std::barrier<>>(1 + thread_count);
    started_barrier = std::make_unique<std::barrier<>>(1 + thread_count);

    if (thread_count > 0) {
        m_workers.push_back(std::make_unique<Worker>(*this, ThreadType::MAIN));
        for (size_t i = 1; i < thread_count; i++) {
            m_workers.push_back(std::make_unique<Worker>(*this, ThreadType::SECONDARY));
        }
    }
}

void Searcher::exit() {
    initialize(0);
}

void Searcher::reset() {
    std::unique_lock lock_guard{mutex};
    for (auto& worker : m_workers) {
        worker->reset_thread_data();
    }
    tt.clear();
}

u64 Searcher::node_count() {
    u64 nodes = 0;
    for (auto& worker : m_workers) {
        nodes += worker->search_nodes();
    }
    return nodes;
}

Worker::Worker(Searcher& searcher, ThreadType thread_type) :
    m_searcher(searcher),
    m_thread_type(thread_type) {
    m_stopped = false;
    m_exiting = false;
    m_thread  = std::thread(&Worker::thread_main, this);
}

Worker::~Worker() {
    m_thread.join();
}

bool Worker::check_tm_hard_limit() {
    time::TimePoint now = time::Clock::now();
    if (now >= m_search_limits.hard_time_limit) {
        m_stopped = true;
        return true;
    }
    return false;
}

void Worker::exit() {
    m_exiting = true;
}

void Worker::thread_main() {
    while (true) {
        m_searcher.idle_barrier->arrive_and_wait();

        if (m_exiting) {
            return;
        }
        {
            std::shared_lock lock_guard{m_searcher.mutex};
            (void)m_searcher.started_barrier->arrive();

            start_searching();
        }
    }
}

void Worker::prepare() {
    m_stopped      = false;
    m_search_nodes = 0;
}

void Worker::start_searching() {
    m_td.psqt_states.reserve(MAX_PLY + 1);
    m_td.psqt_states.clear();
    m_td.psqt_states.emplace_back(root_position);

    // Run iterative deepening search
    if (m_thread_type == ThreadType::MAIN) {
        m_search_start = time::Clock::now();

        m_search_limits = {
          .hard_time_limit = TM::compute_hard_limit(m_search_start, m_searcher.settings,
                                                    root_position.active_color()),
          .soft_time_limit = TM::compute_soft_limit<false>(m_search_start, m_searcher.settings,
                                                           root_position.active_color(), 0.0),
          .soft_node_limit = m_searcher.settings.soft_nodes > 0 ? m_searcher.settings.soft_nodes
                                                                : std::numeric_limits<u64>::max(),
          .hard_node_limit = m_searcher.settings.hard_nodes > 0 ? m_searcher.settings.hard_nodes
                                                                : std::numeric_limits<u64>::max(),
          .depth_limit     = m_searcher.settings.depth > 0 ? m_searcher.settings.depth : MAX_PLY};

        Move best_move = iterative_deepening<true>(root_position);

        // Print (and make sure to flush) the best move
        std::cout << "bestmove " << best_move << std::endl;

        m_searcher.stop_searching();
    } else {
        iterative_deepening<false>(root_position);
    }
}

template<bool IS_MAIN>
Move Worker::iterative_deepening(const Position& root_position) {
    constexpr usize                             SS_PADDING = 2;
    std::array<Stack, MAX_PLY + SS_PADDING + 1> ss;

    Depth last_search_depth = 0;
    Value last_search_score = -VALUE_INF;
    Move  last_best_move    = Move::none();
    PV    last_pv{};

    const auto print_info_line = [&] {
        // Lambda to convert internal units score to uci score. TODO: add eval rescaling here once we get one
        auto format_score = [](Value score) {
            if (score < -VALUE_WIN && score > -VALUE_MATED) {
                return "mate " + std::to_string(-(VALUE_MATED + score + 1) / 2);
            }
            if (score > VALUE_WIN && score < VALUE_MATED) {
                return "mate " + std::to_string((VALUE_MATED + 1 - score) / 2);
            }
            return "cp " + std::to_string(score / 4);
        };

        // Get current time
        auto curr_time = time::Clock::now();

        std::cout << std::dec << "info depth " << last_search_depth << " score "
                  << format_score(last_search_score) << " nodes " << m_searcher.node_count()
                  << " nps " << time::nps(m_searcher.node_count(), curr_time - m_search_start)
                  << " time " << time::cast<time::Milliseconds>(curr_time - m_search_start).count()
                  << " pv " << last_pv << std::endl;
    };

    m_node_counts.fill(0);

    for (Depth search_depth = 1; search_depth < MAX_PLY; search_depth++) {
        // Call search
        Value alpha = -VALUE_INF, beta = VALUE_INF;
        Value delta = 50;
        if (search_depth >= 5) {
            alpha = last_search_score - delta;
            beta  = last_search_score + delta;
        }
        Value score = -VALUE_INF;
        while (true) {
            score = search<IS_MAIN, true>(root_position, &ss[SS_PADDING], alpha, beta, search_depth,
                                          0, false);

            if (m_stopped) {
                break;
            }

            if (score <= alpha) {
                beta  = (alpha + beta) / 2;
                alpha = score - delta;
            } else if (score >= beta) {
                beta = score + delta;
            } else {
                break;
            }

            delta += delta;
        }
        // If m_stopped is true, then the search exited early. Discard the results for this depth.
        if (m_stopped) {
            break;
        }

        // Store information only if the last iterative deepening search completed
        last_search_depth = search_depth;
        last_search_score = score;
        last_pv           = ss[SS_PADDING].pv;
        last_best_move    = last_pv.first_move();

        // Check depth limit
        if (IS_MAIN && search_depth >= m_search_limits.depth_limit) {
            break;
        }

        const auto total_nodes = std::reduce(std::begin(m_node_counts), std::end(m_node_counts), 0);
        const auto best_move_nodes = m_node_counts[last_best_move.from_to()];
        const auto nodes_tm_fraction =
          static_cast<f64>(best_move_nodes) / static_cast<f64>(total_nodes);

        // Check soft node limit
        if (IS_MAIN && search_nodes() >= m_search_limits.soft_node_limit) {
            break;
        }

        time::TimePoint now = time::Clock::now();

        // Starting from depth 6, recalculate the soft time limit based on the fraction of nodes (nodes_tm_fraction)
        // We don't do it for too shallow depths because the node distribution is not stable enough
        if (IS_MAIN && search_depth >= 6) {
            m_search_limits.soft_time_limit = TM::compute_soft_limit<true>(
              m_search_start, m_searcher.settings, root_position.active_color(), nodes_tm_fraction);
        }

        // check soft time limit
        if (IS_MAIN && now >= m_search_limits.soft_time_limit) {
            break;
        }

        if (IS_MAIN) {
            print_info_line();
        }
    }

    // Print last info line
    // This ensures we output our last value of search_nodes before termination, allowing for accurate search reproduction.
    if (IS_MAIN) {
        print_info_line();
    }

    return last_best_move;
}

template<bool IS_MAIN, bool PV_NODE>
Value Worker::search(
  const Position& pos, Stack* ss, Value alpha, Value beta, Depth depth, i32 ply, bool cutnode) {
    ss->pv.clear();

    if (m_stopped) {
        return 0;
    }

    alpha = std::max(alpha, mated_in(ply));
    beta  = std::min(beta, -mated_in(ply) + 1);
    if (alpha >= beta) {
        return alpha;
    }

    if (depth <= 0) {
        return quiesce<IS_MAIN>(pos, ss, alpha, beta, ply);
    }

    const bool ROOT_NODE = ply == 0;
    bool       excluded  = ss->excluded_move != Move::none();

    // TODO: search nodes limit condition here
    // ...
    increment_search_nodes();

    // Check for hard time limit
    // TODO: add control for being main search thread here
    if (IS_MAIN && (search_nodes() & 2047) == 0 && check_tm_hard_limit()) {
        return 0;
    }

    // Check for hard nodes limit
    if (IS_MAIN && search_nodes() >= m_search_limits.hard_node_limit) {
        m_stopped = true;
        return 0;
    }

    // Draw checks
    if (!ROOT_NODE) {
        // Repetition check
        if (repetition_info.detect_repetition(static_cast<usize>(ply))) {
            return 0;
        }
        // 50 mr check
        if (pos.get_50mr_counter() >= 100) {
            return 0;
        }
        // Insufficient material check
        if (pos.is_insufficient_material()) {
            return 0;
        }
    }

    // Return eval if we exceed the max ply.
    if (ply >= MAX_PLY) {
        return evaluate(pos);
    }

    auto tt_data = excluded ? std::nullopt : m_searcher.tt.probe(pos, ply);
    bool ttpv = PV_NODE;

    if (!PV_NODE && tt_data) {
        if (tt_data->depth >= depth
            && (tt_data->bound() == Bound::Exact
                || (tt_data->bound() == Bound::Lower && tt_data->score >= beta)
                || (tt_data->bound() == Bound::Upper && tt_data->score <= alpha))) {
            return tt_data->score;
        }

        // Update ttpv
        ttpv |= tt_data->ttpv();
    }

    bool  is_in_check = pos.is_in_check();
    bool  improving   = false;
    Value correction  = 0;
    Value raw_eval    = -VALUE_INF;
    ss->static_eval   = -VALUE_INF;
    if (!is_in_check) {
        correction      = m_td.history.get_correction(pos);
        raw_eval        = tt_data ? tt_data->eval : evaluate(pos);
        ss->static_eval = raw_eval + correction;
        improving = (ss - 2)->static_eval != -VALUE_INF && ss->static_eval > (ss - 2)->static_eval;

        if (!tt_data) {
            m_searcher.tt.store(pos, ply, raw_eval, Move::none(), -VALUE_INF, 0, ttpv, Bound::None);
        }
    }

    // Internal Iterative Reductions
    if ((PV_NODE || cutnode) && depth >= 8 && (!tt_data || tt_data->move == Move::none())) {
        depth--;
    }

    // Reuse TT score as a better positional evaluation
    auto tt_adjusted_eval = ss->static_eval;
    if (tt_data && tt_data->bound() != Bound::None && abs(tt_data->score) < VALUE_WIN
        && tt_data->bound() != (tt_data->score > ss->static_eval ? Bound::Upper : Bound::Lower)) {
        tt_adjusted_eval = tt_data->score;
    }

    if (!PV_NODE && !is_in_check && depth <= tuned::rfp_depth && !excluded
        && tt_adjusted_eval >= beta + tuned::rfp_margin * depth) {
        return tt_adjusted_eval;
    }

    if (!PV_NODE && !is_in_check && !pos.is_kp_endgame() && depth >= tuned::nmp_depth && !excluded
        && tt_adjusted_eval >= beta + 30) {
        int R =
          tuned::nmp_base_r + depth / 4 + std::min(3, (tt_adjusted_eval - beta) / 400) + improving;
        Position pos_after = pos.null_move();

        repetition_info.push(pos_after.get_hash_key(), true);

        Value value = -search<IS_MAIN, false>(pos_after, ss + 1, -beta, -beta + 1, depth - R,
                                              ply + 1, !cutnode);

        repetition_info.pop();

        if (value >= beta) {
            return value > VALUE_WIN ? beta : value;
        }
    }

    // Razoring
    if (!PV_NODE && !excluded && !is_in_check && depth <= 7 && ss->static_eval + 707 * depth < alpha) {
        const Value razor_score = quiesce<IS_MAIN>(pos, ss, alpha, beta, ply);
        if (razor_score <= alpha) {
            return razor_score;
        }
    }

    MovePicker moves{pos, m_td.history, tt_data ? tt_data->move : Move::none(), ply, ss};
    Move       best_move    = Move::none();
    Value      best_value   = -VALUE_INF;
    i32        moves_played = 0;
    MoveList   quiets_played;
    MoveList   noisies_played;
    i32        alpha_raises = 0;

    // Clear child's killer move.
    (ss + 1)->killer = Move::none();
    // Clear child's fail high count
    (ss + 1)->fail_high_count = 0;

    // Iterate over the move list
    for (Move m = moves.next(); m != Move::none(); m = moves.next()) {
         if (m == ss->excluded_move) {
            continue;
        }

        const auto nodes_before = m_search_nodes.load(std::memory_order::relaxed);
        bool       quiet        = quiet_move(m);

        auto move_history = quiet ? m_td.history.get_quiet_stats(pos, m, ply, ss) : 0;

        if (!ROOT_NODE && best_value > -VALUE_WIN) {
            // Late Move Pruning (LMP)
            if (moves_played >= (3 + depth * depth) / (2 - improving)) {
                break;
            }

            // Forward Futility Pruning (FFP)
            Value futility = ss->static_eval + 500 + 100 * depth + move_history / 32;
            if (quiet && !is_in_check && depth <= 8 && futility <= alpha) {
                moves.skip_quiets();
                continue;
            }

            // Quiet History Pruning
            if (depth <= 4 && !is_in_check && quiet && move_history < depth * -2048) {
                break;
            }

            Value see_threshold = quiet ? -67 * depth : -22 * depth * depth;
            // SEE PVS Pruning
            if (depth <= 10 && !SEE::see(pos, m, see_threshold - move_history * 20 / 1024)) {
                continue;
            }
        }

        // Singular extensions
        int extension = 0;
        if (!excluded && tt_data && m == tt_data->move && depth >= 8 && tt_data->depth >= depth - 3
            && tt_data->bound() != Bound::Upper) {
            Value singular_beta  = tt_data->score - depth * 5;
            int   singular_depth = depth / 2;

            ss->excluded_move    = m;
            Value singular_value = search<IS_MAIN, false>(pos, ss, singular_beta - 1, singular_beta,
                                                          singular_depth, ply, cutnode);
            ss->excluded_move    = Move::none();

            if (singular_value < singular_beta) {
                extension = 1;

                // Double Extension
                if (!PV_NODE && singular_value <= singular_beta - 40) {
                    extension = 2;
                }                
            }
            
            // Negative Extensions
            else if (tt_data->score >= beta) {
                extension = -1;
            }
        }        
        
        // Do move
        ss->cont_hist_entry = &m_td.history.get_cont_hist_entry(pos, m);

        Position pos_after = pos.move(m, m_td.push_psqt_state());
        moves_played++;

        // Put hash into repetition table. TODO: encapsulate this and any other future adjustment to do "on move" into a proper function
        repetition_info.push(pos_after.get_hash_key(), pos_after.is_reversible(m));

        // Get search value
        Depth new_depth = depth - 1 + pos_after.is_in_check() + extension;
        Value value;
        if (depth >= 3 && moves_played >= 2 + 2 * PV_NODE) {
            i32 reduction = static_cast<i32>(
              std::round(1024 * (0.77 + std::log(depth) * std::log(moves_played) / 2.36)));
            reduction -= 1024 * PV_NODE;

            reduction += alpha_raises * 512;

            reduction += (512 * !improving);

            if (cutnode) {
                reduction += 1024;
            }

            if (ttpv) {
                reduction -= 1024;
            }

            if (ttpv && tt_data && tt_data->score <= alpha) {
                reduction += 1024;
            }

            if (tt_data && tt_data->move.is_capture() && !m.is_capture()) {
                reduction += 1024;
            }

            if ((ss + 1)->fail_high_count > 3) {
                reduction += 1024;
            }

            if (quiet) {
                reduction += (1024 - move_history / 8);
            }

            if (!quiet) {
                reduction = std::min(reduction, 2048);
            }

            reduction /= 1024;

            Depth reduced_depth = std::clamp<Depth>(new_depth - reduction, 1, new_depth);
            value = -search<IS_MAIN, false>(pos_after, ss + 1, -alpha - 1, -alpha, reduced_depth,
                                            ply + 1, true);
            if (value > alpha && reduced_depth < new_depth) {
                value = -search<IS_MAIN, false>(pos_after, ss + 1, -alpha - 1, -alpha, new_depth,
                                                ply + 1, !cutnode);
            }
        } else if (!PV_NODE || moves_played > 1) {
            value = -search<IS_MAIN, false>(pos_after, ss + 1, -alpha - 1, -alpha, new_depth,
                                            ply + 1, !cutnode);
        }

        if (PV_NODE && (moves_played == 1 || value > alpha)) {
            value =
              -search<IS_MAIN, true>(pos_after, ss + 1, -beta, -alpha, new_depth, ply + 1, false);
        }
        const auto nodes_after = m_search_nodes.load(std::memory_order::relaxed);
        if (ROOT_NODE) {
            m_node_counts[m.from_to()] += nodes_after - nodes_before;
        }

        // TODO: encapsulate this and any other future adjustment to do "on going back" into a proper function
        repetition_info.pop();
        m_td.pop_psqt_state();
        ss->cont_hist_entry = nullptr;

        if (m_stopped) {
            return 0;
        }

        if (value > best_value) {
            best_value = value;

            if (value > alpha) {
                if (PV_NODE) {
                    ss->pv.set(m, (ss + 1)->pv);
                }
                alpha     = value;
                best_move = m;
                alpha_raises++;

                if (value >= beta) {
                    ss->fail_high_count++;
                    break;
                }
            }
        }

        if (best_move != m) {
            if (quiet_move(m)) {
                quiets_played.push_back(m);
            } else {
                noisies_played.push_back(m);
            }
        }
    }

    if (best_value >= beta) {
        i32       bonus_depth = depth + (best_value >= beta + 100);
        const i32 bonus = std::min(1896, 4 * bonus_depth * bonus_depth + 120 * bonus_depth - 120);
        if (quiet_move(best_move)) {
            ss->killer = best_move;

            m_td.history.update_quiet_stats(pos, best_move, ply, ss, bonus);
            for (Move quiet : quiets_played) {
                m_td.history.update_quiet_stats(pos, quiet, ply, ss, -bonus);
            }
        } else {
            m_td.history.update_noisy_stats(pos, best_move, bonus);
        }
        for (Move noisy : noisies_played) {
            m_td.history.update_noisy_stats(pos, noisy, -bonus);
        }
    }

    // Checkmate / Stalemate check
    if (best_value == -VALUE_INF) {
        if (excluded) {
            return alpha;
        } else {
            if (pos.is_in_check()) {
            return mated_in(ply);
            } else {
                return 0;
            }
        }        
    }

    if (!excluded) {
        Bound bound   = best_value >= beta        ? Bound::Lower
                  : best_move != Move::none() ? Bound::Exact
                                              : Bound::Upper;
        Move  tt_move = best_move != Move::none() ? best_move : tt_data ? tt_data->move : Move::none();
        m_searcher.tt.store(pos, ply, raw_eval, tt_move, best_value, depth, ttpv, bound);

        // Update to correction history.
        if (!is_in_check
            && !(best_move != Move::none() && (best_move.is_capture() || best_move.is_promotion()))
            && !((bound == Bound::Lower && best_value <= ss->static_eval)
                || (bound == Bound::Upper && best_value >= ss->static_eval))) {
            m_td.history.update_correction_history(pos, depth, best_value - raw_eval);
        }
    }

    

    return best_value;
}

template<bool IS_MAIN>
Value Worker::quiesce(const Position& pos, Stack* ss, Value alpha, Value beta, i32 ply) {
    ss->pv.clear();

    if (m_stopped) {
        return 0;
    }

    increment_search_nodes();

    // Check for hard time limit
    if (IS_MAIN && (search_nodes() & 2047) == 0 && check_tm_hard_limit()) {
        return 0;
    }

    // Check for hard nodes limit
    if (IS_MAIN && search_nodes() >= m_search_limits.hard_node_limit) {
        m_stopped = true;
        return 0;
    }

    // 50 mr check
    if (pos.get_50mr_counter() >= 100) {
        return 0;
    }

    // Return eval if we exceed the max ply.
    if (ply >= MAX_PLY) {
        return evaluate(pos);
    }

    // TT Probing
    auto tt_data = m_searcher.tt.probe(pos, ply);
    if (tt_data
        && (tt_data->bound() == Bound::Exact
            || (tt_data->bound() == Bound::Lower && tt_data->score >= beta)
            || (tt_data->bound() == Bound::Upper && tt_data->score <= alpha))) {
        return tt_data->score;
    }

    bool is_in_check = pos.is_in_check();
    bool ttpv =
      tt_data
        ? tt_data->ttpv()
        : false;  // TODO: if we ever get to needing ttpv patches in quiescence, we might want to add PV_NODE handling in here also
    Value correction  = 0;
    Value raw_eval    = -VALUE_INF;
    Value static_eval = -VALUE_INF;
    if (!is_in_check) {
        correction  = m_td.history.get_correction(pos);
        raw_eval    = tt_data ? tt_data->eval : evaluate(pos);
        static_eval = raw_eval + correction;

        if (!tt_data) {
            m_searcher.tt.store(pos, ply, raw_eval, Move::none(), -VALUE_INF, 0, ttpv, Bound::None);
        }
    }

    // Stand pat
    if (static_eval >= beta) {
        return static_eval;
    }
    alpha = std::max(alpha, static_eval);

    MovePicker moves{pos, m_td.history, Move::none(), ply, ss};
    if (!is_in_check) {
        moves.skip_quiets();
    }

    Move  best_move      = Move::none();
    Value best_value     = static_eval;
    u32   moves_searched = 0;

    // Iterate over the move list
    for (Move m = moves.next(); m != Move::none(); m = moves.next()) {
        // QS SEE Pruning
        if (best_value > -VALUE_WIN && !SEE::see(pos, m, tuned::quiesce_see_threshold)) {
            continue;
        }

        // Do move
        ss->cont_hist_entry = &m_td.history.get_cont_hist_entry(pos, m);
        Position pos_after  = pos.move(m, m_td.push_psqt_state());
        moves_searched++;

        // If we've found a legal move, then we can begin skipping quiet moves.
        moves.skip_quiets();

        // Put hash into repetition table. TODO: encapsulate this and any other future adjustment to do "on move" into a proper function
        repetition_info.push(pos_after.get_hash_key(), pos_after.is_reversible(m));

        // Get search value
        Value value = -quiesce<IS_MAIN>(pos_after, ss + 1, -beta, -alpha, ply + 1);

        // TODO: encapsulate this and any other future adjustment to do "on going back" into a proper function
        repetition_info.pop();
        m_td.pop_psqt_state();
        ss->cont_hist_entry = nullptr;

        if (m_stopped) {
            return 0;
        }

        if (value > best_value) {
            best_value = value;

            if (value > alpha) {
                alpha     = value;
                best_move = m;

                if (value >= beta) {
                    break;
                }
            }
        }
    }

    // Checkmate check
    if (is_in_check && moves_searched == 0) {
        return mated_in(ply);
    }

    // Store to the TT
    Bound bound   = best_value >= beta ? Bound::Lower : Bound::Upper;
    Move  tt_move = best_move != Move::none() ? best_move : tt_data ? tt_data->move : Move::none();
    m_searcher.tt.store(pos, ply, raw_eval, tt_move, best_value, 0, ttpv, bound);

    return best_value;
}

Value Worker::evaluate(const Position& pos) {
#ifndef EVAL_TUNING
    return static_cast<Value>(Clockwork::evaluate_stm_pov(pos, m_td.psqt_states.back()));
#else
    return -VALUE_INF;  // Not implemented in tune mode
#endif
}
}
}

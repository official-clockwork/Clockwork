#include "search.hpp"
#include "board.hpp"
#include "common.hpp"
#include "movegen.hpp"
#include "movepick.hpp"
#include "see.hpp"
#include "tm.hpp"
#include "tuned.hpp"
#include "uci.hpp"
#include "util/types.hpp"
#include <array>
#include <cmath>
#include <iostream>
#include <limits>

namespace Clockwork {
namespace Search {

Value mated_in(i32 ply) {
    return -VALUE_MATED + ply;
}

Worker::Worker(TT& tt, ThreadData& td) :
    m_tt(tt),
    m_td(td) {
}


bool Worker::check_tm_hard_limit() {
    time::TimePoint now = time::Clock::now();
    if (now >= m_search_limits.hard_time_limit) {
        m_stopped = true;
        return true;
    }
    return false;
}

void Worker::launch_search(Position            root_position,
                           RepetitionInfo      repetition_info,
                           UCI::SearchSettings settings) {
    // Search setup
    m_search_start = time::Clock::now();
    search_nodes   = 0;
    m_stopped      = false;

    // Get a copy of the repetition_info to futureproof for multithreaded search
    m_repetition_info = repetition_info;

    // TODO: time setup only needed by the main worker thread
    m_search_limits = {.hard_time_limit = TM::compute_hard_limit(m_search_start, settings,
                                                                 root_position.active_color()),
                       .soft_node_limit = settings.soft_nodes > 0 ? settings.soft_nodes
                                                                  : std::numeric_limits<u64>::max(),
                       .hard_node_limit = settings.hard_nodes > 0 ? settings.hard_nodes
                                                                  : std::numeric_limits<u64>::max(),
                       .depth_limit     = settings.depth > 0 ? settings.depth : MAX_PLY};

    m_td.history.clear();

    // Run iterative deepening search
    Move best_move = iterative_deepening(root_position);

    // Print (and make sure to flush) the best move
    std::cout << "bestmove " << best_move << std::endl;
}

Move Worker::iterative_deepening(Position root_position) {

    std::array<Stack, MAX_PLY + 1> ss;
    std::array<Move, MAX_PLY + 1>  pv;
    Value                          alpha = -VALUE_INF, beta = +VALUE_INF;

    Depth root_depth = m_search_limits.depth_limit;
    for (u32 i = 0; i < static_cast<u32>(MAX_PLY); i++) {
        ss[i].pv = &pv[i];
    }

    Depth last_search_depth = 0;
    Value last_search_score = -VALUE_INF;
    Move  last_best_move    = Move::none();

    const auto print_info_line = [&] {
        // Lambda to convert internal units score to uci score. TODO: add eval rescaling here once we get one
        auto format_score = [](Value score) {
            if (score < -VALUE_WIN && score > -VALUE_MATED) {
                return "mate " + std::to_string(-(VALUE_MATED + score + 2) / 2);
            }
            if (score > VALUE_WIN && score < VALUE_MATED) {
                return "mate " + std::to_string((VALUE_MATED + 1 - score) / 2);
            }
            return "cp " + std::to_string(score);
        };

        // Get current time
        auto curr_time = time::Clock::now();

        std::cout << std::dec << "info depth " << last_search_depth << " score "
                  << format_score(last_search_score) << " nodes " << search_nodes << " nps "
                  << time::nps(search_nodes, curr_time - m_search_start) << " pv " << last_best_move
                  << std::endl;
    };

    for (Depth search_depth = 1;; search_depth++) {
        // Call search
        Value score = search<true>(root_position, &ss[0], alpha, beta, search_depth, 0);

        // If m_stopped is true, then the search exited early. Discard the results for this depth.
        if (m_stopped) {
            break;
        }

        // Store information only if the last iterative deepening search completed
        last_search_depth = search_depth;
        last_search_score = score;
        last_best_move    = *ss[0].pv;

        // Check depth limit
        if (search_depth >= root_depth) {
            break;
        }
        // Check soft node limit
        if (search_nodes >= m_search_limits.soft_node_limit) {
            break;
        }
        // TODO: add any soft time limit check here

        print_info_line();
    }

    // Print last info line
    // This ensures we output our last value of search_nodes before termination, allowing for accurate search reproduction.
    print_info_line();

    return last_best_move;
}

template<bool PV_NODE>
Value Worker::search(Position& pos, Stack* ss, Value alpha, Value beta, Depth depth, i32 ply) {
    if (m_stopped) {
        return 0;
    }

    if (depth <= 0) {
        return quiesce(pos, ss, alpha, beta, ply);
    }

    const bool ROOT_NODE = ply == 0;

    // TODO: search nodes limit condition here
    // ...
    search_nodes++;

    // Check for hard time limit
    // TODO: add control for being main search thread here
    if ((search_nodes & 2047) == 0 && check_tm_hard_limit()) {
        return 0;
    }

    // Check for hard nodes limit
    if (search_nodes >= m_search_limits.hard_node_limit) {
        m_stopped = true;
        return 0;
    }

    // Draw checks
    if (!ROOT_NODE) {
        // Repetition check
        if (m_repetition_info.detect_repetition(ply)) {
            return 0;
        }
        // 50 mr check
        if (pos.get_50mr_counter() >= 100) {
            return 0;
        }
    }

    // Return eval if we exceed the max ply.
    if (ply >= MAX_PLY) {
        return evaluate(pos);
    }

    auto tt_data = m_tt.probe(pos, ply);
    if (!PV_NODE && tt_data && tt_data->depth >= depth
        && (tt_data->bound == Bound::Exact
            || (tt_data->bound == Bound::Lower && tt_data->score >= beta)
            || (tt_data->bound == Bound::Upper && tt_data->score <= alpha))) {
        return tt_data->score;
    }

    bool  is_in_check = pos.is_in_check();
    Value static_eval = is_in_check ? -VALUE_INF : evaluate(pos);

    // Internal Iterative Reductions
    if (PV_NODE && depth >= 8 && (!tt_data || tt_data->move == Move::none())) {
        depth--;
    }

    // Reuse TT score as a better positional evaluation
    auto tt_adjusted_eval = static_eval;
    if (tt_data && tt_data->bound != (tt_data->score > static_eval ? Bound::Upper : Bound::Lower)) {
        tt_adjusted_eval = tt_data->score;
    }

    if (!PV_NODE && !is_in_check && depth <= tuned::rfp_depth
        && tt_adjusted_eval >= beta + tuned::rfp_margin * depth) {
        return tt_adjusted_eval;
    }

    if (!PV_NODE && !is_in_check && depth >= tuned::nmp_depth && static_eval >= beta) {
        int      R         = tuned::nmp_base_r + std::min(3, (static_eval - beta) / 300);        
        Position pos_after = pos.null_move();

        m_repetition_info.push(pos_after.get_hash_key(), true);

        Value value = -search<false>(pos_after, ss + 1, -beta, -beta + 1, depth - R, ply + 1);

        m_repetition_info.pop();

        if (value >= beta) {
            return value > VALUE_WIN ? beta : value;
        }
    }

    // Razoring
    if (!PV_NODE && !is_in_check && depth <= 7 && static_eval + 260 * depth < alpha) {
        const Value razor_score = quiesce(pos, ss, alpha, beta, ply);
        if (razor_score <= alpha) {
            return razor_score;
        }
    }

    MovePicker moves{pos, m_td.history, tt_data ? tt_data->move : Move::none(), ss->killer};
    Move       best_move    = Move::none();
    Value      best_value   = -VALUE_INF;
    i32        moves_played = 0;
    MoveList   quiets_played;

    // Clear child's killer move.
    (ss + 1)->killer = Move::none();

    // Iterate over the move list
    for (Move m = moves.next(); m != Move::none(); m = moves.next()) {
        bool quiet = quiet_move(m);

        auto move_history = quiet ? m_td.history.get_quiet_stats(pos, m) : 0;

        if (!ROOT_NODE && best_value > -VALUE_WIN && quiet) {
            // Late Move Pruning (LMP)
            if (moves_played >= 4 + 3 * depth * depth) {
                continue;
            }
            // Quiet History Pruning
            if (depth <= 4 && !is_in_check && move_history < depth * -2048) {
                break;
            }
        }

        Value see_threshold = quiet ? -67 * depth : -64 * depth;
        // SEE PVS Pruning
        if (depth <= 10 && !ROOT_NODE && !SEE::see(pos, m, see_threshold)) {
            continue;
        }

        // Do move
        Position pos_after = pos.move(m);
        moves_played++;

        // Put hash into repetition table. TODO: encapsulate this and any other future adjustment to do "on move" into a proper function
        m_repetition_info.push(pos_after.get_hash_key(), pos_after.is_reversible(m));

        // Get search value
        Depth new_depth = depth - 1 + pos_after.is_in_check();
        Value value;
        if (depth >= 3 && moves_played >= 4 && quiet) {
            i32 reduction =
              static_cast<i32>(0.77 + std::log(depth) * std::log(moves_played) / 2.36);
            reduction -= PV_NODE;
            Depth reduced_depth = std::clamp<Depth>(new_depth - reduction, 1, new_depth);
            value = -search<false>(pos_after, ss + 1, -alpha - 1, -alpha, reduced_depth, ply + 1);
            if (value > alpha && reduced_depth < new_depth) {
                value = -search<false>(pos_after, ss + 1, -alpha - 1, -alpha, new_depth, ply + 1);
            }
        } else if (!PV_NODE || moves_played > 1) {
            value = -search<false>(pos_after, ss + 1, -alpha - 1, -alpha, new_depth, ply + 1);
        }

        if (PV_NODE && (moves_played == 1 || value > alpha)) {
            value = -search<true>(pos_after, ss + 1, -beta, -alpha, new_depth, ply + 1);
        }

        // TODO: encapsulate this and any other future adjustment to do "on going back" into a proper function
        m_repetition_info.pop();

        if (m_stopped) {
            return 0;
        }

        if (value > best_value) {
            best_value = value;
            if (ply == 0) {
                ss->pv[ply] = m;  // No pv update for now, just bestmove
            }

            if (value > alpha) {
                alpha     = value;
                best_move = m;

                if (value >= beta) {
                    break;
                }
            }
        }

        if (best_move != m && quiet_move(m)) {
            quiets_played.push_back(m);
        }
    }

    if (best_value >= beta && quiet_move(best_move)) {
        ss->killer = best_move;

        const i32 bonus = std::min(1896, 4 * depth * depth + 120 * depth - 120);
        m_td.history.update_quiet_stats(pos, best_move, bonus);
        for (Move quiet : quiets_played) {
            m_td.history.update_quiet_stats(pos, quiet, -bonus);
        }
    }

    // Checkmate / Stalemate check
    if (best_value == -VALUE_INF) {
        if (pos.is_in_check()) {
            return mated_in(ply);
        } else {
            return 0;
        }
    }

    Bound bound = best_value >= beta        ? Bound::Lower
                : best_move != Move::none() ? Bound::Exact
                                            : Bound::Upper;
    m_tt.store(pos, ply, best_move, best_value, depth, bound);

    return best_value;
}

Value Worker::quiesce(Position& pos, Stack* ss, Value alpha, Value beta, i32 ply) {
    if (m_stopped) {
        return 0;
    }

    search_nodes++;

    // Check for hard time limit
    if ((search_nodes & 2047) == 0 && check_tm_hard_limit()) {
        return 0;
    }

    // Check for hard nodes limit
    if (search_nodes >= m_search_limits.hard_node_limit) {
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

    bool  is_in_check = pos.is_in_check();
    Value static_eval = is_in_check ? -VALUE_INF : evaluate(pos);

    // Stand pat
    if (static_eval >= beta) {
        return static_eval;
    }
    alpha = std::max(alpha, static_eval);

    MovePicker moves{pos, m_td.history};
    if (!is_in_check) {
        moves.skip_quiets();
    }

    Value best_value     = static_eval;
    u32   moves_searched = 0;

    // Iterate over the move list
    for (Move m = moves.next(); m != Move::none(); m = moves.next()) {
        // QS SEE Pruning
        if (best_value > -VALUE_WIN && !SEE::see(pos, m, tuned::quiesce_see_threshold)) {
            continue;
        }

        // Do move
        Position pos_after = pos.move(m);
        moves_searched++;

        // If we've found a legal move, then we can begin skipping quiet moves.
        moves.skip_quiets();

        // Put hash into repetition table. TODO: encapsulate this and any other future adjustment to do "on move" into a proper function
        m_repetition_info.push(pos_after.get_hash_key(), pos_after.is_reversible(m));

        // Get search value
        Value value = -quiesce(pos_after, ss + 1, -beta, -alpha, ply + 1);

        // TODO: encapsulate this and any other future adjustment to do "on going back" into a proper function
        m_repetition_info.pop();

        if (m_stopped) {
            return 0;
        }

        if (value > best_value) {
            best_value = value;

            if (value > alpha) {
                alpha = value;

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

    return best_value;
}

Value Worker::evaluate(const Position& pos) {
    const Color us   = pos.active_color();
    const Color them = invert(us);

    Value material =
      100 * (pos.piece_count(us, PieceType::Pawn) - pos.piece_count(them, PieceType::Pawn))
      + 330 * (pos.piece_count(us, PieceType::Knight) - pos.piece_count(them, PieceType::Knight))
      + 370 * (pos.piece_count(us, PieceType::Bishop) - pos.piece_count(them, PieceType::Bishop))
      + 550 * (pos.piece_count(us, PieceType::Rook) - pos.piece_count(them, PieceType::Rook))
      + 1000 * (pos.piece_count(us, PieceType::Queen) - pos.piece_count(them, PieceType::Queen));

    Value mobility = 0;
    for (u64 x : std::bit_cast<std::array<u64, 16>>(pos.attack_table(us))) {
        mobility += 10 * std::popcount(x);
    }
    for (u64 x : std::bit_cast<std::array<u64, 16>>(pos.attack_table(them))) {
        mobility -= 10 * std::popcount(x);
    }

    Value fudge = static_cast<i32>(search_nodes & 7) - 3;

    return material + mobility + fudge;
}
}
}

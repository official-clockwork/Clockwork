#include "search.hpp"
#include "board.hpp"
#include "common.hpp"
#include "movegen.hpp"
#include "uci.hpp"
#include "util/types.hpp"
#include <array>
#include <iostream>

namespace Clockwork {
namespace Search {

Value mated_in(i32 ply) {
    return -VALUE_MATED + ply;
}

void Worker::launch_search(Position root_position, UCI::SearchSettings settings) {
    search_nodes = 0;
    // Tm setup (skipped for now)
    Move best_move = iterative_deepening(root_position, settings);
    std::cout << "bestmove " << best_move << std::endl;
}

Move Worker::iterative_deepening(Position root_position, UCI::SearchSettings settings) {

    std::array<Stack, MAX_PLY + 1> ss;
    std::array<Move, MAX_PLY + 1>  pv;
    Value                          alpha = -VALUE_INF, beta = +VALUE_INF;
    Value                          best_value;

    Depth root_depth = settings.depth;
    for (u32 i = 0; i < static_cast<u32>(MAX_PLY); i++) {
        ss[i].pv = &pv[i];
    }
    Value score = search(root_position, &ss[0], alpha, beta, root_depth, 0);
    best_value  = score;

    Move best_move = *ss[0].pv;
    return best_move;
}

Value Worker::search(Position& pos, Stack* ss, Value alpha, Value beta, Depth depth, i32 ply) {
    // TODO: search nodes limit condition here
    // ...
    search_nodes++;

    // Return eval (TODO: quiescence) if depth is 0 or we exceed the max ply.
    if (depth == 0 || ply >= MAX_PLY) {
        return evaluate(pos);
    }

    MoveList moves;
    MoveGen  movegen{pos};
    Value    best_value = -VALUE_INF;
    movegen.generate_moves(moves);
    for (Move m : moves) {
        Position pos_after = pos.move(m);  // Will require a do_move to handle future features.

        Value value = -search(pos_after, ss + 1, -beta, -alpha, depth - 1, ply + 1);
        if (value > best_value) {
            best_value = value;
            if (ply == 0) {
                ss->pv[ply] = m;  // No pv update for now, just bestmove
            }
            // if (value > alpha) {
            //
            //    if (value >= beta) {
            //        break;
            //    }
            //    alpha = std::max(alpha, value);
            // }
        }
    }

    if (best_value == -VALUE_INF) {
        if (pos.is_in_check()) {
            return mated_in(ply);
        } else {
            return 0;
        }
    }

    return best_value;
}

Value Worker::evaluate(const Position& pos) {
    const Color us   = pos.active_color();
    const Color them = invert(us);
    return 100 * (pos.piece_count(us, PieceType::Pawn) - pos.piece_count(them, PieceType::Pawn))
         + 300 * (pos.piece_count(us, PieceType::Knight) - pos.piece_count(them, PieceType::Knight))
         + 300 * (pos.piece_count(us, PieceType::Bishop) - pos.piece_count(them, PieceType::Bishop))
         + 500 * (pos.piece_count(us, PieceType::Rook) - pos.piece_count(them, PieceType::Rook))
         + 900 * (pos.piece_count(us, PieceType::Queen) - pos.piece_count(them, PieceType::Queen));
}

}
}

#include "search.hpp"
#include "board.hpp"
#include "common.hpp"
#include "movegen.hpp"
#include "util/types.hpp"
#include <iostream>
#include <array>

namespace Clockwork{
    namespace Search {

        Value mated_in(i32 ply) {
            return -VALUE_MATED + ply;
        }

        void Worker::launch_search(Position root_position){
            // Tm setup (skipped for now)
            Move best_move = iterative_deepening(root_position);
            std::cout << "bestmove " << best_move << std::endl;
        }

        Move Worker::iterative_deepening(Position root_position){
            
            std::array<Stack, MAX_PLY> ss;
            std::array<Move, MAX_PLY + 1> pv;
            Value alpha = -VALUE_INF, beta=+VALUE_INF;
            Value best_value;

            Depth root_depth = 1;
            ss[0].pv = &(pv[0]);
            Value score = search(root_position, &ss[0], alpha, beta, root_depth);
            best_value = score;

            Move best_move = *ss[0].pv;
            return best_move;

        }

        Value Worker::search(Position& pos, Stack* ss, Value alpha, Value beta, Depth depth){
            if (depth == 0) {
                return evaluate(pos);
            }
        
            MoveList moves;
            MoveGen  movegen{pos};
            Value best_value = -VALUE_INF;
            movegen.generate_moves(moves);
            for (Move m : moves) {
                Position pos_after = pos.move(m); // Will require a do_move to handle future features.
                if (!pos_after.is_valid()) {
                    continue;
                }
        
                Value value = -search(pos_after, ss+1, -beta, -alpha, depth - 1);
                if (value > best_value){
                    best_value = value;
                    if (value > alpha){
                        if (ss->ply == 0) {
                            ss->pv[0] = m; // No pv update for now, just bestmove
                        }
                        if (value >= beta) {
                            break; // Beta cutoff
                        }
                        alpha = std::max(alpha, value);
                    }
                }
            }

            if (best_value == -VALUE_INF){
                if (pos.is_in_check()) return mated_in(ss->ply);
                else return 0;
            }
        
            return best_value;
        }

        Value Worker::evaluate(const Position& pos){
            const auto& board = pos.board();
            const Color us = pos.active_color();
            const Color them = invert(us);
            return (
                (static_cast<i32>(board.bitboard_for(us, PieceType::Pawn).popcount()) - static_cast<i32>(board.bitboard_for(them, PieceType::Pawn).popcount()) ) * 100 
                + (static_cast<i32>(board.bitboard_for(us, PieceType::Knight).popcount()) - static_cast<i32>(board.bitboard_for(them, PieceType::Knight).popcount()) ) * 300 
                + (static_cast<i32>(board.bitboard_for(us, PieceType::Bishop).popcount()) - static_cast<i32>(board.bitboard_for(them, PieceType::Bishop).popcount()) ) * 300 
                + (static_cast<i32>(board.bitboard_for(us, PieceType::Rook).popcount()) - static_cast<i32>(board.bitboard_for(them, PieceType::Rook).popcount()) ) * 500 
                + (static_cast<i32>(board.bitboard_for(us, PieceType::Queen).popcount()) - static_cast<i32>(board.bitboard_for(them, PieceType::Queen).popcount()) ) * 900 
            );
        }
    }
}
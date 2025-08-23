
#include "position.hpp"
#include "tuning/graph.hpp"
#include "tuning/loss.hpp"
#include "tuning/optim.hpp"
#include "tuning/value.hpp"
#include "uci.hpp"
#include "util/pretty.hpp"
#include "util/types.hpp"
#include "zobrist.hpp"
#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <numeric>
#include <random>
#include <tuple>

#define EVAL_TUNING

using namespace Clockwork;

int main(int argc, char* argv[]) {

    // Initialize all necessary tables (TODO: we may need to move this to a dedicated file)
    #ifndef EVAL_TUNING
    Zobrist::init_zobrist_keys();

    UCI::UCIHandler uci;

    if (argc > 1) {
        uci.handle_command_line(argc, argv);
    } else {
        uci.loop();
    }
    #else
    // Load fens.
    std::vector<std::string> fens;
    std::vector<f64>         results;
    std::ifstream            fenFile("data/filtered_positions.fen");
    if (!fenFile) {
        std::cerr << "Error opening fens.book" << std::endl;
        return 1;
    };
    std::string line;
    while (std::getline(fenFile, line)) {
        size_t pos = line.find(';');
        if (pos != std::string::npos) {
            std::string fen = line.substr(0, pos);
            fens.push_back(fen);
            std::string result = line.substr(pos + 1);
            result.erase(std::remove_if(result.begin(), result.end(), ::isspace), result.end());

            if (result == "w") {
                results.push_back(1.0);
            } else if (result == "d") {
                results.push_back(0.5);
            } else if (result == "b") {
                results.push_back(0.0);
            } else {
                std::cerr << "Invalid result in line: " << line << " result is (" << result << ")"
                          << std::endl;
            }
        } else {
            std::cerr << "Invalid line format: " << line << std::endl;
        }
    }

    fenFile.close();

    // Print the number of fens loaded
    std::cout << "Loaded " << fens.size() << " FENs." << std::endl;

    auto PAWN_MAT     = Clockwork::Autograd::Pair<f64>::create_tunable(0, 0);
    auto KNIGHT_MAT   = Clockwork::Autograd::Pair<f64>::create_tunable(0, 0);
    auto BISHOP_MAT   = Clockwork::Autograd::Pair<f64>::create_tunable(0, 0);
    auto ROOK_MAT     = Clockwork::Autograd::Pair<f64>::create_tunable(0, 0);
    auto QUEEN_MAT    = Clockwork::Autograd::Pair<f64>::create_tunable(0, 0);
    auto MOBILITY_VAL = Clockwork::Autograd::Pair<f64>::create_tunable(0, 0);
    auto TEMPO_VAL    = Clockwork::Autograd::Pair<f64>::create_tunable(0, 0);

    Clockwork::Autograd::Graph<f64>::get()->register_param(PAWN_MAT);
    Clockwork::Autograd::Graph<f64>::get()->register_param(KNIGHT_MAT);
    Clockwork::Autograd::Graph<f64>::get()->register_param(BISHOP_MAT);
    Clockwork::Autograd::Graph<f64>::get()->register_param(ROOK_MAT);
    Clockwork::Autograd::Graph<f64>::get()->register_param(QUEEN_MAT);
    Clockwork::Autograd::Graph<f64>::get()->register_param(MOBILITY_VAL);
    Clockwork::Autograd::Graph<f64>::get()->register_param(TEMPO_VAL);

    auto tunable_function = [PAWN_MAT, KNIGHT_MAT, BISHOP_MAT, ROOK_MAT, QUEEN_MAT, MOBILITY_VAL,
                             TEMPO_VAL](std::string fen) {
        auto pos = Position::parse(fen).value();

        const Color us   = pos.active_color();
        const Color them = invert(us);

        i32 phase = pos.piece_count(Color::White, PieceType::Knight)
                  + pos.piece_count(Color::Black, PieceType::Knight)
                  + pos.piece_count(Color::White, PieceType::Bishop)
                  + pos.piece_count(Color::Black, PieceType::Bishop)
                  + 2
                      * (pos.piece_count(Color::White, PieceType::Rook)
                         + pos.piece_count(Color::Black, PieceType::Rook))
                  + 4
                      * (pos.piece_count(Color::White, PieceType::Queen)
                         + pos.piece_count(Color::Black, PieceType::Queen));
        phase = std::min<i32>(phase, 24);

        auto material = PAWN_MAT
                        * static_cast<f64>(pos.piece_count(Color::White, PieceType::Pawn)
                                           - pos.piece_count(Color::Black, PieceType::Pawn))
                      + KNIGHT_MAT
                          * static_cast<f64>(pos.piece_count(Color::White, PieceType::Knight)
                                             - pos.piece_count(Color::Black, PieceType::Knight))
                      + BISHOP_MAT
                          * static_cast<f64>(pos.piece_count(Color::White, PieceType::Bishop)
                                             - pos.piece_count(Color::Black, PieceType::Bishop))
                      + ROOK_MAT
                          * static_cast<f64>(pos.piece_count(Color::White, PieceType::Rook)
                                             - pos.piece_count(Color::Black, PieceType::Rook))
                      + QUEEN_MAT
                          * static_cast<f64>(pos.piece_count(Color::White, PieceType::Queen)
                                             - pos.piece_count(Color::Black, PieceType::Queen));
        
        i32 mob_count = 0;
        for (u64 x : std::bit_cast<std::array<u64, 16>>(pos.attack_table(Color::White))) {
            mob_count += std::popcount(x);
        }
        for (u64 x : std::bit_cast<std::array<u64, 16>>(pos.attack_table(Color::Black))) {
            mob_count -= std::popcount(x);
        }

        auto mobility = MOBILITY_VAL * static_cast<f64>(mob_count);

        auto tempo = (us == Color::White) ? TEMPO_VAL : -TEMPO_VAL;

        return (material + mobility + tempo)->phase(static_cast<f64>(phase) / 24.0);
    };

    auto                            loss_fn = Clockwork::Autograd::mse<f64>;
    Clockwork::Autograd::AdamW<f64> optim(10, 0.9, 0.999, 1e-8, 0.0);
    //Clockwork::Autograd::SGD<f64> optim(Clockwork::Autograd::Graph<f64>::get()->get_parameters(), 10, 0.9);

    i32       epochs     = 1;
    const f64 K          = 1.0 / 250;
    size_t    batch_size = 16384;  // Set batch size here

    std::mt19937 rng(std::random_device{}());  // Random number generator for shuffling

    for (i32 epoch = 0; epoch < epochs; epoch++) {
        // Print epoch header
        std::cout << "Epoch " << (epoch + 1) << "/" << epochs << std::endl;

        std::vector<size_t> indices(fens.size());
        std::iota(indices.begin(), indices.end(), 0);
        std::shuffle(indices.begin(), indices.end(), rng);

        size_t total_batches = (fens.size() + batch_size - 1) / batch_size;

        for (size_t batch_idx = 0, start = 0; start < fens.size();
             start += batch_size, ++batch_idx) {
            size_t end = std::min(start + batch_size, fens.size());

            std::vector<Clockwork::Autograd::ValuePtr<f64>> batch_outputs;
            std::vector<f64>                                batch_targets;

            for (size_t j = start; j < end; ++j) {
                size_t      idx = indices[j];
                std::string fen = fens[idx];
                f64         y   = results[idx];

                auto result = (tunable_function(fen) * K)->sigmoid();
                batch_outputs.push_back(result);
                batch_targets.push_back(y);
            }

            auto loss = loss_fn(batch_outputs, batch_targets);

            Clockwork::Autograd::Graph<f64>::get()->backward();
            optim.step();
            Clockwork::Autograd::Graph<f64>::get()->cleanup();

            // Print batch progress bar
            print_progress(batch_idx + 1, total_batches);
        }

        std::cout << std::endl;  // Finish progress bar line

        for (auto param : Clockwork::Autograd::Graph<f64>::get()->get_parameters()) {
            std::cout << param << std::endl;
        }
        std::cout << std::endl;
        for (auto param : Clockwork::Autograd::Graph<f64>::get()->get_pair_parameters()) {
            std::cout << param << std::endl;
        }
        std::cout << std::endl;

        if (epoch > 5) {
            optim.set_lr(optim.get_lr() * 0.97);
        }
    }
    #endif
    return 0;
}


#include "tuning/graph.hpp"
#include "tuning/loss.hpp"
#include "tuning/optim.hpp"
#include "tuning/value.hpp"
#include "uci.hpp"
#include "util/types.hpp"
#include "zobrist.hpp"
#include "position.hpp"
#include <functional>
#include <tuple>
#include <iostream>
#include <fstream>

using namespace Clockwork;

int main(int argc, char* argv[]) {

    // Initialize all necessary tables (TODO: we may need to move this to a dedicated file)
    // Zobrist::init_zobrist_keys();

    // UCI::UCIHandler uci;

    // if (argc > 1) {
    //     uci.handle_command_line(argc, argv);
    // } else {
    //     uci.loop();
    // }


    // Load fens.
    std::vector<std::string> fens;
    std::vector<f64>         results;
    std::ifstream            fenFile("fens.book");
    if (!fenFile) {
        std::cerr << "Error opening fens.book" << std::endl;
        return 1;
    };
    std::string line;
    while (std::getline(fenFile, line)) {
        // The line is : FEN;(w/d/b)
        size_t pos = line.find(';');
        if (pos != std::string::npos) {
            std::string fen = line.substr(0, pos);
            fens.push_back(fen);
            std::string result = line.substr(pos + 1);
            if (result == "w") {
                results.push_back(1.0);  // Win
            } else if (result == "d") {
                results.push_back(0.5);  // Draw
            } else if (result == "b") {
                results.push_back(0.0);  // Loss
            } else {
                std::cerr << "Invalid result in line: " << line << std::endl;
            }
        } else {
            std::cerr << "Invalid line format: " << line << std::endl;
        }
    }
    fenFile.close();

    // Print the number of fens loaded
    std::cout << "Loaded " << fens.size() << " FENs." << std::endl;

    auto PAWN_MAT = Clockwork::Autograd::Value<f64>::create_tunable(0);
    auto KNIGHT_MAT = Clockwork::Autograd::Value<f64>::create_tunable(0);
    auto BISHOP_MAT = Clockwork::Autograd::Value<f64>::create_tunable(0);
    auto ROOK_MAT = Clockwork::Autograd::Value<f64>::create_tunable(0);
    auto QUEEN_MAT = Clockwork::Autograd::Value<f64>::create_tunable(0);
    auto MOBILITY_VAL = Clockwork::Autograd::Value<f64>::create_tunable(0);
    auto TEMPO_VAL = Clockwork::Autograd::Value<f64>::create_tunable(0);

    Clockwork::Autograd::Graph<f64>::get()->register_param(PAWN_MAT);
    Clockwork::Autograd::Graph<f64>::get()->register_param(KNIGHT_MAT);
    Clockwork::Autograd::Graph<f64>::get()->register_param(BISHOP_MAT);
    Clockwork::Autograd::Graph<f64>::get()->register_param(ROOK_MAT);
    Clockwork::Autograd::Graph<f64>::get()->register_param(QUEEN_MAT);
    Clockwork::Autograd::Graph<f64>::get()->register_param(MOBILITY_VAL);
    Clockwork::Autograd::Graph<f64>::get()->register_param(TEMPO_VAL);

    auto tunable_function = [PAWN_MAT, KNIGHT_MAT, BISHOP_MAT, ROOK_MAT, QUEEN_MAT, MOBILITY_VAL, TEMPO_VAL](std::string fen) {
        auto pos = Position::parse(fen).value();

        const Color us   = pos.active_color();
        const Color them = invert(us);

        auto material =
          PAWN_MAT * static_cast<f64>(pos.piece_count(Color::White, PieceType::Pawn) - pos.piece_count(Color::Black, PieceType::Pawn))
          + KNIGHT_MAT * static_cast<f64>(pos.piece_count(Color::White, PieceType::Knight) - pos.piece_count(Color::Black, PieceType::Knight))
          + BISHOP_MAT * static_cast<f64>(pos.piece_count(Color::White, PieceType::Bishop) - pos.piece_count(Color::Black, PieceType::Bishop))
          + ROOK_MAT * static_cast<f64>(pos.piece_count(Color::White, PieceType::Rook) - pos.piece_count(Color::Black, PieceType::Rook))
          + QUEEN_MAT * static_cast<f64>(pos.piece_count(Color::White, PieceType::Queen) - pos.piece_count(Color::Black, PieceType::Queen));

        auto mobility = Clockwork::Autograd::Value<f64>::create(0);
        for (u64 x : std::bit_cast<std::array<u64, 16>>(pos.attack_table(Color::White))) {
            mobility = mobility + MOBILITY_VAL * static_cast<f64>(std::popcount(x));
        }
        for (u64 x : std::bit_cast<std::array<u64, 16>>(pos.attack_table(Color::Black))) {
            mobility = mobility - MOBILITY_VAL * static_cast<f64>(std::popcount(x));
        }

        auto tempo = (us == Color::White)? TEMPO_VAL : -TEMPO_VAL;

        return material + mobility + tempo;
    };

    auto                          loss_fn = Clockwork::Autograd::mse<f64>;
    Clockwork::Autograd::SGD<f64> optim(Clockwork::Autograd::Graph<f64>::get()->get_parameters(), 0.1, 0.9);

    i32 epochs = 10;
    const f64 K = 1.0 / 650;
    for (i32 epoch = 0; epoch < epochs; epoch++) {
        for (size_t i = 0; i < fens.size(); i++){
            std::string fen      = fens[i];
            f64 y      = results[i];

            auto result = (tunable_function(fen) * K)->sigmoid();

            auto loss = loss_fn(std::vector({result}), std::vector({y}));

            Clockwork::Autograd::Graph<f64>::get()->backward();
            
            optim.step();

            for (auto param : Clockwork::Autograd::Graph<f64>::get()->get_parameters()) {
                std::cout << param << std::endl;
            }
            std::cout << std::endl;

            Clockwork::Autograd::Graph<f64>::get()->cleanup();
        }
        
    }

    return 0;
}

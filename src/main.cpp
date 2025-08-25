
#include "eval_types.hpp"
#include "evaluation.hpp"
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
    std::vector<Position> positions;
    std::vector<f64>      results;
    std::ifstream         fenFile("data/filtered_positions.fen");
    if (!fenFile) {
        std::cerr << "Error opening fens.book" << std::endl;
        return 1;
    };
    std::string line;
    while (std::getline(fenFile, line)) {
        size_t pos = line.find(';');
        if (pos != std::string::npos) {
            std::string fen = line.substr(0, pos);
            positions.push_back(Position::parse(fen).value());
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

    // Print the number of positions loaded
    std::cout << "Loaded " << positions.size() << " FENs." << std::endl;

    Clockwork::Autograd::AdamW optim(10, 0.9, 0.999, 1e-8, 0.0);

    i32       epochs     = 10;
    const f64 K          = 1.0 / 400;
    size_t    batch_size = 16384;  // Set batch size here

    std::mt19937 rng(std::random_device{}());  // Random number generator for shuffling

    for (i32 epoch = 0; epoch < epochs; epoch++) {
        // Print epoch header
        std::cout << "Epoch " << (epoch + 1) << "/" << epochs << std::endl;

        std::vector<size_t> indices(positions.size());
        std::iota(indices.begin(), indices.end(), 0);
        std::shuffle(indices.begin(), indices.end(), rng);

        size_t total_batches = (positions.size() + batch_size - 1) / batch_size;

        for (size_t batch_idx = 0, start = 0; start < positions.size();
             start += batch_size, ++batch_idx) {
            size_t end = std::min(start + batch_size, positions.size());

            std::vector<Clockwork::Autograd::ValuePtr> batch_outputs;
            std::vector<f64>                           batch_targets;
            batch_outputs.reserve(end - start);
            batch_targets.reserve(end - start);

            for (size_t j = start; j < end; ++j) {
                size_t   idx    = indices[j];
                f64      y      = results[idx];
                Position pos    = positions[idx];
                auto     result = (evaluate(pos) * K)->sigmoid();
                batch_outputs.push_back(result);
                batch_targets.push_back(y);
            }

            auto loss = Clockwork::Autograd::mse(batch_outputs, batch_targets);

            Clockwork::Autograd::Graph::get()->backward();
            optim.step();
            Clockwork::Autograd::Graph::get()->cleanup();

            // Print batch progress bar
            print_progress(batch_idx + 1, total_batches);
        }

        std::cout << std::endl;  // Finish progress bar line

        for (auto param : Clockwork::Autograd::Graph::get()->get_parameters()) {
            std::cout << param << std::endl;
        }
        std::cout << std::endl;
        for (auto param : Clockwork::Autograd::Graph::get()->get_pair_parameters()) {
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

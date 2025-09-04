#include "eval_types.hpp"
#include "evaluation.hpp"
#include "position.hpp"
#include "tuning/graph.hpp"
#include "tuning/loss.hpp"
#include "tuning/optim.hpp"
#include "tuning/value.hpp"
#include "util/pretty.hpp"
#include "util/types.hpp"
#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <numeric>
#include <random>
#include <tuple>

using namespace Clockwork;

int main(int argc, char* argv[]) {

    // Load fens from multiple files.
    std::vector<Position> positions;
    std::vector<f64>      results;

    // List of files to load
    std::vector<std::string> fenFiles = {"data/v2_filtered/gioviok_sampled_positions_337763.txt",
                                         "data/v2_filtered/giovivast_sampled_positions_2568565.txt",
                                         "data/v2_filtered/micpillar_sampled_positions_25219.txt",
                                         "data/v2_filtered/styx_sampled_positions_2101270.txt"};

    for (const auto& filename : fenFiles) {
        std::ifstream fenFile(filename);
        if (!fenFile) {
            std::cerr << "Error opening " << filename << std::endl;
            continue;  // skip to the next file
        }

        std::string line;
        while (std::getline(fenFile, line)) {
            size_t pos = line.find(';');
            if (pos != std::string::npos) {
                std::string fen    = line.substr(0, pos);
                auto        parsed = Position::parse(fen);
                if (parsed) {
                    positions.push_back(*parsed);
                } else {
                    std::cerr << "Failed to parse FEN in file " << filename << ": " << fen
                              << std::endl;
                    continue;
                }

                std::string result = line.substr(pos + 1);
                result.erase(std::remove_if(result.begin(), result.end(), ::isspace), result.end());

                if (result == "w") {
                    results.push_back(1.0);
                } else if (result == "d") {
                    results.push_back(0.5);
                } else if (result == "b") {
                    results.push_back(0.0);
                } else {
                    std::cerr << "Invalid result in file " << filename << " line: " << line
                              << " (result is '" << result << "')" << std::endl;
                }
            } else {
                std::cerr << "Invalid line format in " << filename << ": " << line << std::endl;
            }
        }

        fenFile.close();
    }

    // Print the number of positions loaded
    std::cout << "Loaded " << positions.size() << " FENs from " << fenFiles.size() << " files."
              << std::endl;

    Clockwork::Autograd::AdamW optim(10, 0.9, 0.999, 1e-8, 0.0);

    i32       epochs     = 1000;
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
                auto     result = (evaluate_white_pov(pos) * K)->sigmoid();
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

        std::cout << "Pawn mat " << PAWN_MAT << std::endl;
        std::cout << "Knight mat " << KNIGHT_MAT << std::endl;
        std::cout << "Bishop mat " << BISHOP_MAT << std::endl;
        std::cout << "Rook mat " << ROOK_MAT << std::endl;
        std::cout << "Queen mat " << QUEEN_MAT << std::endl;

        std::cout << "Mobility " << MOBILITY_VAL << std::endl;
        std::cout << "Tempo " << TEMPO_VAL << std::endl;

        std::cout << "Bishop pair " << BISHOP_PAIR_VAL << std::endl;
        std::cout << "Doubled pawns " << DOUBLED_PAWN_VAL << std::endl;

        auto printPsqtArray = [](const auto& arr) {
            for (std::size_t i = 0; i < arr.size(); ++i) {
                std::cout << arr[i] << ",\t";
                if ((i & 7) == 7) {
                    std::cout << std::endl;
                }
            }
            std::cout << std::endl;
        };

        printPsqtArray(PAWN_PSQT);
        printPsqtArray(KNIGHT_PSQT);
        printPsqtArray(BISHOP_PSQT);
        printPsqtArray(ROOK_PSQT);
        printPsqtArray(QUEEN_PSQT);
        printPsqtArray(KING_PSQT);

        if (epoch > 5) {
            optim.set_lr(optim.get_lr() * 0.85);
        }
    }

    return 0;
}

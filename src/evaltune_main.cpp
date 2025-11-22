#include "eval_constants.hpp"
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
#include <barrier>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <numeric>
#include <random>
#include <sstream>
#include <thread>
#include <tuple>

using namespace Clockwork;
using namespace Clockwork::Autograd;

int main() {

    std::vector<Position> positions;
    std::vector<f64>      results;

    const std::vector<std::string> fenFiles = {
      "data/dfrcv1.txt", "data/dfrcv0.txt", "data/v2.2.txt", "data/v2.1.txt", "data/v3.txt",
    };

    const u32 thread_count = std::max<u32>(1, std::thread::hardware_concurrency() / 2);

    std::cout << "Running on " << thread_count << " threads\n";

    for (const auto& filename : fenFiles) {
        std::ifstream fenFile(filename);
        if (!fenFile) {
            std::cerr << "Error opening " << filename << "\n";
            return 1;
        }

        std::string line;
        while (std::getline(fenFile, line)) {
            size_t pos = line.find(';');
            if (pos == std::string::npos) {
                std::cerr << "Bad line in " << filename << ": " << line << "\n";
                continue;
            }

            std::string fen    = line.substr(0, pos);
            auto        parsed = Position::parse(fen);

            if (!parsed) {
                std::cerr << "Failed to parse FEN in " << filename << ": " << fen << "\n";
                continue;
            }

            positions.push_back(*parsed);

            std::string result = line.substr(pos + 1);
            result.erase(std::remove_if(result.begin(), result.end(), ::isspace), result.end());

            if (result == "w") {
                results.push_back(1.0);
            } else if (result == "d") {
                results.push_back(0.5);
            } else if (result == "b") {
                results.push_back(0.0);
            } else {
                std::cerr << "Invalid result in " << filename << ": " << line << "\n";
            }
        }
    }

    std::cout << "Loaded " << positions.size() << " FENs.\n";
    if (positions.empty()) {
        return 1;
    }

    // Setup tuning
    const ParameterCountInfo parameter_count = Globals::get().get_parameter_counts();

    // This line loads the defaults from your S() macros
    Parameters current_parameter_values = Graph::get().get_all_parameter_values();

    // Uncomment for zero tune: Overwrite them all with zeros.
    current_parameter_values = Parameters::zeros(parameter_count);

    // The optimizer will now start with all-zero parameters
    AdamW optim(parameter_count, 10, 0.9, 0.999, 1e-8, 0.0);

    const i32    epochs     = 8;
    const f64    K          = 1.0 / 400;
    const size_t batch_size = 16 * 16384;

    std::mt19937        rng(std::random_device{}());
    std::vector<size_t> indices(positions.size());
    // Initialize indices 1..N
    std::iota(indices.begin(), indices.end(), 0);
    const size_t total_batches = (positions.size() + batch_size - 1) / batch_size;

    // Shared gradient accumulator
    Parameters batch_gradients = Parameters::zeros(parameter_count);

    std::mutex mutex;

    std::barrier epoch_barrier{thread_count + 1};
    std::barrier batch_barrier{thread_count + 1, [&]() noexcept {
                                   // Single-thread optimizer update
                                   optim.step(current_parameter_values, batch_gradients);
                                   batch_gradients = Parameters::zeros(parameter_count);
                               }};

    // Spawn worker threads
    for (u32 t = 0; t < thread_count; ++t) {
        std::thread([&, t]() {
            // Each thread uses its own Graph arena
            for (int epoch = 0; epoch < epochs; ++epoch) {

                epoch_barrier.arrive_and_wait();

                for (size_t batch_start = 0; batch_start < positions.size();
                     batch_start += batch_size) {
                    size_t batch_end       = std::min(batch_start + batch_size, positions.size());
                    size_t this_batch_size = batch_end - batch_start;

                    size_t sub_size = (this_batch_size + thread_count - 1) / thread_count;

                    size_t sub_start = batch_start + sub_size * t;
                    size_t sub_end   = std::min(sub_start + sub_size, batch_end);

                    Graph::get().copy_parameter_values(current_parameter_values);

                    std::vector<ValueHandle> outputs;
                    std::vector<f64>         targets;
                    outputs.reserve(sub_end - sub_start);
                    targets.reserve(sub_end - sub_start);

                    // Forward
                    for (size_t j = sub_start; j < sub_end; ++j) {
                        size_t idx = indices[j];

                        auto        y = results[idx];
                        ValueHandle v = (evaluate_white_pov(positions[idx]) * K).sigmoid();
                        outputs.push_back(v);
                        targets.push_back(y);
                    }

                    // Backward
                    ValueHandle loss = mse<f64, Reduction::Sum>(outputs, targets)
                                     * ValueHandle::create(1.0 / double(this_batch_size));

                    Graph::get().backward();

                    Parameters grads = Graph::get().get_all_parameter_gradients();

                    // Accumulate
                    {
                        std::lock_guard guard(mutex);
                        batch_gradients.accumulate(grads);
                    }

                    Graph::get().cleanup();
                    Graph::get().zero_grad();

                    batch_barrier.arrive_and_wait();
                }
            }
        }).detach();
    }

    // Epoch loop
    for (int epoch = 0; epoch < epochs; ++epoch) {

        std::cout << "Epoch " << epoch + 1 << "/" << epochs << "\n";

        const auto start = time::Clock::now();

        std::shuffle(indices.begin(), indices.end(), rng);

        epoch_barrier.arrive_and_wait();

        for (size_t bi = 0, bstart = 0; bstart < positions.size(); bstart += batch_size, ++bi) {
            batch_barrier.arrive_and_wait();
            print_progress(bi + 1, total_batches);
        }

        std::cout << "\n";

        // Dump current parameter values
        Graph::get().copy_parameter_values(current_parameter_values);

        Graph::get().cleanup();
        Graph::get().zero_grad();
#define PROFILE_RUN
#ifndef PROFILE_RUN
        std::cout << "inline const PParam PAWN_MAT   = " << PAWN_MAT << ";" << std::endl;
        std::cout << "inline const PParam KNIGHT_MAT = " << KNIGHT_MAT << ";" << std::endl;
        std::cout << "inline const PParam BISHOP_MAT = " << BISHOP_MAT << ";" << std::endl;
        std::cout << "inline const PParam ROOK_MAT   = " << ROOK_MAT << ";" << std::endl;
        std::cout << "inline const PParam QUEEN_MAT  = " << QUEEN_MAT << ";" << std::endl;
        std::cout << "inline const PParam TEMPO_VAL  = " << TEMPO_VAL << ";" << std::endl;
        std::cout << std::endl;

        std::cout << "inline const PParam BISHOP_PAIR_VAL   = " << BISHOP_PAIR_VAL << ";"
                  << std::endl;
        std::cout << "inline const PParam ROOK_OPEN_VAL     = " << ROOK_OPEN_VAL << ";"
                  << std::endl;
        std::cout << "inline const PParam ROOK_SEMIOPEN_VAL = " << ROOK_SEMIOPEN_VAL << ";"
                  << std::endl;
        std::cout << std::endl;
        std::cout << "inline const PParam DOUBLED_PAWN_VAL = " << DOUBLED_PAWN_VAL << ";"
                  << std::endl;
        std::cout << std::endl;

        std::cout << "inline const PParam POTENTIAL_CHECKER_VAL = " << POTENTIAL_CHECKER_VAL << ";"
                  << std::endl;

        std::cout << "inline const PParam OUTPOST_KNIGHT_VAL    = " << OUTPOST_KNIGHT_VAL << ";"
                  << std::endl;
        std::cout << "inline const PParam OUTPOST_BISHOP_VAL    = " << OUTPOST_BISHOP_VAL << ";"
                  << std::endl;

        std::cout << std::endl;

        std::cout << "inline const PParam PAWN_PUSH_THREAT_KNIGHT = " << PAWN_PUSH_THREAT_KNIGHT
                  << ";" << std::endl;
        std::cout << "inline const PParam PAWN_PUSH_THREAT_BISHOP = " << PAWN_PUSH_THREAT_BISHOP
                  << ";" << std::endl;
        std::cout << "inline const PParam PAWN_PUSH_THREAT_ROOK   = " << PAWN_PUSH_THREAT_ROOK
                  << ";" << std::endl;
        std::cout << "inline const PParam PAWN_PUSH_THREAT_QUEEN  = " << PAWN_PUSH_THREAT_QUEEN
                  << ";" << std::endl;
        std::cout << std::endl;

        auto print_table = [](const std::string& name, const auto& table) {
            std::cout << "inline const std::array<PParam, " << table.size() << "> " << name
                      << " = {" << std::endl
                      << "   ";
            for (const auto& val : table) {
                std::cout << " " << val << ",";
            }
            std::cout << std::endl << "};" << std::endl;
        };

        print_table("PAWN_PHALANX", PAWN_PHALANX);
        print_table("DEFENDED_PAWN", DEFENDED_PAWN);
        print_table("PASSED_PAWN", PASSED_PAWN);
        print_table("DEFENDED_PASSED_PUSH", DEFENDED_PASSED_PUSH);
        print_table("BLOCKED_PASSED_PAWN", BLOCKED_PASSED_PAWN);
        std::cout << std::endl;

        print_table("FRIENDLY_KING_PASSED_PAWN_DISTANCE", FRIENDLY_KING_PASSED_PAWN_DISTANCE);
        print_table("ENEMY_KING_PASSED_PAWN_DISTANCE", ENEMY_KING_PASSED_PAWN_DISTANCE);
        std::cout << std::endl;

        print_table("KNIGHT_MOBILITY", KNIGHT_MOBILITY);
        print_table("BISHOP_MOBILITY", BISHOP_MOBILITY);
        print_table("ROOK_MOBILITY", ROOK_MOBILITY);
        print_table("QUEEN_MOBILITY", QUEEN_MOBILITY);
        print_table("KING_MOBILITY", KING_MOBILITY);
        std::cout << std::endl;

        print_table("KNIGHT_KING_RING", KNIGHT_KING_RING);
        print_table("BISHOP_KING_RING", BISHOP_KING_RING);
        print_table("ROOK_KING_RING", ROOK_KING_RING);
        print_table("QUEEN_KING_RING", QUEEN_KING_RING);
        std::cout << std::endl;

        std::cout << "inline const PParam PAWN_THREAT_KNIGHT = " << PAWN_THREAT_KNIGHT << ";"
                  << std::endl;
        std::cout << "inline const PParam PAWN_THREAT_BISHOP = " << PAWN_THREAT_BISHOP << ";"
                  << std::endl;
        std::cout << "inline const PParam PAWN_THREAT_ROOK   = " << PAWN_THREAT_ROOK << ";"
                  << std::endl;
        std::cout << "inline const PParam PAWN_THREAT_QUEEN  = " << PAWN_THREAT_QUEEN << ";"
                  << std::endl;
        std::cout << std::endl;

        std::cout << "inline const PParam KNIGHT_THREAT_BISHOP = " << KNIGHT_THREAT_BISHOP << ";"
                  << std::endl;
        std::cout << "inline const PParam KNIGHT_THREAT_ROOK   = " << KNIGHT_THREAT_ROOK << ";"
                  << std::endl;
        std::cout << "inline const PParam KNIGHT_THREAT_QUEEN  = " << KNIGHT_THREAT_QUEEN << ";"
                  << std::endl;
        std::cout << std::endl;

        std::cout << "inline const PParam BISHOP_THREAT_KNIGHT = " << BISHOP_THREAT_KNIGHT << ";"
                  << std::endl;
        std::cout << "inline const PParam BISHOP_THREAT_ROOK   = " << BISHOP_THREAT_ROOK << ";"
                  << std::endl;
        std::cout << "inline const PParam BISHOP_THREAT_QUEEN  = " << BISHOP_THREAT_QUEEN << ";"
                  << std::endl;
        std::cout << std::endl;

        print_table("BISHOP_PAWNS", BISHOP_PAWNS);
        std::cout << std::endl;

        auto printPsqtArray = [](const std::string& name, const auto& arr) {
            std::cout << "inline const std::array<PParam, " << arr.size() << "> " << name << " = {"
                      << std::endl;
            for (std::size_t i = 0; i < arr.size(); ++i) {
                if ((i & 7) == 0) {
                    std::cout << "    ";
                }
                std::stringstream ss;
                ss << arr[i] << ",";
                std::cout << std::left << std::setw(16) << ss.str();
                if ((i & 7) == 7) {
                    std::cout << "//" << std::endl;
                }
            }
            std::cout << "};" << std::endl;
        };

        printPsqtArray("PAWN_PSQT", PAWN_PSQT);
        printPsqtArray("KNIGHT_PSQT", KNIGHT_PSQT);
        printPsqtArray("BISHOP_PSQT", BISHOP_PSQT);
        printPsqtArray("ROOK_PSQT", ROOK_PSQT);
        printPsqtArray("QUEEN_PSQT", QUEEN_PSQT);
        printPsqtArray("KING_PSQT", KING_PSQT);
        std::cout << std::endl;
#endif
        const auto end = time::Clock::now();
        std::cout << "// Epoch duration: " << time::cast<time::FloatSeconds>(end - start).count()
                  << "s\n";

        if (epoch > 5) {
            optim.set_lr(optim.get_lr() * 0.91);
        }
    }

    return 0;
}


#include "tuning/loss.hpp"
#include "tuning/optim.hpp"
#include "tuning/value.hpp" 
#include "tuning/graph.hpp"
#include "uci.hpp"
#include "util/types.hpp"
#include "zobrist.hpp"
#include <functional>
#include <tuple>

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
    // std::vector<std::string> fens;
    // std::vector<f64>         results;
    // std::ifstream            fenFile("fens.book");
    // if (!fenFile) {
    //     std::cerr << "Error opening fens.book" << std::endl;
    //     return 1;
    // };
    // std::string line;
    // while (std::getline(fenFile, line)) {
    //     // The line is : FEN;(w/d/b)
    //     size_t pos = line.find(';');
    //     if (pos != std::string::npos) {
    //         std::string fen = line.substr(0, pos);
    //         fens.push_back(fen);
    //         std::string result = line.substr(pos + 1);
    //         if (result == "w") {
    //             results.push_back(1.0);  // Win
    //         } else if (result == "d") {
    //             results.push_back(0.5);  // Draw
    //         } else if (result == "b") {
    //             results.push_back(0.0);  // Loss
    //         } else {
    //             std::cerr << "Invalid result in line: " << line << std::endl;
    //         }
    //     } else {
    //         std::cerr << "Invalid line format: " << line << std::endl;
    //     }
    // }
    // fenFile.close();

    // // Print the number of fens loaded
    // std::cout << "Loaded " << fens.size() << " FENs." << std::endl;

    auto a = Clockwork::Autograd::Value<f64>::create(0);
    auto b = Clockwork::Autograd::Value<f64>::create(0);
    auto c = Clockwork::Autograd::Value<f64>::create(0);
    auto d = Clockwork::Autograd::Value<f64>::create(0);

    Clockwork::Autograd::Graph<f64>::get()->register_param(a);
    Clockwork::Autograd::Graph<f64>::get()->register_param(b);
    Clockwork::Autograd::Graph<f64>::get()->register_param(c);
    Clockwork::Autograd::Graph<f64>::get()->register_param(d);

    auto tunable_function = [a, b, c, d](f64 x, f64 y) {
        return std::pow(x, 2) * a + x * y * b + y * c + d;
    };

    auto loss_fn = Clockwork::Autograd::mse<f64>;
    Clockwork::Autograd::SGD<f64>  optim(Clockwork::Autograd::Graph<f64>::get()->get_parameters(), 0.1, 0.9);

    std::vector<std::tuple<f64, f64, f64>> data;
    const f64                              true_a = 3;
    const f64                              true_b = -2;
    const f64                              true_c = 1;
    const f64                              true_d = 5;

    // Simple 2D grid sampling
    for (f64 x = -5.0; x <= 5.0; x += 1.0) {
        for (f64 y = -5.0; y <= 5.0; y += 1.0) {
            f64 z = true_a * x * x + true_b * x * y + true_c * y + true_d;
            data.emplace_back(x, y, z);
        }
    }

    i32 epochs = 10;

    for (i32 epoch = 0; epoch < epochs; epoch++) {
        for (auto datapoint : data) {
            f64 x      = std::get<0>(datapoint);
            f64 y      = std::get<1>(datapoint);
            f64 target = std::get<2>(datapoint);

            auto result = tunable_function(x, y);

            auto loss = loss_fn(std::vector({result}), std::vector({target}));

            Clockwork::Autograd::Graph<f64>::get()->backward();
            optim.step();
            Clockwork::Autograd::Graph<f64>::get()->cleanup();
        }
        std::cout << "Values: " << a << " " << b << " " << c << " " << d << std::endl;
    }

    return 0;
}

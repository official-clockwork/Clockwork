#include "tuning/graph.hpp"
#include "tuning/loss.hpp"
#include "tuning/value.hpp"
#include "uci.hpp"
#include "zobrist.hpp"

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


    return 0;
}

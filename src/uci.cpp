#include "uci.hpp"
#include "move.hpp"

#include <ios>
#include <iostream>
#include <istream>
#include <sstream>
#include <string>

namespace Clockwork::UCI {

void UCIHandler::loop() {
    std::string input;

    while (std::getline(std::cin, input))
        execute_command(input);
}

void UCIHandler::handle_command_line(int argc, char* argv[]) {
    for (int i = 1; i < argc; ++i)
        execute_command(argv[i]);
}

void UCIHandler::execute_command(const std::string& line) {
    std::istringstream is{line};

    std::string command;
    is >> std::skipws >> command;

    if (command == "uci") {
        std::cout << "id Name Clockwork\n";
        std::cout << "id author The Clockwork community" << std::endl;
    }
    else if (command == "isready")
        std::cout << "readyok" << std::endl;
    else if (command == "quit")
        std::exit(0);
    else if (command == "go")
        handle_go(is);
    else if (command == "position")
        handle_position(is);
    else
        std::cout << "Unknown command" << std::endl;
}

void UCIHandler::handle_go(std::istringstream& is) {
    std::string token;
    while (is >> token) {
        if (token == "depth") {
            is >> settings.depth;
        }
        else if (token == "movetime") {
            is >> settings.move_time;
        }
        else if (token == "wtime") {
            is >> settings.w_time;
        }
        else if (token == "btime") {
            is >> settings.b_time;
        }
        else if (token == "winc") {
            is >> settings.w_inc;
        }
        else if (token == "binc") {
            is >> settings.b_inc;
        }
    }
}

void UCIHandler::handle_position(std::istringstream& is) {
    std::string token;
    if (is >> token) {
        if (token == "startpos") {}
        else if (token == "fen") {}
    }
}

}

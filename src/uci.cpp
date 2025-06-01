#include "uci.hpp"

#include <ios>
#include <iostream>
#include <istream>
#include <sstream>
#include <string>

namespace Clockwork::UCI {

void UCIHandler::loop() {
    std::string input;

    while (std::getline(std::cin, input))
        executeCommand(input);
}

void UCIHandler::handleCommandLine(int argc, char* argv[]) {
    for (int i = 1; i < argc; ++i)
        executeCommand(argv[i]);
}

void UCIHandler::executeCommand(std::string line) {
    std::istringstream is{line};

    std::string command;
    is >> std::skipws >> command;

    if (command == "uci")
    {
        std::cout << "id Name Clockwork\n";
        std::cout << "id author The Clockwork community" << std::endl;
    }
    else if (command == "isready")
        std::cout << "readyok" << std::endl;
    else if (command == "quit")
        std::exit(0);
    else
        std::cout << "Unknown command" << std::endl;
}

}

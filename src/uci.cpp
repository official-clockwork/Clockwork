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
        executeCommand(input);
}

void UCIHandler::handleCommandLine(int argc, char* argv[]) {
    for (int i = 1; i < argc; ++i)
        executeCommand(argv[i]);
}

void UCIHandler::executeCommand(const std::string& line) {
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
    else if (command == "go")
        handleGo(is);
    else if (command == "position")
        handlePosition(is);
    else
        std::cout << "Unknown command" << std::endl;
}

void UCIHandler::handleGo(std::istringstream& is) {
    std::string token;
    while (is >> token)
    {
        if (token == "depth")
        {
            is >> settings.depth;
        }
        else if (token == "movetime")
        {
            is >> settings.moveTime;
        }
        else if (token == "wtime")
        {
            is >> settings.wTime;
        }
        else if (token == "btime")
        {
            is >> settings.bTime;
        }
        else if (token == "winc")
        {
            is >> settings.wInc;
        }
        else if (token == "binc")
        {
            is >> settings.bInc;
        }
    }
}

void UCIHandler::handlePosition(std::istringstream& is) {
    std::string token;
    if (is >> token)
    {
        if (token == "startpos")
        {
        }
        else if (token == "fen")
        {
        }
    }
}

}

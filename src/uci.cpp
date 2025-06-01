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
            int depth;
            is >> depth;
            settings.depth = depth;
        }
        else if (token == "movetime")
        {
            int moveTime;
            is >> moveTime;
            settings.moveTime = moveTime;
        }
        else if (token == "wtime")
        {
            int wTime;
            is >> wTime;
            settings.wTime = wTime;
        }
        else if (token == "btime")
        {
            int bTime;
            is >> bTime;
            settings.bTime = bTime;
        }
        else if (token == "winc")
        {
            int wInc;
            is >> wInc;
            settings.wInc = wInc;
        }
        else if (token == "binc")
        {
            int bInc;
            is >> bInc;
            settings.bInc = bInc;
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

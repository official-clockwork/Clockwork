#pragma once

#include <string>
#include <vector>

#include "common.hpp"
#include "position.hpp"
#include "repetition_info.hpp"
#include "search.hpp"
#include "tt.hpp"

namespace Clockwork::UCI {

class UCIHandler {
public:
    UCIHandler();

    void loop();
    void handle_command_line(i32 argc, char* argv[]);

private:
    Position       m_position;
    RepetitionInfo m_repetition_info;

    Search::Searcher searcher;

    Search::SearchSettings settings;

    void execute_command(const std::string&);
    void handle_bench(std::istringstream&);
    void handle_go(std::istringstream&);
    void handle_position(std::istringstream&);
    void handle_setoption(std::istringstream&);
    void handle_d(std::istringstream&);
    void handle_attacks(std::istringstream&);
    void handle_perft(std::istringstream&);
};

}

#pragma once

#include <string>
#include <vector>

#include "position.hpp"

namespace Clockwork::UCI {

struct SearchSettings {
    i32 depth     = 0;
    i32 w_time    = 0;
    i32 b_time    = 0;
    i32 w_inc     = 0;
    i32 b_inc     = 0;
    i32 move_time = 0;
};

class UCIHandler {
public:
    UCIHandler();

    void loop();
    void handle_command_line(i32 argc, char* argv[]);

private:
    Position m_position;

    SearchSettings settings;

    void execute_command(const std::string&);
    void handle_bench(std::istringstream&);
    void handle_go(std::istringstream&);
    void handle_position(std::istringstream&);
    void handle_attacks(std::istringstream&);
    void handle_perft(std::istringstream&);
};

}

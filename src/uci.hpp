#pragma once

#include <string>
#include <vector>

#include "common.hpp"
#include "position.hpp"
#include "repetition_info.hpp"
#include "tt.hpp"

namespace Clockwork::UCI {

struct SearchSettings {
    Color stm        = Color::White;
    i32   depth      = 0;
    i64   w_time     = -1;
    i64   b_time     = -1;
    i64   w_inc      = -1;
    i64   b_inc      = -1;
    i64   move_time  = -1;
    u64   hard_nodes = 0;
    u64   soft_nodes = 0;
};

class UCIHandler {
public:
    UCIHandler();

    void loop();
    void handle_command_line(i32 argc, char* argv[]);

private:
    Position       m_position;
    RepetitionInfo m_repetition_info;
    // move this somewhere else later
    TT m_tt;

    SearchSettings settings;

    void execute_command(const std::string&);
    void handle_bench(std::istringstream&);
    void handle_go(std::istringstream&);
    void handle_position(std::istringstream&);
    void handle_setoption(std::istringstream&);
    void handle_attacks(std::istringstream&);
    void handle_perft(std::istringstream&);
};

}

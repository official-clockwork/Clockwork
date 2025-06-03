#pragma once

#include <string>

namespace Clockwork::UCI {

struct SearchSettings {
    int depth     = 0;
    int w_time    = 0;
    int b_time    = 0;
    int w_inc     = 0;
    int b_inc     = 0;
    int move_time = 0;
};

class UCIHandler {
public:
    void loop();
    void handle_command_line(int argc, char* argv[]);

private:
    SearchSettings settings;
    void           execute_command(const std::string&);
    void           handle_go(std::istringstream&);
    void           handle_position(std::istringstream&);
};

}

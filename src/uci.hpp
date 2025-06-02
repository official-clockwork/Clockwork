#pragma once

#include <string>

namespace Clockwork::UCI {

struct SearchSettings {
    int depth = 0;
    int wTime = 0;
    int bTime = 0;
    int wInc = 0;
    int bInc = 0;
    int moveTime = 0;
};

class UCIHandler { 
   public:
    void loop();
    void handle_command_line(int argc, char* argv[]);

   private:
    SearchSettings settings;
    void execute_command(const std::string&);
    void handle_go(std::istringstream&);
    void handle_position(std::istringstream&);
};

}

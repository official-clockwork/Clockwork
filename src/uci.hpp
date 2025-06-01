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
    void handleCommandLine(int argc, char* argv[]);

   private:
    SearchSettings settings;
    void executeCommand(const std::string&);
    void handleGo(std::istringstream&);
    void handlePosition(std::istringstream&);
};

}

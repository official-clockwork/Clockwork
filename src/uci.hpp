#pragma once

#include <string>

namespace Clockwork::UCI {

class UCIHandler {
   public:
    void loop();
    void handleCommandLine(int argc, char* argv[]);

   private:
    void executeCommand(std::string);
    void handleGo(std::istringstream&);
    void handlePosition(std::istringstream&);
};

}

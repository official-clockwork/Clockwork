#include "uci.hpp"

using namespace Clockwork;

int main(int argc, char* argv[]) {
    UCI::UCIHandler uci;

    if (argc > 1) {
        uci.handle_command_line(argc, argv);
    } else {
        uci.loop();
    }

    return 0;
}

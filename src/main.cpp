#include "cuckoo.hpp"
#include "uci.hpp"
#include "zobrist.hpp"

using namespace Clockwork;

int main(int argc, char* argv[]) {

    // Initialize all necessary tables (TODO: we may need to move this to a dedicated file)
    Zobrist::init_zobrist_keys();
    Cuckoo::init();

    UCI::UCIHandler uci;

    if (argc > 1) {
        uci.handle_command_line(argc, argv);
    } else {
        uci.loop();
    }

    return 0;
}

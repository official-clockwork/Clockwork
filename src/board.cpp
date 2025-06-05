#include "board.hpp"

#include <iomanip>
#include <iostream>

#include "square.hpp"

namespace Clockwork {

std::ostream& operator<<(std::ostream& os, const Wordboard& at) {
    std::ios state{nullptr};
    state.copyfmt(os);

    for (i32 rank = 7; rank >= 0; rank--) {
        for (i32 file = 0; file < 8; file++) {
            Square sq    = Square::from_file_and_rank(file, rank);
            u16    value = at.read(sq);

            os << std::hex << std::setfill('0') << std::setw(4) << value << ' ';
        }
        os << std::endl;
    }

    os.copyfmt(state);
    return os;
}

}

#include "board.hpp"

#include <iostream>

#include "square.hpp"
#include "util/ios_fmt_guard.hpp"

namespace Clockwork {

std::ostream& operator<<(std::ostream& os, const Wordboard& at) {
    IosFmtGuard guard{os};

    for (i32 rank = 7; rank >= 0; rank--) {
        for (i32 file = 0; file < 8; file++) {
            Square    sq    = Square::from_file_and_rank(file, rank);
            PieceMask value = at.read(sq);

            os << std::hex << std::setfill('0') << std::setw(4) << value.value() << ' ';
        }
        os << std::endl;
    }

    return os;
}

}  // namespace Clockwork

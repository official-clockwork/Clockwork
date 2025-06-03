#include "board.hpp"

#include <iomanip>
#include <iostream>

#include "square.hpp"

namespace Clockwork {

std::ostream& operator<<(std::ostream& os, const Wordboard& at) {
    std::ios state{nullptr};
    state.copyfmt(os);

    for (int rank = 7; rank >= 0; rank--)
    {
        for (int file = 0; file < 8; file++)
        {
            const Square sq    = Square::fromFileAndRank(file, rank);
            const u16    value = at[sq];

            os << std::hex << std::setfill('0') << std::setw(4) << value << ' ';
        }
        os << std::endl;
    }

    os.copyfmt(state);
    return os;
}

}

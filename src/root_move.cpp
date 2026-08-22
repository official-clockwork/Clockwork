#include "root_move.hpp"
#include <ostream>

namespace Clockwork::Search {

std::ostream& operator<<(std::ostream& os, const PV& pv) {
    for (Move m : pv.m_pv) {
        os << m << ' ';
    }
    return os;
}

}

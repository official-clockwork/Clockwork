#include "search.hpp"
#include "speedtest.hpp"
#include "zobrist.hpp"

using namespace Clockwork;

int main() {
    Zobrist::init_zobrist_keys();
    Search::Searcher searcher;
    Speedtest::speedtest(searcher);
    return 0;
}

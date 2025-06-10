#include <iomanip>
#include <iostream>

#include "position.hpp"
#include "repetition_info.hpp"
#include "test.hpp"
#include "zobrist.hpp"

using namespace Clockwork;

static Position       g_position;
static RepetitionInfo g_repetition_info;

void move(std::string_view movestr) {
    Move move  = Move::parse(movestr, g_position).value();
    g_position = g_position.move(move);
    g_repetition_info.push(g_position.get_hash_key(), g_position.is_reversible(move));
    std::cout << "move: " << movestr << " " << std::hex << std::setw(16)
              << g_position.get_hash_key() << std::endl;
}

void repeat_in_history() {
    std::cout << "repeat_in_history" << std::endl;
    g_position =
      Position::parse("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1").value();
    g_repetition_info.reset();
    g_repetition_info.push(g_position.get_hash_key(), false);
    std::cout << "startpos: " << std::hex << std::setw(16) << g_position.get_hash_key()
              << std::endl;
    REQUIRE(g_repetition_info.detect_repetition(0) == false);
    move("g1f3");
    REQUIRE(g_repetition_info.detect_repetition(0) == false);
    move("g8f6");
    REQUIRE(g_repetition_info.detect_repetition(0) == false);
    move("f3g1");
    REQUIRE(g_repetition_info.detect_repetition(0) == false);
    move("f6g8");
    REQUIRE(g_repetition_info.detect_repetition(0) == false);
    move("g1f3");
    REQUIRE(g_repetition_info.detect_repetition(0) == false);
    move("g8f6");
    REQUIRE(g_repetition_info.detect_repetition(0) == false);
    move("f3g1");
    REQUIRE(g_repetition_info.detect_repetition(0) == false);
    move("f6g8");
    REQUIRE(g_repetition_info.detect_repetition(0) == true);
}

void repeat_in_search() {
    std::cout << "repeat_in_search" << std::endl;
    g_position =
      Position::parse("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1").value();
    g_repetition_info.reset();
    g_repetition_info.push(g_position.get_hash_key(), false);
    std::cout << "startpos: " << std::hex << std::setw(16) << g_position.get_hash_key()
              << std::endl;
    REQUIRE(g_repetition_info.detect_repetition(0) == false);
    move("g1f3");
    REQUIRE(g_repetition_info.detect_repetition(1) == false);
    move("g8f6");
    REQUIRE(g_repetition_info.detect_repetition(2) == false);
    move("f3g1");
    REQUIRE(g_repetition_info.detect_repetition(3) == false);
    move("f6g8");
    REQUIRE(g_repetition_info.detect_repetition(4) == true);
    move("g1f3");
    REQUIRE(g_repetition_info.detect_repetition(5) == true);
    move("g8f6");
    REQUIRE(g_repetition_info.detect_repetition(6) == true);
    move("f3g1");
    REQUIRE(g_repetition_info.detect_repetition(7) == true);
    move("f6g8");
    REQUIRE(g_repetition_info.detect_repetition(8) == true);
}

int main() {
    Zobrist::init_zobrist_keys();
    repeat_in_history();
    repeat_in_search();
    return 0;
}

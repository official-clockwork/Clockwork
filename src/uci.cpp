#include "uci.hpp"
#include "bench.hpp"
#include "search.h"

#include <algorithm>
#include <ios>
#include <iostream>
#include <istream>
#include <sstream>
#include <string>
#include <string_view>

#include "move.hpp"
#include "perft.hpp"
#include "position.hpp"
#include "search.hpp"

namespace Clockwork::UCI {

constexpr std::string_view STARTPOS{"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"};

UCIHandler::UCIHandler() :
    m_position(*Position::parse(STARTPOS)) {
}

void UCIHandler::loop() {
    std::string input;

    while (std::getline(std::cin, input)) {
        execute_command(input);
    }
}

void UCIHandler::handle_command_line(i32 argc, char* argv[]) {
    for (i32 i = 1; i < argc; ++i) {
        execute_command(argv[i]);
    }
}

void UCIHandler::execute_command(const std::string& line) {
    std::istringstream is{line};

    std::string command;
    is >> std::skipws >> command;

    if (command == "uci") {
        std::cout << "id name Clockwork\n";
        std::cout << "id author The Clockwork community\n";
        std::cout << "uciok" << std::endl;
    } else if (command == "ucinewgame") {
        m_tt.clear();
    } else if (command == "isready") {
        std::cout << "readyok" << std::endl;
    } else if (command == "quit") {
        std::exit(0);
    } else if (command == "go") {
        handle_go(is);
    } else if (command == "position") {
        handle_position(is);
    } else if (command == "fen") {
        std::cout << m_position << std::endl;
    } else if (command == "attacks") {
        handle_attacks(is);
    } else if (command == "perft") {
        handle_perft(is);
    } else if (command == "bench") {
        handle_bench(is);
    } else {
        std::cout << "Unknown command" << std::endl;
    }
}

void UCIHandler::handle_bench(std::istringstream& is) {
    Depth depth = 6;
    if (!(is >> depth)) {
        is.clear();
        depth = 6;
    }
    Search::Worker worker{m_tt};
    Bench::benchmark(worker, depth);
}

void UCIHandler::handle_go(std::istringstream& is) {
    // Clear any previous settings
    settings = {};
    std::string token;
    while (is >> token) {
        if (token == "depth") {
            is >> settings.depth;
        } else if (token == "movetime") {
            is >> settings.move_time;
            settings.move_time = std::max(i64(0), settings.move_time);
        } else if (token == "wtime") {
            is >> settings.w_time;
            settings.w_time = std::max(i64(0), settings.w_time);
        } else if (token == "btime") {
            is >> settings.b_time;
            settings.b_time = std::max(i64(0), settings.b_time);
        } else if (token == "winc") {
            is >> settings.w_inc;
            settings.w_inc = std::max(i64(0), settings.w_inc);
        } else if (token == "binc") {
            is >> settings.b_inc;
            settings.b_inc = std::max(i64(0), settings.b_inc);
        } else if (token == "softnodes") {
            is >> settings.soft_nodes;
        } else if (token == "nodes") {
            is >> settings.hard_nodes;
        }
    }
    Search::Worker worker{m_tt};
    worker.launch_search(m_position, m_repetition_info, settings);
}

void UCIHandler::handle_position(std::istringstream& is) {
    std::string token;

    m_repetition_info.reset();

    if (is >> token) {
        if (token == "startpos") {
            m_position = *Position::parse(STARTPOS);
        } else if (token == "fen") {
            std::string board, color, castle, enpassant, clock, ply;
            is >> board >> color >> castle >> enpassant >> clock >> ply;
            auto pos = Position::parse(board, color, castle, enpassant, clock, ply);
            if (!pos) {
                std::cout << "Invalid fen" << std::endl;
                return;
            }
            m_position = *pos;
        } else {
            std::cout << "Unexpected token: " << token << std::endl;
            return;
        }
    }

    m_repetition_info.push(m_position.get_hash_key(), false);

    if (is >> token) {
        if (token != "moves") {
            std::cout << "Unexpected token: " << token << std::endl;
            return;
        }
        while (is >> token) {
            auto move = Move::parse(token, m_position);
            if (!move) {
                std::cout << "Invalid move: " << token << std::endl;
                return;
            }
            m_position = m_position.move(*move);
            m_repetition_info.push(m_position.get_hash_key(), m_position.is_reversible(*move));
        }
    }
}

void UCIHandler::handle_attacks(std::istringstream&) {
    std::cout << m_position.attack_table(Color::White) << std::endl;
    std::cout << m_position.attack_table(Color::Black) << std::endl;
    std::cout << "White: ";
    for (usize i = 0; i < 16; i++) {
        std::cout << " " << piece_char(m_position.piece_list(Color::White).array[i]) << ":"
                  << m_position.piece_list_sq(Color::White).array[i];
    }
    std::cout << std::endl;
    std::cout << "Black: ";
    for (usize i = 0; i < 16; i++) {
        std::cout << " " << piece_char(m_position.piece_list(Color::Black).array[i]) << ":"
                  << m_position.piece_list_sq(Color::Black).array[i];
    }
    std::cout << std::endl;
}

void UCIHandler::handle_perft(std::istringstream& is) {
    std::string token;
    i32         depth = 1;

    if (is >> token) {
        depth = std::stoi(token);
        depth = std::max(0, depth);
    }

    split_perft(m_position, static_cast<usize>(depth));
}

}

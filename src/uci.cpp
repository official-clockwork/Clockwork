#include "uci.hpp"
#include "bench.hpp"
#include "evaluation.hpp"
#include "move.hpp"
#include "movepick.hpp"
#include "perft.hpp"
#include "position.hpp"
#include "search.hpp"
#include "speedtest.hpp"
#include "tuned.hpp"
#include "util/ios_fmt_guard.hpp"
#include "util/parse.hpp"
#include "util/random.hpp"
#include "util/version.hpp"
#include <algorithm>
#include <fstream>
#include <ios>
#include <iostream>
#include <istream>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_set>

namespace Clockwork::UCI {


constexpr std::string_view STARTPOS{"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"};
constexpr usize            MAX_HASH    = 268435456;
constexpr usize            MAX_THREADS = 1024;

UCIHandler::UCIHandler() :
    m_position(*Position::parse(STARTPOS)) {
    searcher.initialize(1);
    searcher.set_position(m_position, m_repetition_info);
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
    searcher.exit();
}

void UCIHandler::execute_command(const std::string& line) {
    std::istringstream is{line};

    std::string command;
    is >> std::skipws >> command;

    if (command == "uci") {
        std::cout << "id name Clockwork " << GIT_VERSION << "\n";
        std::cout << "id author The Clockwork community\n";
        std::cout << "option name UCI_Chess960 type check default false\n";
        std::cout << "option name UseSoftNodes type check default false\n";
        std::cout << "option name Threads type spin default 1 min 1 max " << MAX_THREADS << "\n";
        std::cout << "option name Hash type spin default 16 min 1 max " << MAX_HASH << "\n";
        tuned::uci_print_tunable_options();
        std::cout << "uciok" << std::endl;
    } else if (command == "ucinewgame") {
        searcher.reset();
    } else if (command == "isready") {
        searcher.wait();
        std::cout << "readyok" << std::endl;
    } else if (command == "wait") {
        searcher.wait();
    } else if (command == "quit") {
        std::exit(0);
    } else if (command == "go") {
        handle_go(is);
    } else if (command == "stop") {
        searcher.stop_searching();
    } else if (command == "position") {
        handle_position(is);
    } else if (command == "setoption") {
        handle_setoption(is);
    } else if (command == "fen") {
        std::cout << m_position << std::endl;
    } else if (command == "d") {
        handle_d(is);
    } else if (command == "attacks") {
        handle_attacks(is);
    } else if (command == "tunables" || command == "tunestr") {
        tuned::uci_print_tunable_values();
    } else if (command == "perft") {
        handle_perft(is);
    } else if (command == "bench") {
        handle_bench(is);
    } else if (command == "speedtest") {
        handle_speedtest(is);
    }
#ifndef EVAL_TUNING
    else if (command == "eval") {
        std::cout << "Evaluation (stm): " << evaluate_stm_pov(m_position) << std::endl;
    }
#endif
    else if (command == "genfens") {
        handle_genfens(is);
    } else {
        std::cout << "Unknown command" << std::endl;
    }
}


void UCIHandler::handle_bench(std::istringstream& is) {
    Depth depth = 16;
    if (!(is >> depth)) {
        is.clear();
        depth = 16;
    }
    Bench::benchmark(searcher, depth);
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
            if (m_use_soft_nodes) {
                is >> settings.soft_nodes;
                settings.hard_nodes = settings.soft_nodes * 16;
            } else {
                is >> settings.hard_nodes;
            }
        }
    }
    searcher.launch_search(settings);
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

    searcher.set_position(m_position, m_repetition_info);
}

void UCIHandler::handle_d(std::istringstream&) {
    IosFmtGuard guard{std::cout};
    std::cout << "   +------------------------+" << std::endl;
    for (i32 rank = 7; rank >= 0; rank--) {
        std::cout << " " << static_cast<char>('1' + rank) << " |";
        for (i32 file = 0; file < 8; file++) {
            Place place = m_position.board()[Square::from_file_and_rank(file, rank)];
            std::cout << " " << place.to_char() << " ";
        }
        std::cout << "|" << std::endl;
    }
    std::cout << "   +------------------------+" << std::endl;
    std::cout << "     a  b  c  d  e  f  g  h  " << std::endl;
    std::cout << std::endl;
    std::cout << "fen: " << m_position << std::endl;
    std::cout << "key: " << std::hex << std::setw(16) << std::setfill('0')
              << m_position.get_hash_key() << std::endl;
}

void UCIHandler::handle_setoption(std::istringstream& is) {
    std::string token, name, value_str;

    is >> token;
    if (token != "name") {
        std::cout << "Unexpected token: " << token << std::endl;
        return;
    }

    is >> name;

    is >> token;
    if (token != "value") {
        std::cout << "Unexpected token: " << token << std::endl;
        return;
    }

    is >> value_str;

    if (name == "UCI_Chess960") {
        if (value_str == "true") {
            g_frc = true;
        } else if (value_str == "false") {
            g_frc = false;
        } else {
            std::cout << "Invalid value " << value_str << std::endl;
        }
    } else if (name == "Hash") {
        if (auto value = parse_number<usize>(value_str)) {
            usize hash_size = std::clamp<usize>(*value, 1, MAX_HASH);
            searcher.resize_tt(hash_size);
        } else {
            std::cout << "Invalid value " << value_str << std::endl;
        }
    } else if (name == "Threads") {
        if (auto value = parse_number<usize>(value_str)) {
            size_t thread_count = std::clamp<size_t>(*value, 1, MAX_THREADS);
            searcher.initialize(thread_count);
            searcher.set_position(m_position, m_repetition_info);
        } else {
            std::cout << "Invalid value " << value_str << std::endl;
        }
    } else if (name == "UseSoftNodes") {
        if (value_str == "true") {
            m_use_soft_nodes = true;
        } else if (value_str == "false") {
            m_use_soft_nodes = false;
        } else {
            std::cout << "Invalid value " << value_str << std::endl;
        }
    } else if (tuned::uci_parse_tunable(name, value_str)) {
        // Successfully parsed tunable
    } else {
        std::cout << "Unknown option: " << name << std::endl;
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

void UCIHandler::handle_genfens(std::istringstream& is) {
    int         N             = 0;
    uint64_t    seed          = 0;
    bool        seed_provided = false;
    std::string book          = "None";
    std::string token;

    // Parse arguments
    if (!(is >> N) || N <= 0) {
        std::cout << "Invalid or missing number of positions (N)." << std::endl;
        return;
    }

    while (is >> token) {
        if (token == "seed") {
            if (!(is >> seed)) {
                std::cout << "Invalid seed value." << std::endl;
                return;
            }
            seed_provided = true;
        } else if (token == "book") {
            if (!(is >> book)) {
                std::cout << "Missing book filename after 'book'." << std::endl;
                return;
            }
        } else {
            std::cout << "Invalid genfens argument: " << token << std::endl;
            return;
        }
    }

    // Set RNG state
    if (!seed_provided) {
        std::cout << "Seed not provided. Defaulting to 0." << std::endl;
    }
    Clockwork::Random::seed({seed, seed, seed | 1, seed ^ 0xDEADBEEFDEADBEEFULL});

    std::vector<std::string> lines;
    std::string              line;
    if (book != "None") {
        std::cout << "Using book file: " << book << std::endl;

        // Open the book file
        std::ifstream file(book);
        if (!file) {
            std::cout << "Could not open file: " << book << std::endl;
            return;
        }

        // Load all lines
        while (std::getline(file, line)) {
            if (!line.empty()) {
                lines.push_back(line);
            }
        }

        if (lines.empty()) {
            std::cout << "Book file is empty." << std::endl;
            return;
        }
        file.close();
    } else {
        // Add startpositions to lines
        lines.push_back("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    }

    // Line generation is as follows:
    // 1) Pick a random line from the book (or startposition if no book)
    // 2) Play random legal moves (see passing noisy or quiet moves)
    // Launch a 16k softnodes verification search to make sure the position doesn't lose immediately
    // 3) If the position is legal, print it
    i32 generated = 0;

    while (generated < N) {
reset:
        // Pick a random line from the book
        const std::string& selected_line = lines[Clockwork::Random::rand_64() % lines.size()];

        // Set up position
        Position pos = *Position::parse(selected_line);

        int moves = 0;
        // Make 4 random moves out of book
        while (moves < 4) {
            RandomMovePicker picker(pos);
            Move             m = picker.next();
            if (m == Move::none()) {
                // No moves available, skip
                goto reset;
            }
            pos = pos.move(m);
            moves++;
        }

        // Mock search limits for datagen verification
        Search::SearchSettings settings = {
          .stm = pos.active_color(), .hard_nodes = 1048576, .soft_nodes = 16384, .silent = true};

        searcher.initialize(1);  // Initialize with 1 thread always for datagen

        RepetitionInfo rep_info;
        rep_info.reset();
        rep_info.push(pos.get_hash_key(), false);

        searcher.set_position(pos, rep_info);
        searcher.launch_search(settings);

        // Wait for the search to finish and get the score
        Value score = searcher.wait_for_score();

        if (std::abs(score) > 450) {
            // Position is mate or losing, skip
            goto reset;
        }

        // If we reach here, the position is legal
        std::cout << "info string genfens " << pos << std::endl;
        generated++;
    }
}

void UCIHandler::handle_speedtest(std::istringstream&) {
    Speedtest::speedtest(searcher);
}

}  // namespace Clockwork::UCI

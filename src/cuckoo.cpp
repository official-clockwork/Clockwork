#include "cuckoo.hpp"
#include "bitboard.hpp"
#include "rays.hpp"
#include "zobrist.hpp"
#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

namespace Clockwork {

std::array<HashKey, Cuckoo::TABLE_SIZE> Cuckoo::keys{};
std::array<Move, Cuckoo::TABLE_SIZE>    Cuckoo::moves{};

// helper function to check if a piece of given type can attack from sq0 to sq1
// sort of hate this, in viri i do this differently.
static bool can_attack(PieceType ptype, Square sq0, Square sq1) {
    if (sq0 == sq1) {
        return false;
    }

    auto [f0, r0] = sq0.to_file_and_rank();
    auto [f1, r1] = sq1.to_file_and_rank();

    i32 file_diff     = f1 - f0;
    i32 rank_diff     = r1 - r0;
    i32 abs_file_diff = std::abs(file_diff);
    i32 abs_rank_diff = std::abs(rank_diff);

    switch (ptype) {
    case PieceType::Knight:
        return (abs_file_diff == 2 && abs_rank_diff == 1)
            || (abs_file_diff == 1 && abs_rank_diff == 2);

    case PieceType::Bishop:
        return abs_file_diff == abs_rank_diff && abs_file_diff > 0;

    case PieceType::Rook:
        return (file_diff == 0 && rank_diff != 0) || (file_diff != 0 && rank_diff == 0);

    case PieceType::Queen:
        return (file_diff == 0 && rank_diff != 0) || (file_diff != 0 && rank_diff == 0)
            || (abs_file_diff == abs_rank_diff && abs_file_diff > 0);

    case PieceType::King:
        return abs_file_diff <= 1 && abs_rank_diff <= 1;

    default:
        return false;
    }
}

void Cuckoo::init() {
    // std::cout << "Initializing cuckoo hash tables..." << std::endl;

    // keep a tally of the table entries to sanity-check the initialization process.
    usize count = 0;

    std::vector<HashKey> temp_keys(TABLE_SIZE, 0);
    std::vector<Move>    temp_moves(TABLE_SIZE, Move::none());

    for (Color color : {Color::White, Color::Black}) {
        for (PieceType ptype : {PieceType::Knight, PieceType::Bishop, PieceType::Rook,
                                PieceType::Queen, PieceType::King}) {
            for (u8 sq0_raw = 0; sq0_raw < 64; sq0_raw++) {
                Square sq0{sq0_raw};
                for (u8 sq1_raw = sq0_raw + 1; sq1_raw < 64; sq1_raw++) {
                    Square sq1{sq1_raw};

                    // check if a piece of this type standing on sq0 could attack sq1
                    if (!can_attack(ptype, sq0, sq1)) {
                        continue;
                    }

                    HashKey key = Zobrist::piece_square_zobrist[static_cast<usize>(color)]
                                                               [static_cast<usize>(ptype)][sq0_raw]
                                ^ Zobrist::piece_square_zobrist[static_cast<usize>(color)]
                                                               [static_cast<usize>(ptype)][sq1_raw]
                                ^ Zobrist::side_key;

                    Move mv{sq0, sq1};

                    // Insert into cuckoo hash table using cuckoo hashing
                    usize slot = h1(key);
                    while (true) {
                        std::swap(temp_keys[slot], key);
                        std::swap(temp_moves[slot], mv);

                        if (mv == Move::none()) {
                            break;
                        }

                        slot = (slot == h1(key)) ? h2(key) : h1(key);
                    }
                    count++;
                }
            }
        }
    }

    // std::cout << "Inserted " << count << " entries into cuckoo hash table." << std::endl;

    assert(count == 3668);

    // woo C++ evil initialisation
    std::ranges::copy(temp_keys, keys.begin());
    std::ranges::copy(temp_moves, moves.begin());
}

}  // namespace Clockwork

#include "zobrist.hpp"
#include "util/random.hpp"
#include <cstddef>

namespace Clockwork {

std::array<std::array<std::array<HashKey, 64>, 7>, 2> Zobrist::piece_square_zobrist{};
std::array<HashKey, 64>                               Zobrist::en_passant_zobrist{};
std::array<HashKey, 16>                               Zobrist::castling_zobrist{};
HashKey                                               Zobrist::side_key = 0;


void Zobrist::init_zobrist_keys() {
    // PieceType - Square zobrists
    for (size_t color_index = 0; color_index < 2; ++color_index) {
        // Exculde index 0 (null piece) which is already zero filled
        for (size_t piece_index = 1; piece_index < 7; ++piece_index) {
            for (size_t sq_index = 0; sq_index < 64; ++sq_index) {
                piece_square_zobrist[color_index][piece_index][sq_index] = Random::rand_64();
            }
        }
    }
    // En passant zobrists
    for (size_t sq_index = 0; sq_index < 64; ++sq_index) {
        en_passant_zobrist[sq_index] = Random::rand_64();
    }
    // Castling rights zobrist
    for (size_t castle_index = 0; castle_index < 16; ++castle_index) {
        castling_zobrist[castle_index] = Random::rand_64();
    }
    // Stm zobrist
    side_key = Random::rand_64();
}

}  // namespace Clockwork

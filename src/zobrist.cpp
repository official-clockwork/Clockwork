#include "zobrist.hpp"
#include "util/random.hpp"
#include <cstddef>

namespace Clockwork {

std::array<std::array<std::array<HashKey, 64>, 7>, 2> Zobrist::piece_square_zobrist{};
std::array<HashKey, 64>                               Zobrist::en_passant_zobrist{};
std::array<HashKey, 16>                               Zobrist::castling_zobrist{};
HashKey                                               Zobrist::side_key = 0;

std::array<std::array<std::array<ZobristInfo, 64>, 7>, 2> Zobrist::piece_zobrist_info{};


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

    // Initialize piece_zobrist_info using the correct per piecetype/color update rules.
    // IMPORTANT! From now on, the update rules of the secondary keys go here, new key's update rules should be added here.
    for (size_t color_index = 0; color_index < 2; ++color_index) {
        for (size_t piece_index = 1; piece_index < 7; ++piece_index) {
            for (size_t sq_index = 0; sq_index < 64; ++sq_index) {
                HashKey piece_key = piece_square_zobrist[color_index][piece_index][sq_index];
                HashKey pawn_key =
                  (piece_index == static_cast<size_t>(PieceType::Pawn)) ? piece_key : 0;
                HashKey non_pawn_key =
                  (piece_index != static_cast<size_t>(PieceType::Pawn)) ? piece_key : 0;
                HashKey major_key = (piece_index == static_cast<size_t>(PieceType::Rook)
                                     || piece_index == static_cast<size_t>(PieceType::Queen)
                                     || piece_index == static_cast<size_t>(PieceType::King))
                                    ? piece_key
                                    : 0;
                HashKey minor_key = (piece_index == static_cast<size_t>(PieceType::Knight)
                                     || piece_index == static_cast<size_t>(PieceType::Bishop)
                                     || piece_index == static_cast<size_t>(PieceType::King))
                                    ? piece_key
                                    : 0;

                ZobristInfo info(piece_key, pawn_key, (color_index == 0) ? non_pawn_key : 0,
                                 (color_index == 1) ? non_pawn_key : 0, major_key, minor_key);
                piece_zobrist_info[color_index][piece_index][sq_index] = info;
            }
        }
    }
}

}  // namespace Clockwork

#pragma once

#include "common.hpp"
#include "eval_constants.hpp"
#include "eval_types.hpp"
#include "position.hpp"
#include "square.hpp"
#include "util/static_vector.hpp"

namespace Clockwork {

struct PsqtUpdates {
    struct Update {
        Color     color;
        PieceType pt;
        Square    sq;
    };
    StaticVector<Update, 2> adds;
    StaticVector<Update, 2> removes;
};

struct PsqtState {
public:
    PsqtState() = default;
    PsqtState(const Position& pos) {
        for (Color c : {Color::White, Color::Black}) {
            auto& pieces  = pos.piece_list(c);
            auto& squares = pos.piece_list_sq(c);

            for (size_t i = 0; i < 16; i) {
                PieceType pt = pieces[i];
                if (pt == PieceType::None) {
                    continue;
                }

                add_piece(c, pt, squares[i]);
            }
        }
    }

    void add_piece(Color color, PieceType pt, Square sq) {
        if (color == Color::White) {
            sq = sq.flip_vertical();
        }
        auto& accumulator = m_accumulators[static_cast<usize>(color)];
        switch (pt) {
        case PieceType::Pawn:
            accumulator[0] += PAWN_MAT + PAWN_PSQT[sq.raw - 8];
            accumulator[1] += PAWN_MAT + PAWN_PSQT[sq.flip_horizontal().raw - 8];
            break;
        case PieceType::Knight:
            accumulator[0] += KNIGHT_MAT + KNIGHT_PSQT[sq.raw];
            accumulator[1] += KNIGHT_MAT + KNIGHT_PSQT[sq.flip_horizontal().raw];
            break;
        case PieceType::Bishop:
            accumulator[0] += BISHOP_MAT + BISHOP_PSQT[sq.raw];
            accumulator[1] += BISHOP_MAT + BISHOP_PSQT[sq.flip_horizontal().raw];
            break;
        case PieceType::Rook:
            accumulator[0] += ROOK_MAT + ROOK_PSQT[sq.raw];
            accumulator[1] += ROOK_MAT + ROOK_PSQT[sq.flip_horizontal().raw];
            break;
        case PieceType::Queen:
            accumulator[0] += QUEEN_MAT + QUEEN_PSQT[sq.raw];
            accumulator[1] += QUEEN_MAT + QUEEN_PSQT[sq.flip_horizontal().raw];
            break;
        case PieceType::King:
            accumulator[0] += KING_PSQT[sq.raw];
            accumulator[1] += KING_PSQT[sq.flip_horizontal().raw];
            break;
        default:
            unreachable();
            break;
        }
    }

    void remove_piece(Color color, PieceType pt, Square sq) {
        if (color == Color::White) {
            sq = sq.flip_vertical();
        }
        auto& accumulator = m_accumulators[static_cast<usize>(color)];
        switch (pt) {
        case PieceType::Pawn:
            accumulator[0] -= PAWN_MAT + PAWN_PSQT[sq.raw - 8];
            accumulator[1] -= PAWN_MAT + PAWN_PSQT[sq.flip_horizontal().raw - 8];
            break;
        case PieceType::Knight:
            accumulator[0] -= KNIGHT_MAT + KNIGHT_PSQT[sq.raw];
            accumulator[1] -= KNIGHT_MAT + KNIGHT_PSQT[sq.flip_horizontal().raw];
            break;
        case PieceType::Bishop:
            accumulator[0] -= BISHOP_MAT + BISHOP_PSQT[sq.raw];
            accumulator[1] -= BISHOP_MAT + BISHOP_PSQT[sq.flip_horizontal().raw];
            break;
        case PieceType::Rook:
            accumulator[0] -= ROOK_MAT + ROOK_PSQT[sq.raw];
            accumulator[1] -= ROOK_MAT + ROOK_PSQT[sq.flip_horizontal().raw];
            break;
        case PieceType::Queen:
            accumulator[0] -= QUEEN_MAT + QUEEN_PSQT[sq.raw];
            accumulator[1] -= QUEEN_MAT + QUEEN_PSQT[sq.flip_horizontal().raw];
            break;
        case PieceType::King:
            accumulator[0] -= KING_PSQT[sq.raw];
            accumulator[1] -= KING_PSQT[sq.flip_horizontal().raw];
            break;
        default:
            unreachable();
            break;
        }
    }

    void apply_updates(const PsqtUpdates& updates) {
        for (const auto& add : updates.adds) {
            add_piece(add.color, add.pt, add.sq);
        }

        for (const auto& remove : updates.removes) {
            remove_piece(remove.color, remove.pt, remove.sq);
        }
    }

    PScore score(const Position& pos) const;

    bool operator==(const PsqtState& other) const noexcept = default;
    bool operator!=(const PsqtState& other) const noexcept = default;

private:
    std::array<std::array<PScore, 2>, 2> m_accumulators;
};

}

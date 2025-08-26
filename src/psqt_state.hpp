#pragma once

#include "common.hpp"
#include "eval_constants.hpp"
#include "eval_types.hpp"
#include "square.hpp"


namespace Clockwork {

struct PsqtState {
public:
    PsqtState() = default;

    void add_piece(Color color, PieceType pt, Square sq) {
        if (color == Color::White) {
            sq = sq.flip_vertical();
        }
        PScore diff{};
        switch (pt) {
        case PieceType::Pawn:
            diff = PAWN_MAT + PAWN_PSQT[sq.raw - 8];
            break;
        case PieceType::Knight:
            diff = KNIGHT_MAT + KNIGHT_PSQT[sq.raw];
            break;
        case PieceType::Bishop:
            diff = BISHOP_MAT + BISHOP_PSQT[sq.raw];
            break;
        case PieceType::Rook:
            diff = ROOK_MAT + ROOK_PSQT[sq.raw];
            break;
        case PieceType::Queen:
            diff = QUEEN_MAT + QUEEN_PSQT[sq.raw];
            break;
        case PieceType::King:
            diff = KING_PSQT[sq.raw];
            break;
        default:
            unreachable();
            break;
        }
        // TODO: change this to +=/-= after adding this operators
        if (color == Color::White) {
            m_score = m_score + diff;
        } else {
            m_score = m_score - diff;
        }
    }

    void remove_piece(Color color, PieceType pt, Square sq) {
        if (color == Color::White) {
            sq = sq.flip_vertical();
        }
        PScore diff{};
        switch (pt) {
        case PieceType::Pawn:
            diff = PAWN_MAT + PAWN_PSQT[sq.raw - 8];
            break;
        case PieceType::Knight:
            diff = KNIGHT_MAT + KNIGHT_PSQT[sq.raw];
            break;
        case PieceType::Bishop:
            diff = BISHOP_MAT + BISHOP_PSQT[sq.raw];
            break;
        case PieceType::Rook:
            diff = ROOK_MAT + ROOK_PSQT[sq.raw];
            break;
        case PieceType::Queen:
            diff = QUEEN_MAT + QUEEN_PSQT[sq.raw];
            break;
        case PieceType::King:
            diff = KING_PSQT[sq.raw];
            break;
        default:
            unreachable();
            break;
        }
        // TODO: change this to +=/-= after adding this operators
        if (color == Color::White) {
            m_score = m_score - diff;
        } else {
            m_score = m_score + diff;
        }
    }

    PScore score() const {
        return m_score;
    }

    bool operator==(const PsqtState& other) const noexcept = default;
    bool operator!=(const PsqtState& other) const noexcept = default;

private:
    PScore m_score = PSCORE_ZERO;
};

}

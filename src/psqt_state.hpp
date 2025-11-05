#pragma once

#include "common.hpp"
#include "eval_constants.hpp"
#include "eval_types.hpp"
#include "position.hpp"
#include "square.hpp"
#include "util/static_vector.hpp"
#include <bit>

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
            m_accumulators[static_cast<usize>(c)] = PSCORE_ZERO;
            m_king_sides[static_cast<usize>(c)]   = pos.king_side(c);
            auto& pieces                          = pos.piece_list(c);
            auto& squares                         = pos.piece_list_sq(c);

            for (u8 i = 0; i < 16; i++) {
                PieceType pt = pieces[i];
                if (pt == PieceType::None) {
                    continue;
                }

                add_piece(c, pt, squares[i]);
            }
        }
    }

    void apply_updates(const Position& pos, const PsqtUpdates& updates) {
        Color reload_side;
        if (pos.king_side(Color::White) != m_king_sides[static_cast<usize>(Color::White)]) {
            reload_side = Color::White;
        } else if (pos.king_side(Color::Black) != m_king_sides[static_cast<usize>(Color::Black)]) {
            reload_side = Color::Black;
        } else {
            for (const auto& add : updates.adds) {
                add_piece(add.color, add.pt, add.sq);
            }

            for (const auto& remove : updates.removes) {
                remove_piece(remove.color, remove.pt, remove.sq);
            }
            return;
        }

        m_king_sides[static_cast<usize>(reload_side)] = pos.king_side(reload_side);
        reload_accumulator(pos, reload_side);

        for (const auto& remove : updates.removes) {
            if (remove.color != reload_side) {
                remove_piece(remove.color, remove.pt, remove.sq);
            }
        }
    }

    PScore score() const {
        return m_accumulators[static_cast<usize>(Color::White)]
             - m_accumulators[static_cast<usize>(Color::Black)];
    }

    bool operator==(const PsqtState& other) const noexcept = default;
    bool operator!=(const PsqtState& other) const noexcept = default;

private:
    void reload_accumulator(const Position& pos, Color c) {
        m_accumulators[static_cast<usize>(c)] = PSCORE_ZERO;
        auto&     pieces                      = pos.piece_list(c);
        auto&     squares                     = pos.piece_list_sq(c);
        PieceMask valid_pieces                = pieces.mask_valid();

        for (PieceId id : valid_pieces) {
            PieceType pt = pieces[id];
            add_piece(c, pt, squares[id]);
        }
    }

    void add_piece(Color color, PieceType pt, Square sq) {
        if (color == Color::White) {
            sq = sq.flip_vertical();
        }

        if (m_king_sides[static_cast<usize>(color)]) {
            sq = sq.flip_horizontal();
        }

        auto& accumulator = m_accumulators[static_cast<usize>(color)];
        switch (pt) {
        case PieceType::Pawn:
            accumulator += PAWN_MAT + PAWN_PSQT[sq.raw - 8];
            break;
        case PieceType::Knight:
            accumulator += KNIGHT_MAT + KNIGHT_PSQT[sq.raw];
            break;
        case PieceType::Bishop:
            accumulator += BISHOP_MAT + BISHOP_PSQT[sq.raw];
            break;
        case PieceType::Rook:
            accumulator += ROOK_MAT + ROOK_PSQT[sq.raw];
            break;
        case PieceType::Queen:
            accumulator += QUEEN_MAT + QUEEN_PSQT[sq.raw];
            break;
        case PieceType::King:
            accumulator += KING_PSQT[sq.raw];
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

        if (m_king_sides[static_cast<usize>(color)]) {
            sq = sq.flip_horizontal();
        }

        auto& accumulator = m_accumulators[static_cast<usize>(color)];
        switch (pt) {
        case PieceType::Pawn:
            accumulator -= PAWN_MAT + PAWN_PSQT[sq.raw - 8];
            break;
        case PieceType::Knight:
            accumulator -= KNIGHT_MAT + KNIGHT_PSQT[sq.raw];
            break;
        case PieceType::Bishop:
            accumulator -= BISHOP_MAT + BISHOP_PSQT[sq.raw];
            break;
        case PieceType::Rook:
            accumulator -= ROOK_MAT + ROOK_PSQT[sq.raw];
            break;
        case PieceType::Queen:
            accumulator -= QUEEN_MAT + QUEEN_PSQT[sq.raw];
            break;
        case PieceType::King:
            accumulator -= KING_PSQT[sq.raw];
            break;
        default:
            unreachable();
            break;
        }
    }

    std::array<PScore, 2> m_accumulators;
    std::array<bool, 2>   m_king_sides;
};

}  // namespace Clockwork

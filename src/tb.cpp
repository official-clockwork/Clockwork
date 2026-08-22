#include "tb.hpp"
#include "bb_attacks.hpp"
#include <algorithm>
#include <array>
#include <string>
#include <tbprobe.h>


#include <iostream>

namespace Clockwork::tb {

namespace {

[[nodiscard]] u64 piece_type_bb(const Position& pos, PieceType piece_type) {
    const auto bb =
      pos.bitboard_for(Color::White, piece_type) | pos.bitboard_for(Color::Black, piece_type);
    return bb.value();
}

}

InitStatus init(std::string_view path) {
    const std::string path_str{path};

    if (!tb_init(path_str.c_str())) {
        return InitStatus::Failed;
    }

    if (TB_LARGEST == 0) {
        return InitStatus::NoneFound;
    }

    return InitStatus::Success;
}

void free() {
    tb_free();
}

u32 dtz_count() {
    return static_cast<u32>(TB_NUM_DTZ);
}

u32 wdl_count() {
    return static_cast<u32>(TB_NUM_WDL);
}

u32 max_pieces() {
    return static_cast<u32>(TB_LARGEST);
}

bool probe_root(const Position&             pos,
                RepetitionInfo&             repetition_info,
                std::span<Search::RootMove> root_moves) {
    const auto move_from_tb = [&](PyrrhicMove tb_move) {
        static constexpr std::array PROMO_PIECE_FLAGS = {
          MoveFlags::Normal,      MoveFlags::PromoQueen,  MoveFlags::PromoRook,
          MoveFlags::PromoBishop, MoveFlags::PromoKnight,
        };

        const Square from{static_cast<u8>(PYRRHIC_MOVE_FROM(tb_move))};
        const Square to{static_cast<u8>(PYRRHIC_MOVE_TO(tb_move))};

        auto flags = PROMO_PIECE_FLAGS[PYRRHIC_MOVE_FLAGS(tb_move) & PYRRHIC_MASK_PROMO_FLAGS];

        if (PYRRHIC_MOVE_IS_ENPASS(tb_move)) {
            // an ep move cannot be a promotion
            flags = MoveFlags::EnPassant;
        }

        if (pos.piece_at(to) != PieceType::None) {
            flags |= MoveFlags::CaptureBit;
        }

        return Move{from, to, flags};
    };

    const auto wdl_from_tb = [](i32 tb_rank) {
        static constexpr i32 MAX_DTZ = 262144;

        static constexpr i32 WIN_BOUND  = MAX_DTZ - 100;
        static constexpr i32 DRAW_BOUND = -MAX_DTZ + 101;

        if (tb_rank >= WIN_BOUND) {
            return Search::WDL::Win;
        } else if (tb_rank >= DRAW_BOUND) {
            return Search::WDL::Draw;
        } else {
            return Search::WDL::Loss;
        }
    };

    TbRootMoves tb_root_moves{};
    bool        dtz_succeeded = true;

    const auto ep_square = pos.en_passant();
    const u32  ep_idx    = ep_square.is_valid() ? ep_square.raw : 0;

    // has_repeated() test case:
    // - position fen rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 moves e2e4 e7e5 g1f3 b8c6 f1c4 g8f6 d2d4 e5d4 e1g1 f8c5 e4e5 d7d5 e5f6 d5c4 f1e1 c8e6 f3g5 d8d5 b1c3 d5f5 c3e4 e8c8 g2g4 f5e5 g5e6 f7e6 f6g7 h8g8 c1h6 d4d3 c2c3 d8d7 e4c5 e5c5 d1f3 c5d5 f3d5 e6d5 f2f4 c6d8 g1f2 d8f7 g4g5 b7b5 f4f5 d5d4 a2a3 d7d6 e1e6 d4c3 b2c3 d6e6 f5e6 f7d6 a1e1 a7a5 e1e3 b5b4 a3b4 a5b4 e3f3 c8b7 g5g6 h7g6 f3f8 d6e4 f2e3 d3d2 e6e7 d2d1q e7e8q e4d6 e8g6 d1e1 e3d4 e1d1 d4e5 d1e2 e5d5 e2d1 d5e6 d1e1 e6d7 e1d1 g6g2 d6e4 d7e6 d1d6 e6f5 d6c5 f5f4 c5d6 f4f3 d6d3 f3g4 d3d7 g4h5 d7d5 h5h4 d5e6 f8g8 e6h6 h4g4 h6e6 g4f3 e6g8 f3e4 b7b6 g2f2 b6b5 f2f8 g8e6 e4d4 e6g4 d4e5 g4e2 e5f6 e2f3 f6e7 f3e4 e7d7 e4c6 d7d8 c6d5 d8c7 d5c6 c7d8 c6b6 d8e7 b6e3 e7f7 e3f3 f7e8 f3c6 e8e7 c6e4 e7d6 e4g6 d6e7 g6e4 e7d6 e4g6 d6e5 g6g5 e5e6 g5e3 e6f7 e3f3 f7g8 f3d5 f8f7 d5d6 f7e8 b5a6 e8a4 a6b6 a4b4 d6b4 c3b4 c4c3 g8f7 c3c2 g7g8q c2c1q g8b8 b6a6 b8a8 a6b6 a8b8 b6a6 b8d6 a6b7 d6e7 b7a6 h2h4 c1d1 e7g5 d1b3 f7g7 b3b4 h4h5 b4b7 g7g6 b7b1 g5f5 b1g1 g6h7 g1a7 h7g8 a7e3 g8f7 e3a7 f7f6 a7d4 f6e7 d4a7 e7f6 a7d4 f6f7 d4a7 f7g8 a7e3 g8g7 a6b7 h5h6 e3g3 g7f7 g3c7 f7g6 c7g3 f5g5 g3d3 g5f5 d3g3
    // - f5g5 should have a lower tb rank (262110) than g6f6 (262114)
    auto wdl = tb_probe_root_dtz(
      pos.board().get_color_bitboard(Color::White).value(),
      pos.board().get_color_bitboard(Color::Black).value(), piece_type_bb(pos, PieceType::King),
      piece_type_bb(pos, PieceType::Queen), piece_type_bb(pos, PieceType::Rook),
      piece_type_bb(pos, PieceType::Bishop), piece_type_bb(pos, PieceType::Knight),
      piece_type_bb(pos, PieceType::Pawn), pos.get_50mr_counter(), ep_idx,
      pos.active_color() == Color::White, repetition_info.has_repeated(), &tb_root_moves);

    if (!wdl) {
        dtz_succeeded = false;
        wdl           = tb_probe_root_wdl(
          pos.board().get_color_bitboard(Color::White).value(),
          pos.board().get_color_bitboard(Color::Black).value(), piece_type_bb(pos, PieceType::King),
          piece_type_bb(pos, PieceType::Queen), piece_type_bb(pos, PieceType::Rook),
          piece_type_bb(pos, PieceType::Bishop), piece_type_bb(pos, PieceType::Knight),
          piece_type_bb(pos, PieceType::Pawn), pos.get_50mr_counter(), ep_idx,
          pos.active_color() == Color::White, true, &tb_root_moves);
    }

    if (!wdl || tb_root_moves.size == 0) {
        return dtz_succeeded;
    }

    const auto get_root_move = [&](Move move) -> Search::RootMove* {
        for (auto& root_move : root_moves) {
            if (root_move.pv.first_move() == move) {
                return &root_move;
            }
        }
        return nullptr;
    };

    for (usize i = 0; i < tb_root_moves.size; i++) {
        auto [tb_move, tb_rank] = tb_root_moves.moves[i];

        const auto move = move_from_tb(tb_move);

        // Correct moves that immediately threefold.
        // Test case:
        // - position fen rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 moves e2e4 e7e5 g1f3 b8c6 f1c4 g8f6 d2d4 e5d4 e1g1 f8c5 e4e5 d7d5 e5f6 d5c4 f1e1 c8e6 f3g5 d8d5 b1c3 d5f5 c3e4 e8c8 g2g4 f5e5 g5e6 f7e6 f6g7 h8g8 c1h6 d4d3 c2c3 d8d7 e4c5 e5c5 d1f3 c5d5 f3d5 e6d5 f2f4 c6d8 g1f2 d8f7 g4g5 b7b5 f4f5 d5d4 a2a3 d7d6 e1e6 d4c3 b2c3 d6e6 f5e6 f7d6 a1e1 a7a5 e1e3 b5b4 a3b4 a5b4 e3f3 c8b7 g5g6 h7g6 f3f8 d6e4 f2e3 d3d2 e6e7 d2d1q e7e8q e4d6 e8g6 d1e1 e3d4 e1d1 d4e5 d1e2 e5d5 e2d1 d5e6 d1e1 e6d7 e1d1 g6g2 d6e4 d7e6 d1d6 e6f5 d6c5 f5f4 c5d6 f4f3 d6d3 f3g4 d3d7 g4h5 d7d5 h5h4 d5e6 f8g8 e6h6 h4g4 h6e6 g4f3 e6g8 f3e4 b7b6 g2f2 b6b5 f2f8 g8e6 e4d4 e6g4 d4e5 g4e2 e5f6 e2f3 f6e7 f3e4 e7d7 e4c6 d7d8 c6d5 d8c7 d5c6 c7d8 c6b6 d8e7 b6e3 e7f7 e3f3 f7e8 f3c6 e8e7 c6e4 e7d6 e4g6 d6e7 g6e4 e7d6 e4g6 d6e5 g6g5 e5e6 g5e3 e6f7 e3f3 f7g8 f3d5 f8f7 d5d6 f7e8 b5a6 e8a4 a6b6 a4b4 d6b4 c3b4 c4c3 g8f7 c3c2 g7g8q c2c1q g8b8 b6a6 b8a8 a6b6 a8b8 b6a6 b8d6 a6b7 d6e7 b7a6 h2h4 c1d1 e7g5 d1b3 f7g7 b3b4 h4h5 b4b7 g7g6 b7b1 g5f5 b1g1 g6h7 g1a7 h7g8 a7e3 g8f7 e3a7 f7f6 a7d4 f6e7 d4a7 e7f6 a7d4 f6f7 d4a7 f7g8 a7e3 g8g7 a6b7 h5h6 e3g3 g7f7 g3c7 f7g6 c7g3 f5g5 g3d3 g5f5 d3g3 f5g5 g3d3 g5f5
        // - d3g3 should be corrected to rank 0, all others losing
        if (pos.is_reversible(move)) {
            Position pos_after = pos.move(move);
            repetition_info.push(pos_after.get_hash_key(), true);
            if (repetition_info.detect_repetition(0)) {
                tb_rank = 0;
            }
            repetition_info.pop();
        }

        auto* root_move = get_root_move(move);
        if (!root_move) {
            continue;
        }

        const auto wdl = wdl_from_tb(tb_rank);
        root_move->set_tb_status(wdl, tb_rank);
    }

    std::ranges::stable_sort(root_moves, [](const Search::RootMove& a, const Search::RootMove& b) {
        return a.tb_rank > b.tb_rank;
    });

    return dtz_succeeded;
}

Search::WDL probe_wdl(const Position& pos) {
    const auto ep_square = pos.en_passant();
    const u32  ep_idx    = ep_square.is_valid() ? ep_square.raw : 0;

    const auto wdl = tb_probe_wdl(
      pos.board().get_color_bitboard(Color::White).value(),
      pos.board().get_color_bitboard(Color::Black).value(), piece_type_bb(pos, PieceType::King),
      piece_type_bb(pos, PieceType::Queen), piece_type_bb(pos, PieceType::Rook),
      piece_type_bb(pos, PieceType::Bishop), piece_type_bb(pos, PieceType::Knight),
      piece_type_bb(pos, PieceType::Pawn), ep_idx, pos.active_color() == Color::White);

    switch (wdl) {
    case TB_RESULT_FAILED:
        return Search::WDL::None;
    case TB_WIN:
        return Search::WDL::Win;
    case TB_LOSS:
        return Search::WDL::Loss;
    default:
        // Cursed wins and blessed losses are both functionally draws
        return Search::WDL::Draw;
    }
}

}

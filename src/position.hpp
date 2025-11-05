#pragma once

#include "board.hpp"
#include "move.hpp"
#include "square.hpp"
#include "util/types.hpp"
#include <array>
#include <bit>
#include <cassert>
#include <iosfwd>
#include <tuple>

namespace Clockwork {

struct PsqtState;
struct PsqtUpdates;

template<typename T>
struct alignas(16) PieceList {
    std::array<T, 16> array{};

    constexpr T& operator[](PieceId id) {
        return array[id.raw];
    }
    constexpr T operator[](PieceId id) const {
        return array[id.raw];
    }

    [[nodiscard]] u8x16 to_vector() const {
        return std::bit_cast<u8x16>(array);
    }

    [[nodiscard]] PieceMask mask_valid() const {
        return PieceMask{to_vector().nonzeros().to_bits()};
    }

    [[nodiscard]] PieceMask mask_eq(PieceType ptype) const {
        return PieceMask{to_vector().eq(u8x16::splat(static_cast<u8>(ptype))).to_bits()};
    }

    template<PieceType... ptype>
    [[nodiscard]] PieceMask mask_eq() const {
        constexpr u8 bits = (0 | ... | (1 << static_cast<u8>(ptype)));
        // clang-format off
        const u8x16  to_bits{{
          0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80,  //
          0, 0, 0, 0, 0, 0, 0, 0,                          //
        }};
        // clang-format on
        return PieceMask{(to_vector().swizzle(to_bits) & u8x16::splat(bits)).nonzeros().to_bits()};
    }

    constexpr bool operator==(const PieceList& other) const {
        return array == other.array;
    }
};

struct RookInfo {
    Square aside = Square::invalid();
    Square hside = Square::invalid();

    constexpr void clear() {
        aside = Square::invalid();
        hside = Square::invalid();
    }

    constexpr void unset(Square sq) {
        aside = aside == sq ? Square::invalid() : aside;
        hside = hside == sq ? Square::invalid() : hside;
    }

    [[nodiscard]] constexpr bool is_clear() const {
        return !aside.is_valid() && !hside.is_valid();
    }

    [[nodiscard]] constexpr size_t as_index() const {
        return static_cast<size_t>(aside.is_valid()) | (static_cast<size_t>(hside.is_valid()) << 1);
    }

    constexpr bool operator==(const RookInfo&) const = default;
};

struct CreateSuperpieceMaskInfo {
    u16 padding1 = 0;  // must be zero
    u16 diag;          // north-east, north-west, south-east or south-west (2-7 squares away)
    u16 orth;          // north, east, south, west (2 - 7 squares away)
    u16 orth_near;     // north, east, south, west (1 square away)
    u16 knight;        // knight move away
    u16 wpawn_near;    // south-east or south-west (1 square away)
    u16 bpawn_near;    // north-east or north-west (1 square away)
    u16 padding2;
};
static_assert(sizeof(CreateSuperpieceMaskInfo) == 16);

struct Position {
public:
    constexpr Position() = default;

    [[nodiscard]] const Byteboard& board() const {
        return m_board;
    }
    [[nodiscard]] const Wordboard& attack_table(Color color) const {
        return m_attack_table[static_cast<usize>(color)];
    }
    [[nodiscard]] const PieceList<PieceType>& piece_list(Color color) const {
        return m_piece_list[static_cast<usize>(color)];
    }
    [[nodiscard]] const PieceList<Square>& piece_list_sq(Color color) const {
        return m_piece_list_sq[static_cast<usize>(color)];
    }
    [[nodiscard]] Color active_color() const {
        return m_active_color;
    }
    [[nodiscard]] Square en_passant() const {
        return m_enpassant;
    }
    [[nodiscard]] RookInfo rook_info(Color color) const {
        return m_rook_info[static_cast<usize>(color)];
    }
    [[nodiscard]] HashKey get_hash_key() const {
        return m_hash_key;
    }
    [[nodiscard]] HashKey get_pawn_key() const {
        return m_pawn_key;
    }
    [[nodiscard]] HashKey get_non_pawn_key(Color color) const {
        return m_non_pawn_key[static_cast<usize>(color)];
    }
    [[nodiscard]] HashKey get_major_key() const {
        return m_major_key;
    }
    [[nodiscard]] HashKey get_minor_key() const {
        return m_minor_key;
    }

    [[nodiscard]] Square king_sq(Color color) const {
        return piece_list_sq(color)[PieceId{0}];
    }

    [[nodiscard]] size_t king_side(Color color) const {
        return king_sq(color).file() >= 4;
    }

    [[nodiscard]] PieceType piece_at(Square sq) const {
        return m_board[sq].ptype();
    }

    [[nodiscard]] bool is_valid() const {
        return attack_table(m_active_color).read(king_sq(invert(m_active_color))).empty();
    }

    [[nodiscard]] PieceMask checker_mask() const {
        return attack_table(invert(m_active_color)).read(king_sq(m_active_color));
    }

    [[nodiscard]] bool is_in_check() const {
        return !checker_mask().empty();
    }

    [[nodiscard]] Bitboard bitboard_for(Color color, PieceType ptype) const {
        return m_board.bitboard_for(color, ptype);
    }

    [[nodiscard]] usize piece_count(Color color, PieceType ptype) const {
        return piece_list(color).mask_eq(ptype).popcount();
    }

    template<PieceType... ptypes>
    [[nodiscard]] usize piece_count(Color color) const {
        return piece_list(color).mask_eq<ptypes...>().popcount();
    }

    [[nodiscard]] PieceMask get_piece_mask(Color color) const {
        return piece_list(color).mask_valid();
    }

    [[nodiscard]] PieceMask get_piece_mask(Color color, PieceType ptype) const {
        return piece_list(color).mask_eq(ptype);
    }

    template<PieceType... ptypes>
    [[nodiscard]] PieceMask get_piece_mask(Color color) const {
        return piece_list(color).mask_eq<ptypes...>();
    }

    [[nodiscard]] bool is_square_attacked_by(Square sq, Color color) const {
        return !attack_table(color).read(sq).empty();
    }

    [[nodiscard]] bool is_square_attacked_by(Square sq, Color color, PieceType ptype) const {
        return !(attack_table(color).read(sq) & piece_list(color).mask_eq(ptype)).empty();
    }

    [[nodiscard]] bool is_square_attacked_by(Square sq, Color color, PieceId id) const {
        return attack_table(color).read(sq).is_set(id);
    }

    [[nodiscard]] Bitboard attacked_by(Color color, PieceType ptype) const {
        return attack_table(color).get_piece_mask_bitboard(piece_list(color).mask_eq(ptype));
    }

    [[nodiscard]] usize mobility_of(Color color, PieceId id) const {
        return attack_table(color).count_matching_mask(id.to_piece_mask());
    }

    [[nodiscard]] usize mobility_of(Color color, PieceId id, Bitboard mask) const {
        return (attack_table(color).get_piece_mask_bitboard(id.to_piece_mask()) & mask).popcount();
    }

    [[nodiscard]] Wordboard create_attack_table_superpiece_mask(Square                   sq,
                                                                CreateSuperpieceMaskInfo cmi) const;

    [[nodiscard]] PieceType pt_of(Color color, PieceId id) const {
        return piece_list(color)[id];
    }

    [[nodiscard]] usize piece_count(Color color) const {
        return 16 - piece_count(color, PieceType::None);
    }

    [[nodiscard]] isize ipiece_count(Color color, PieceType ptype) const {
        return static_cast<isize>(piece_count(color, ptype));
    }

    [[nodiscard]] bool is_kp_endgame() const {
        for (Color color : {Color::White, Color::Black}) {
            if (!(piece_count(color) == 1 + piece_count(color, PieceType::Pawn))) {
                return false;
            }
        }
        return true;
    }

    [[nodiscard]] bool is_insufficient_material() const {
        auto wpcnt = piece_count(Color::White);
        auto bpcnt = piece_count(Color::Black);
        switch (wpcnt + bpcnt) {
        case 2:
            return true;
        case 3:
            if (piece_count<PieceType::Pawn, PieceType::Rook, PieceType::Queen>(Color::White)
                || piece_count<PieceType::Pawn, PieceType::Rook, PieceType::Queen>(Color::Black)) {
                return false;
            }
            return true;
        case 4:
            if (piece_count<PieceType::Pawn, PieceType::Rook, PieceType::Queen>(Color::White)
                || piece_count<PieceType::Pawn, PieceType::Rook, PieceType::Queen>(Color::Black)) {
                return false;
            }
            return false;
        default:
            return false;
        }
    }

    template<bool UPDATE_PSQT>
    [[nodiscard]] Position move(Move m, PsqtState* psqt_state) const;
    [[nodiscard]] Position null_move() const;

    [[nodiscard]] Position move(Move m) const {
        return move<false>(m, nullptr);
    }
    [[nodiscard]] Position move(Move m, PsqtState& psqt_state) const {
        return move<true>(m, &psqt_state);
    }

    [[nodiscard]] std::tuple<Wordboard, Bitboard> calc_pin_mask() const;

    [[nodiscard]] u16 get_50mr_counter() const;

    [[nodiscard]] bool is_reversible(Move move);

    const std::array<Wordboard, 2> calc_attacks_slow();
    const std::array<PieceMask, 2> calc_attacks_slow(Square sq);

    [[nodiscard]] HashKey                calc_hash_key_slow() const;
    [[nodiscard]] HashKey                calc_pawn_key_slow() const;
    [[nodiscard]] std::array<HashKey, 2> calc_non_pawn_key_slow() const;
    [[nodiscard]] HashKey                calc_major_key_slow() const;
    [[nodiscard]] HashKey                calc_minor_key_slow() const;

    static std::optional<Position> parse(std::string_view str);
    static std::optional<Position> parse(std::string_view board,
                                         std::string_view color,
                                         std::string_view castle,
                                         std::string_view enpassant,
                                         std::string_view irreversible_clock,
                                         std::string_view ply);

    bool                 operator==(const Position&) const = default;
    friend std::ostream& operator<<(std::ostream& os, const Position& position);

private:
    std::array<Wordboard, 2>            m_attack_table{};
    std::array<PieceList<Square>, 2>    m_piece_list_sq{};
    std::array<PieceList<PieceType>, 2> m_piece_list{};
    Byteboard                           m_board{};
    u64                                 m_hash{};
    u16                                 m_50mr{};
    u16                                 m_ply{};
    Color                               m_active_color{};
    Square                              m_enpassant = Square::invalid();
    std::array<RookInfo, 2>             m_rook_info;
    HashKey                             m_hash_key;
    HashKey                             m_pawn_key;
    std::array<HashKey, 2>              m_non_pawn_key;
    HashKey                             m_major_key;
    HashKey                             m_minor_key;

    void incrementally_remove_piece(bool color, PieceId id, Square sq, PsqtUpdates& updates);
    void incrementally_add_piece(bool color, Place p, Square sq, PsqtUpdates& updates);
    void incrementally_mutate_piece(
      bool old_color, PieceId old_id, Square sq, bool new_color, Place p, PsqtUpdates& updates);
    void
    incrementally_move_piece(bool color, Square from, Square to, Place p, PsqtUpdates& updates);

    void  remove_attacks(bool color, PieceId id);
    m8x64 toggle_rays(Square sq);
    void  add_attacks(bool color, PieceId id, Square sq, PieceType ptype);
    void  add_attacks(bool color, PieceId id, Square sq, PieceType ptype, m8x64 mask);
};

}  // namespace Clockwork

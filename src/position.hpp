#pragma once

#include <array>
#include <bit>
#include <cassert>
#include <iosfwd>

#include "board.hpp"
#include "move.hpp"
#include "square.hpp"
#include "util/types.hpp"
#include "util/vec.hpp"

namespace Clockwork {

template<typename T>
struct alignas(16) PieceList {
    std::array<T, 16> array{};

    constexpr T& operator[](PieceId id) {
        return array[id.raw];
    }
    constexpr T operator[](PieceId id) const {
        return array[id.raw];
    }

    [[nodiscard]] v128 to_vec() const {
        static_assert(sizeof(array) == sizeof(v128));
        return v128::load(array.data());
    }

    [[nodiscard]] u16 mask_valid() const {
        return to_vec().nonzero8();
    }

    [[nodiscard]] u16 mask_eq(PieceType ptype) const {
        return v128::eq8(to_vec(), v128::broadcast8(static_cast<u8>(ptype)));
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

    constexpr bool operator==(const RookInfo&) const = default;
};

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

    [[nodiscard]] Square king_sq(Color color) const {
        return piece_list_sq(color)[PieceId{0}];
    }

    [[nodiscard]] bool is_valid() const {
        return attack_table(m_active_color).read(king_sq(invert(m_active_color))) == 0;
    }

    [[nodiscard]] Position move(Move m) const;

    const std::array<Wordboard, 2> calc_attacks_slow();
    const std::array<u16, 2>       calc_attacks_slow(Square sq);

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

    void incrementally_remove_piece(bool color, PieceId id, Square sq);
    void incrementally_add_piece(bool color, Place p, Square sq);
    void
    incrementally_mutate_piece(bool old_color, PieceId old_id, Square sq, bool new_color, Place p);

    void remove_attacks(bool color, PieceId id);
    v512 toggle_rays(Square sq);
    void add_attacks(bool color, PieceId id, Square sq, PieceType ptype);
    void add_attacks(bool color, PieceId id, Square sq, PieceType ptype, v512 mask);
};

}

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

    constexpr T& operator[](PieceId id) { return array[id.raw]; }
    constexpr T  operator[](PieceId id) const { return array[id.raw]; }

    v128 toVec() const {
        static_assert(sizeof(array) == sizeof(v128));
        return v128::load(array.data());
    }

    u16 maskValid() const { return toVec().nonzero8(); }

    u16 maskEq(PieceType ptype) const {
        return v128::eq8(toVec(), v128::broadcast8(static_cast<u8>(ptype)));
    }

    constexpr bool operator==(const PieceList& other) const { return array == other.array; }
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

    constexpr bool isClear() const { return !aside.isValid() && !hside.isValid(); }

    constexpr bool operator==(const RookInfo&) const = default;
};

struct Position {
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

   public:
    constexpr Position() = default;

    const Byteboard& board() const { return m_board; }
    const Wordboard& attackTable(Color color) const {
        return m_attack_table[static_cast<usize>(color)];
    }
    const PieceList<PieceType>& pieceList(Color color) const {
        return m_piece_list[static_cast<usize>(color)];
    }
    const PieceList<Square>& pieceListSq(Color color) const {
        return m_piece_list_sq[static_cast<usize>(color)];
    }
    Color    activeColor() const { return m_active_color; }
    Square   enPassant() const { return m_enpassant; }
    RookInfo rookInfo(Color color) const { return m_rook_info[static_cast<usize>(color)]; }

    Square kingSq(Color color) const { return pieceListSq(color)[PieceId{0}]; }

    bool isValid() const {
        return attackTable(m_active_color)[kingSq(invert(m_active_color))] == 0;
    }

    Position move(Move m) const;

    const std::array<Wordboard, 2> calcAttacksSlow();
    const std::array<u16, 2>       calcAttacksSlow(Square sq);

    static std::optional<Position> parse(std::string_view str);
    static std::optional<Position> parse(std::string_view board,
                                         std::string_view color,
                                         std::string_view castle,
                                         std::string_view enpassant,
                                         std::string_view irreversible_clock,
                                         std::string_view ply);

    bool                 operator==(const Position&) const = default;
    friend std::ostream& operator<<(std::ostream& os, const Position& position);
};

}

#pragma once

#include "move.hpp"
#include "position.hpp"
#include "square.hpp"
#include "util/static_vector.hpp"

namespace Clockwork {

using MoveList = StaticVector<Move, 256>;

class MoveGen {
   public:
    explicit MoveGen(const Position& position) :
        m_active_color(position.activeColor()),
        m_position(position) {}

    void generateMoves(MoveList& moves);

   private:
    void write(MoveList& moves, Square sq, u16 piecemask, MoveFlags mf);
    void write(MoveList& moves, const Wordboard& at, u64 bb, u16 piecemask, MoveFlags mf);
    void writePawn(MoveList& moves, u64 bb, int shift, MoveFlags mf);

    const Color     m_active_color;
    const Position& m_position;
};

}

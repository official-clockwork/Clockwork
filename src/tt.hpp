#pragma once

#include "position.hpp"

namespace Clockwork {

struct TTEntry {
    HashKey key;
    Move    move;
};


struct TTData {
    Move move;
};

class TT {
public:
    static constexpr size_t DEFAULT_SIZE_MB = 16;
    static constexpr size_t TT_ALIGNMENT    = 64;

    TT(size_t mb = DEFAULT_SIZE_MB);
    ~TT();

    std::optional<TTData> probe(const Position& pos) const;
    void                  store(const Position& pos, Move move);
    void                  resize(size_t mb);
    void                  clear();

private:
    TTEntry* m_entries;
    size_t   m_size;
};

}

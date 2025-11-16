#pragma once

#include "position.hpp"
#include <array>
#include <atomic>
#include <bit>

namespace Clockwork {

enum Bound : u8 {
    None  = 0,
    Lower = 1,
    Upper = 2,
    Exact = 3,
};

struct TTEntry {
    u16  key16;
    Move move;
    i16  score;
    i16  eval;
    u8   depth;
    u8   info;

    [[nodiscard]] Bound bound() const {
        return static_cast<Bound>(info & 0b011);
    }
    [[nodiscard]] bool ttpv() const {
        return static_cast<bool>(info & 0b100);
    }
    [[nodiscard]] u8 age() const {
        return info >> 3;
    }
};

struct TTCluster {
    std::array<TTEntry, 3> entries;
    std::array<u8, 2>      padding;
};

struct TTClusterMemory {
    alignas(32) std::array<std::atomic<u64>, 4> data;

    [[nodiscard]] auto load() const -> TTCluster {
        std::array<u64, 4> out;
        out[0] = this->data[0].load(std::memory_order_relaxed);
        out[1] = this->data[1].load(std::memory_order_relaxed);
        out[2] = this->data[2].load(std::memory_order_relaxed);
        out[3] = this->data[3].load(std::memory_order_relaxed);
        return std::bit_cast<TTCluster>(out);
    }

    auto store(TTCluster cluster) {
        std::array<u64, 4> mem = std::bit_cast<std::array<u64, 4>>(cluster);
        this->data[0].store(mem[0], std::memory_order_relaxed);
        this->data[1].store(mem[1], std::memory_order_relaxed);
        this->data[2].store(mem[2], std::memory_order_relaxed);
        this->data[3].store(mem[3], std::memory_order_relaxed);
    }
};

static_assert(sizeof(TTEntry) == 10 * sizeof(u8));
static_assert(sizeof(TTCluster) == 32 * sizeof(u8));
static_assert(sizeof(TTClusterMemory) == 32 * sizeof(u8));

struct TTData {
    Value eval;
    Move  move;
    Value score;
    Depth depth;
    u8    info;

    [[nodiscard]] Bound bound() const {
        return static_cast<Bound>(info & 0b011);
    }
    [[nodiscard]] bool ttpv() const {
        return static_cast<bool>(info & 0b100);
    }
    [[nodiscard]] u8 age() const {
        return info >> 3;
    }
};

class TT {
public:
    static constexpr size_t DEFAULT_SIZE_MB = 16;
    static constexpr size_t TT_ALIGNMENT    = 64;

    static constexpr u8 MAX_AGE  = 32;
    static constexpr u8 AGE_MASK = 0x1F;

    TT(size_t mb = DEFAULT_SIZE_MB);
    ~TT();

    std::optional<TTData> probe(const Position& position, i32 ply) const;
    void                  store(const Position& position,
                                i32             ply,
                                Value           eval,
                                Move            move,
                                Value           score,
                                Depth           depth,
                                bool            ttpv,
                                Bound           bound);
    void                  resize(size_t mb);
    void                  clear();
    void                  increment_age();

private:
    TTClusterMemory* m_clusters;
    size_t           m_size;
    u8               m_age;
};

}  // namespace Clockwork

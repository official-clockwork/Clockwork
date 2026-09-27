#pragma once

#include "position.hpp"
#include "util/mem.hpp"
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

struct alignas(32) TTCluster {
public:
    static constexpr usize ENTRY_COUNT    = 3;
    static constexpr usize FRAGMENT_WIDTH = 21;
    static constexpr u64   FRAGMENT_MASK  = (1 << FRAGMENT_WIDTH) - 1;

    [[nodiscard]] TTEntry load(usize index) {
        u64 raw = std::atomic_ref{entries[index]}.load(std::memory_order_relaxed);
        return std::bit_cast<TTEntry>(raw);
    }

    void store(usize index, TTEntry entry) {
        u64 raw = std::bit_cast<u64>(entry);
        std::atomic_ref{entries[index]}.store(raw, std::memory_order_relaxed);
    }

    usize lookup(u64 fragment) {
        u64 needle   = fragment * FRAGMENTS_LSB;
        u64 haystack = std::atomic_ref{fragments}.load(std::memory_order_relaxed);
        u64 zeros    = needle ^ haystack;
        u64 matches  = (zeros - FRAGMENTS_LSB) & ~zeros & FRAGMENTS_MSB;
        return static_cast<usize>(std::countr_zero(matches)) / FRAGMENT_WIDTH;
    }

    u64 get_fragment(usize index) {
        u64   f     = std::atomic_ref{fragments}.load(std::memory_order_relaxed);
        usize shift = FRAGMENT_WIDTH * index;
        return (f >> shift) & FRAGMENT_MASK;
    }

    void set_fragment(usize index, u64 fragment) {
        u64   f     = std::atomic_ref{fragments}.load(std::memory_order_relaxed);
        usize shift = FRAGMENT_WIDTH * index;
        f &= ~(FRAGMENT_MASK << shift);
        f |= fragment << shift;
        std::atomic_ref{fragments}.store(f, std::memory_order_relaxed);
    }

private:
    static constexpr u64 FRAGMENTS_LSB = 0x0000'0400'0020'0001;
    static constexpr u64 FRAGMENTS_MSB = 0x4000'0200'0010'0000;

    std::array<u64, ENTRY_COUNT> entries;
    u64                          fragments;
};

static_assert(sizeof(TTEntry) == 8 * sizeof(u8));
static_assert(sizeof(TTCluster) == 32 * sizeof(u8));
static_assert(sizeof(TTCluster) == 32 * sizeof(u8));

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

    std::optional<TTData> probe(const Position& position, i32 ply);
    void                  store(const Position& position,
                                i32             ply,
                                Value           eval,
                                Move            move,
                                Value           score,
                                Depth           depth,
                                bool            ttpv,
                                Bound           bound);
    void                  resize(size_t mb, usize thread_count);
    void                  clear(usize thread_count);
    void                  increment_age();
    i32                   hashfull();
    TTCluster*            addr_key(const u64 key) const;


private:
    unique_ptr_huge_page<TTCluster[]> m_clusters;
    size_t                            m_size;
    u8                                m_age;
};

}  // namespace Clockwork

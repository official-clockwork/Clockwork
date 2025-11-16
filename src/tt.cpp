#include "tt.hpp"

namespace Clockwork {

static u16 shrink_key(HashKey key) {
    return static_cast<u16>(key);
}

static u64 mulhi64(u64 a, u64 b) {
    u128 result = static_cast<u128>(a) * static_cast<u128>(b);
    return static_cast<u64>(result >> 64);
}

void* aligned_alloc(size_t alignment, size_t size) {
#ifdef _WIN32
    return _aligned_malloc(size, alignment);
#else
    return std::aligned_alloc(alignment, size);
#endif
}

void aligned_free(void* ptr) {
    if (ptr == nullptr) {
        return;
    }
#ifdef _WIN32
    return _aligned_free(ptr);
#else
    return std::free(ptr);
#endif
}

[[nodiscard]] static u8 make_tt_info(bool is_tt_pv, Bound bound, u8 age) {
    return static_cast<u8>(bound) | (static_cast<u8>(static_cast<u8>(is_tt_pv) << 2))
         | static_cast<u8>(age << 3);
}

i16 score_to_tt(Value score, i32 ply) {
    if (score > VALUE_WIN) {
        return static_cast<i16>(score + ply);
    } else if (score < -VALUE_WIN) {
        return static_cast<i16>(score - ply);
    } else {
        return static_cast<i16>(score);
    }
}

Value score_from_tt(i16 ttScore, i32 ply) {
    if (ttScore > VALUE_WIN) {
        return static_cast<Value>(ttScore - ply);
    } else if (ttScore < -VALUE_WIN) {
        return static_cast<Value>(ttScore + ply);
    } else {
        return static_cast<Value>(ttScore);
    }
}

TT::TT(size_t mb) :
    m_clusters{nullptr},
    m_size{0},
    m_age{0} {
    resize(mb);
}

TT::~TT() {
    aligned_free(m_clusters);
}

std::optional<TTData> TT::probe(const Position& pos, i32 ply) const {
    size_t     idx     = mulhi64(pos.get_hash_key(), m_size);
    const auto cluster = this->m_clusters[idx].load();
    const auto key     = shrink_key(pos.get_hash_key());

    for (const auto entry : cluster.entries) {
        if (entry.key16 != key) {
            continue;
        }

        TTData data = {.eval  = entry.eval,
                       .move  = entry.move,
                       .score = score_from_tt(entry.score, ply),
                       .depth = static_cast<Depth>(entry.depth),
                       .info  = entry.info};

        return {data};
    }

    return {};
}

void TT::store(const Position& pos,
               i32             ply,
               Value           eval,
               Move            move,
               Value           score,
               Depth           depth,
               bool            ttpv,
               Bound           bound) {
    size_t     cluster_index = mulhi64(pos.get_hash_key(), m_size);
    auto       cluster       = this->m_clusters[cluster_index].load();
    const auto key           = shrink_key(pos.get_hash_key());

    auto   tte = cluster.entries[0];
    size_t idx = 0;

    if (!(tte.key16 == 0 || tte.key16 == key)) {
        for (size_t i = 1; i < 3; ++i) {
            const auto entry = cluster.entries[i];

            if (entry.key16 == 0 || entry.key16 == key) {
                tte = entry;
                idx = i;
                break;
            }

            if (tte.depth - ((MAX_AGE + m_age - tte.age()) & AGE_MASK) * 4
                > entry.depth - ((MAX_AGE + m_age - entry.age()) & AGE_MASK) * 4) {
                tte = entry;
                idx = i;
            }
        }
    }

    if (move == Move::none() && tte.key16 == key) {
        // if we don't have a best move, and the entry is for the same position,
        // then we should retain the best move from the previous entry.
        move = tte.move;
    }

    // give entries a bonus for type:
    // exact = 3, lower = 2, upper = 1
    i32 insert_flag_bonus = bound == Bound::Exact ? 3
                          : bound == Bound::Lower ? 2
                          : bound == Bound::Upper ? 1
                                                  : 0;
    i32 record_flag_bonus = tte.bound() == Bound::Exact ? 3
                          : tte.bound() == Bound::Lower ? 2
                          : tte.bound() == Bound::Upper ? 1
                                                        : 0;

    i32 age_differential = (MAX_AGE + m_age - tte.age()) & AGE_MASK;

    i32 insert_priority =
      depth + insert_flag_bonus + (age_differential * age_differential) / 4;  //+ i32::from(pv);
    i32 record_prority = tte.depth + record_flag_bonus;

    if (tte.key16 != key || (bound == Bound::Exact && tte.bound() != Bound::Exact)
        || insert_priority * 3 >= record_prority * 2) {
        tte.key16 = key;
        tte.move  = move;
        tte.score = score_to_tt(score, ply);
        tte.eval  = static_cast<i16>(eval);
        tte.depth = static_cast<u8>(depth);
        tte.info  = make_tt_info(ttpv, bound, m_age);

        // write back
        cluster.entries[idx] = tte;
        this->m_clusters[cluster_index].store(cluster);
    }
}

void TT::resize(size_t mb) {
    aligned_free(m_clusters);

    size_t bytes   = mb * 1024 * 1024;
    size_t entries = bytes / sizeof(TTClusterMemory);

    m_size     = entries;
    m_clusters = static_cast<TTClusterMemory*>(aligned_alloc(TT_ALIGNMENT, bytes));
    clear();
}

void TT::clear() {
    for (size_t i = 0; i < m_size; ++i) {
        m_clusters[i].data[0].store(0, std::memory_order_relaxed);
        m_clusters[i].data[1].store(0, std::memory_order_relaxed);
        m_clusters[i].data[2].store(0, std::memory_order_relaxed);
        m_clusters[i].data[3].store(0, std::memory_order_relaxed);
    }
}

void TT::increment_age() {
    const u8 new_age = (this->m_age + 1) & AGE_MASK;
    this->m_age      = new_age;
}

}  // namespace Clockwork

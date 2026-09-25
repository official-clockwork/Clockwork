#include "tt.hpp"
#include <algorithm>  // For std::min
#include <thread>

namespace Clockwork {

static u64 to_fragment(HashKey key) {
    return key & TTCluster::FRAGMENT_MASK;
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
    resize(mb, 1);
}

std::optional<TTData> TT::probe(const Position& pos, i32 ply) const {
    size_t      cluster_index = mulhi64(pos.get_hash_key(), m_size);
    const auto& cluster       = this->m_clusters[cluster_index];
    auto        fragment      = to_fragment(pos.get_hash_key());

    if (auto entry_index = cluster.lookup(fragment); entry_index < TTCluster::ENTRY_COUNT) {
        auto entry = cluster.load(entry_index);

        TTData data = {.eval  = entry.eval,
                       .move  = entry.move,
                       .score = score_from_tt(entry.score, ply),
                       .depth = static_cast<Depth>(entry.depth),
                       .info  = entry.info};

        return {data};
    }

    return {};
}

TTCluster* TT::addr_key(const u64 key) const {
    size_t idx = mulhi64(key, m_size);
    return &this->m_clusters[idx];
}

void TT::store(const Position& pos,
               i32             ply,
               Value           eval,
               Move            move,
               Value           score,
               Depth           depth,
               bool            ttpv,
               Bound           bound) {
    size_t cluster_index = mulhi64(pos.get_hash_key(), m_size);
    auto&  cluster       = this->m_clusters[cluster_index];
    auto   fragment      = to_fragment(pos.get_hash_key());

    TTEntry tte;
    size_t  entry_index;
    bool    fragment_match = false;

    if ((entry_index = cluster.lookup(fragment)) < TTCluster::ENTRY_COUNT) {
        fragment_match = true;
        tte            = cluster.load(entry_index);
    } else if ((entry_index = cluster.lookup(0)) < TTCluster::ENTRY_COUNT) {
        tte = TTEntry{};
    } else {
        tte         = cluster.load(0);
        entry_index = 0;
        for (size_t i = 1; i < 3; ++i) {
            auto entry = cluster.load(i);

            if (tte.depth - ((MAX_AGE + m_age - tte.age()) & AGE_MASK) * 4
                > entry.depth - ((MAX_AGE + m_age - entry.age()) & AGE_MASK) * 4) {
                tte         = entry;
                entry_index = i;
            }
        }
    }

    if (move == Move::none() && fragment_match) {
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

    if (!fragment_match || (bound == Bound::Exact && tte.bound() != Bound::Exact)
        || insert_priority * 3 >= record_prority * 2) {
        tte.move  = move;
        tte.score = score_to_tt(score, ply);
        tte.eval  = static_cast<i16>(eval);
        tte.depth = static_cast<u8>(depth);
        tte.info  = make_tt_info(ttpv, bound, m_age);

        // write back
        cluster.store(entry_index, tte);
        cluster.set_fragment(entry_index, fragment);
    }
}

void TT::resize(size_t mb, usize thread_count) {

    size_t bytes   = mb * 1024 * 1024;
    size_t entries = bytes / sizeof(TTCluster);

    m_size     = entries;
    m_clusters = make_unique_for_overwrite_huge_page<TTCluster[]>(m_size);
    clear(thread_count);
}

void TT::clear(usize thread_count) {
    usize max_threads = std::max(thread_count, usize(1));

    std::vector<std::thread> threads;
    threads.reserve(max_threads);

    for (usize t = 0; t < max_threads; ++t) {
        threads.emplace_back([this, t, max_threads]() {
            usize start = (m_size * t) / max_threads;
            usize end   = (m_size * (t + 1)) / max_threads;
            if (t == max_threads - 1) {
                end = m_size;
            }

            usize chunk_bytes = (end - start) * sizeof(TTCluster);
            std::memset(&m_clusters[start], 0, chunk_bytes);
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }
}

void TT::increment_age() {
    const u8 new_age = (this->m_age + 1) & AGE_MASK;
    this->m_age      = new_age;
}

i32 TT::hashfull() const {
    if (m_size == 0) {
        return 0;
    }

    constexpr i32 CLUSTERS_TO_SAMPLE = 1000;
    i32           occupied_count     = 0;

    size_t num_to_probe = std::min(static_cast<size_t>(CLUSTERS_TO_SAMPLE), m_size);
    if (num_to_probe == 0) {
        return 0;
    }

    for (size_t i = 0; i < num_to_probe; ++i) {
        const auto& cluster = this->m_clusters[i];
        for (size_t entry_index = 0; entry_index < TTCluster::ENTRY_COUNT; entry_index++) {
            auto entry = cluster.load(entry_index);
            if (entry.age() == m_age && entry.bound() != Bound::None) {
                occupied_count++;
            }
        }
    }

    // Return permill (0-1000)
    // Each cluster has 3 entries, so num_to_probe * 3 is the total number of entries sampled.
    return static_cast<i32>((static_cast<u64>(occupied_count) * 1000) / (num_to_probe * 3));
}

}  // namespace Clockwork

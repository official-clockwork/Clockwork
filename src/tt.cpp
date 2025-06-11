#include "tt.hpp"

namespace Clockwork {

uint64_t mulhi64(uint64_t a, uint64_t b) {
    unsigned __int128 result =
      static_cast<unsigned __int128>(a) * static_cast<unsigned __int128>(b);
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

TT::TT(size_t mb) :
    m_entries{nullptr},
    m_size{0} {
    resize(mb);
}

TT::~TT() {
    aligned_free(m_entries);
}

std::optional<TTData> TT::probe(const Position& pos) const {
    size_t      idx   = mulhi64(pos.get_hash_key(), m_size);
    const auto& entry = m_entries[idx];
    if (entry.key == pos.get_hash_key()) {
        TTData data = {.move = entry.move};
        return {data};
    }
    return {};
}

void TT::store(const Position& pos, Move move) {
    size_t idx   = mulhi64(pos.get_hash_key(), m_size);
    auto&  entry = m_entries[idx];
    entry.key    = pos.get_hash_key();
    entry.move   = move;
}

void TT::resize(size_t mb) {
    aligned_free(m_entries);

    size_t entries = mb * 1024 * 1024 / sizeof(TTEntry);

    m_size    = entries;
    m_entries = static_cast<TTEntry*>(aligned_alloc(TT_ALIGNMENT, entries * sizeof(TTEntry)));
    clear();
}

void TT::clear() {
    std::fill(m_entries, m_entries + m_size, TTEntry{});
}

}

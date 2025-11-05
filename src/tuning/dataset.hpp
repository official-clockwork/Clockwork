#pragma once

#include "graph.hpp"
#include "value.hpp"

#include <cstring>
#include <fcntl.h>
#include <memory>
#include <stdexcept>
#include <string>
#include <sys/mman.h>
#include <unistd.h>
#include <vector>

namespace Clockwork::Autograd {

class Dataset {
private:
    std::string              m_file_path;
    char*                    m_mapped_data;
    size_t                   m_file_size;
    std::vector<std::string> m_lines;

    void map_file();
    void parse_lines();

public:
    Dataset(const std::string& file_path);
    ~Dataset();

    const std::vector<std::string>& lines() const {
        return m_lines;
    }
    size_t size() const {
        return m_lines.size();
    }
};

class DataLoader {
private:
    const Dataset& m_dataset;
    size_t         m_batch_size;
    size_t         m_index;

public:
    DataLoader(const Dataset& dataset, size_t batch_size);

    bool                     has_next() const;
    std::vector<std::string> next_batch();
    void                     reset();
};

}  // namespace Clockwork::Autograd

#pragma once

#include "graph.hpp"
#include "value.hpp"
#include <memory>
#include <string>
#include <vector>

namespace Clockwork {
namespace Autograd {
class Dataset {
private:
    std::vector<std::string> m_lines;

public:
    Dataset(std::string load_dir) :
        m_lines({}) {};
}
}
}

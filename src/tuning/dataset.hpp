#pragma once

#include "graph.hpp"
#include "value.hpp"
#include <memory>
#include <vector>
#include <string>

namespace Clockwork {
namespace Autograd {
    class Dataset {
        private:
        std::vector<std::string> m_lines;

        public:
        Dataset(std::string load_dir) : m_lines({}){};


    }
}
}

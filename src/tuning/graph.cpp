#include "tuning/graph.hpp"
#include "tuning/globals.hpp"

namespace Clockwork::Autograd {

Graph::Graph() {
    for (ValuePlaceholder* placeholder : Globals::get().get_parameters()) {
        register_param(std::make_shared<Value>(placeholder->default_value()));
    }
    for (PairPlaceholder* placeholder : Globals::get().get_pair_parameters()) {
        register_param(
          std::make_shared<Pair>(placeholder->default_value(), placeholder->constant()));
    }
}

}  // namespace Clockwork::Autograd

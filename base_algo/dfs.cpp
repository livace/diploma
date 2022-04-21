#include "dfs.h"

namespace base_algo {

namespace {

std::vector<bool> dfs(graph::Graph& graph, graph::Vertex root) {
  return std::vector<bool>(graph.vertices().size(), false);
}

}

} // namespace base_algo

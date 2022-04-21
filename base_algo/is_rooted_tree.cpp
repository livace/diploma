#include "is_rooted_tree.h"

#include "dfs.h"

#include "util/assert.h"

namespace base_algo {

bool isRootedTree(const graph::Graph& graph, graph::Vertex root) {
  if (graph.edges().size() + 1 != graph.vertices().size()) {
    return false;
  }
  auto visited = inverseDfs(graph, root);
  return std::all_of(visited.begin(), visited.end(), [](const auto& x) { return x.second; });
}

} // namespace base_algo

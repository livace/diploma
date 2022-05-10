#include "is_connected.h"

#include "dfs.h"

namespace base_algo {

bool isConnected(const graph::Graph& graph) {
  auto visited = dfs(graph, graph.vertices()[0]);
  return std::all_of(visited.begin(), visited.end(), [](const auto& x) { return x.second; });
}

} // namespace base_algo

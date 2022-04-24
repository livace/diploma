#include "is_undirected.h"

namespace base_algo {

bool isUndirected(const graph::Graph& graph) {
  for (auto edge : graph.edges()) {
    if (!graph.has(graph::Edge(edge.to(), edge.from()))) {
      return false;
    }
  }

  return true;
}

} // namespace base_algo

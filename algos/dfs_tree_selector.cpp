#include "dfs_tree_selector.h"

namespace algos {

DfsTreeSelector::DfsTreeSelector(const graph::Graph& graph) : graph_(graph) {}

void DfsTreeSelector::visit(graph::Vertex vertex) {
  tree_.addVertex(vertex);
  visited_.insert(vertex);

  for (auto connection : graph_.inversedConnections(vertex)) {
    if (!visited_.count(connection)) {
      visit(connection);

      tree_.addEdge({connection, vertex});
    }
  }
}

graph::Graph DfsTreeSelector::get() {
  return tree_;
}

} // namespace algos

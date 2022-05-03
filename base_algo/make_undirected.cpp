#include "without.h"

#include "graph/builder.h"

namespace base_algo {

graph::Graph makeUndirected(const graph::Graph& graph) {
  graph::Graph result;
  for (auto vertex : graph.vertices()) {
    result.addVertex(vertex);
  }

  for (auto edge : graph.edges()) {
    if (graph.has({edge.to(), edge.from()})) {
      result.addEdge(edge);
    }
  }

  return result;
}

} // namespace base_algo

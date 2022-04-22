#include "without.h"

#include "graph/builder.h"

namespace base_algo {

graph::Graph without(const graph::Graph& graph, graph::Vertex vertex) {
  graph::Builder builder;

  for (auto graph_vertex : graph.vertices()) {
    if (graph_vertex != vertex) {
      builder.addVertex(vertex);
    }
  }

  for (auto edge : graph.edges()) {
    if (edge.from() != vertex && edge.to() != vertex) {
      builder.addEdge(edge);
    }
  }

  return builder.build();
}

} // namespace base_algo

#include "subtract.h"

#include "graph/graph.h"
#include "graph/builder.h"

namespace base_algo {

graph::Graph join(const graph::Graph& lhs, const graph::Graph& rhs) {
  graph::Builder builder;

  for (auto vertex : lhs.vertices()) {
    builder.addVertex(vertex);
  }
  for (auto vertex : rhs.vertices()) {
    builder.addVertex(vertex);
  }

  for (auto edge : lhs.edges()) {
    builder.addEdge(edge);
  }

  for (auto edge : rhs.edges()) {
    builder.addEdge(edge);
  }

  return builder.build();
}

} // namespace base_algo

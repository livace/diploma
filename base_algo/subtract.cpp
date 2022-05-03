#include "subtract.h"

#include "graph/graph.h"
#include "graph/builder.h"

namespace base_algo {

graph::Graph subtract(const graph::Graph& from, const graph::Graph& subgraph) {
  graph::Builder builder;

  for (auto vertex : from.vertices()) {
    builder.addVertex(vertex);
  }

  for (auto edge : from.edges()) {
    if (!subgraph.has(edge)) {
      builder.addEdge(edge);
    }
  }

  return builder.build();
}

} // namespace base_algo

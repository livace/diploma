#include "intersect.h"

#include "graph/graph.h"

namespace base_algo {

graph::Graph intersect(graph::Graph lhs, graph::Graph rhs) {
  graph::Graph graph;

  for (auto vertex : lhs.vertices()) {
    if (!rhs.has(vertex)) {
      continue;
    }

    graph.addVertex(vertex);

    for (auto connection : lhs.connections(vertex)) {
      if (rhs.has({vertex, connection})) {
        graph.addEdge({vertex, connection});
      }
    }
  }

  return graph;
}

} // namespace base_algo

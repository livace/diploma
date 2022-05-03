#include "intersect.h"

#include "graph/graph.h"

namespace base_algo {

graph::Graph intersect(graph::Graph lhs, graph::Graph rhs) {
  std::unordered_map<graph::Vertex, std::unordered_set<graph::Vertex>> connectivity_list;

  for (auto vertex : lhs.vertices()) {
    if (!rhs.has(vertex)) {
      continue;
    }

    connectivity_list[vertex];

    for (auto connection : lhs.connections(vertex)) {
      if (rhs.has(graph::Edge(vertex, connection))) {
        connectivity_list[vertex].insert(connection);
      }
    }
  }

  return graph::Graph(connectivity_list);
}

} // namespace base_algo

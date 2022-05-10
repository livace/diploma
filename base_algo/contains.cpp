#include "contains.h"
#include "intersect.h"

namespace base_algo {

bool contains(const graph::Graph& graph, const graph::Graph& subgraph) {
  return intersect(graph, subgraph) == subgraph;
}

} // namespace base_algo

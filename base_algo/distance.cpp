#include "distance.h"

#include "util/assert.h"
#include "util/log.h"

namespace base_algo {

size_t distance(const graph::Graph& graph, graph::Vertex from, graph::Vertex to) {
  size_t result = 0;

  while (from != to) {
    ASSERT_FATAL(graph.connections(from).size() == 1);

    result++;
    from = *graph.connections(from).begin();
  }

  return result;
}

} // namespace base_algo

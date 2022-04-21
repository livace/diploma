#include "is_rooted_tree.h"

#include "dfs.h"
#include "intersect.h"

#include "util/assert.h"

namespace base_algo {

bool isNested(const graph::Graph& graph, const graph::Graph& into) {
  return intersect(graph, into) == graph;

}

} // namespace base_algo

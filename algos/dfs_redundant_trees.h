#pragma once

#include "graph/graph.h"
#include "util/expect.h"

#include <utility>

namespace algos {

enum class EdgeSkippingPolicy {
  Simple,
  Ordered
};

std::istream& operator>>(std::istream& is, EdgeSkippingPolicy& edge_skipping_policy);
std::ostream& operator<<(std::ostream& os, EdgeSkippingPolicy edge_skipping_policy);

util::Expected<std::vector<graph::Graph>, std::string> makeDfsRedundantTrees(
  size_t cnt,
  uint64_t seed,
  const graph::Graph& graph,
  graph::Vertex root,
  double max_depth,
  EdgeSkippingPolicy edge_skipping_policy,
  bool reset_depth,
  int vertex_limit,
  bool greedy_size);

} // namespace algos

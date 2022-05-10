#pragma once

#include "graph/graph.h"
#include "util/expect.h"

#include <utility>

namespace algos {

util::Expected<std::vector<graph::Graph>, std::string> makeDfsRedundantTrees(
  size_t cnt,
  uint64_t seed,
  const graph::Graph& graph,
  graph::Vertex root);

} // namespace algos

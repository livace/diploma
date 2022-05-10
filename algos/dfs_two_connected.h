#pragma once

#include "graph/graph.h"
#include "util/expect.h"

#include <utility>

namespace algos {

util::Expected<std::tuple<graph::Graph, graph::Graph, graph::Graph>, std::string> makeDfsTwoConnectedThreeRedundantTrees(
  const graph::Graph& graph,
  graph::Vertex root,
  int strict_cnt);

} // namespace algos

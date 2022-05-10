#pragma once

#include "graph/graph.h"
#include "util/expect.h"

#include <utility>

namespace algos {

util::Expected<std::tuple<graph::Graph, graph::Graph, graph::Graph>, std::string> makeDfsGadagThreeRedundantTrees(
  const graph::Graph& graph,
  graph::Vertex root);

} // namespace algos

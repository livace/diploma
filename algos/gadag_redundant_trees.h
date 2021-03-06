#pragma once

#include "graph/graph.h"
#include "util/expect.h"

#include <utility>

namespace algos {

std::pair<graph::Graph, graph::Graph> makeTwoRedundantTrees(
  const graph::Graph& gadag,
  std::unordered_map<graph::Vertex, graph::Vertex> local_root,
  graph::Vertex root);

util::Expected<std::tuple<graph::Graph, graph::Graph, graph::Graph>, std::string> makeGadagThreeRedundantTrees(
  const graph::Graph& graph,
  graph::Vertex root);

} // namespace algos

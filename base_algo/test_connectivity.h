#pragma once

#include "graph/graph.h"

#include <random>

namespace base_algo {

size_t testConnectivity(
  const graph::Graph& initial_graph,
  const std::vector<graph::Graph>& trees,
  size_t num_broken,
  std::mt19937_64& rng,
  graph::Vertex root);

}

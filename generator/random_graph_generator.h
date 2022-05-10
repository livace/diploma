#pragma once

#include "generator.h"

#include <random>

namespace generator {

class RandomGraphGenerator : public Generator {
public:
  RandomGraphGenerator(
    int connectivity,
    int vertices,
    int edges,
    uint64_t seed);

  graph::Graph generate() override;

private:
  graph::Graph generateTree(const graph::Graph& previous_trees);
  void addEdges(graph::Graph& graph, int edges);

private:
  int connectivity_;
  int vertices_;
  int edges_;

  std::mt19937_64 rng_;
};

} // namespace generator

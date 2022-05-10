#include "dfs_redundant_trees.h"

#include "base_algo/subtract.h"
#include "base_algo/intersect.h"
#include "base_algo/join.h"

#include "util/expect.h"
#include "util/assert.h"
#include "util/log.h"

#include <iostream>
#include <random>

namespace algos {

namespace {

class DfsVisitor {
public:
  DfsVisitor(
    std::mt19937_64& rng,
    const graph::Graph& graph,
    const graph::Graph& already_selected
  ) : rng_(rng), graph_(graph), already_selected_(already_selected) {}

  void run(graph::Vertex vertex) {
    visit(vertex);
    while (visited_.size() != graph_.vertices().size()) {
      auto edge = popRandom();

      if (!visited_.count(edge.from())) {
        tree_.addEdge(edge);
        visit(edge.from());
      }
    }
  }

  void visit(graph::Vertex vertex) {
    tree_.addVertex(vertex);
    visited_.insert(vertex);

    for (auto connection : graph_.inversedConnections(vertex)) {
      if (already_selected_.has({connection, vertex})) {
        skipped_edges_.push_back({connection, vertex});
        continue;
      }

      if (!visited_.count(connection)) {
        visit(connection);

        tree_.addEdge({connection, vertex});
      }
    }
  }

  graph::Graph get() {
    return tree_;
  }

  graph::Edge popRandom() {
    ASSERT_FATAL(!skipped_edges_.empty());
    std::uniform_int_distribution<std::mt19937_64::result_type> edge_dist(0, skipped_edges_.size() - 1);
    auto edge_id = edge_dist(rng_);

    std::swap(skipped_edges_[edge_id], skipped_edges_.back());

    auto res = skipped_edges_.back();
    skipped_edges_.pop_back();

    return res;
  }

private:
  std::mt19937_64& rng_;

  std::unordered_set<graph::Vertex> visited_;
  const graph::Graph& graph_;
  const graph::Graph& already_selected_;

  std::vector<graph::Edge> skipped_edges_;

  graph::Graph tree_;
};

} // namespace

util::Expected<std::vector<graph::Graph>, std::string> makeDfsRedundantTrees(
  size_t cnt,
  uint64_t seed,
  const graph::Graph& graph,
  graph::Vertex root) {

  std::vector<graph::Graph> trees;
  graph::Graph already_selected;

  std::mt19937_64 rng(seed);

  for (size_t i = 0; i < cnt; i++) {
    DfsVisitor visitor(rng, graph, already_selected);

    visitor.run(root);
    auto tree = visitor.get();

    already_selected = base_algo::join(already_selected, tree);

    trees.push_back(std::move(tree));
  }

  return trees;
}

} // namespace algos

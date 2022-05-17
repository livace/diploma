#include "dfs_redundant_trees.h"

#include "base_algo/subtract.h"
#include "base_algo/intersect.h"
#include "base_algo/join.h"

#include "util/expect.h"
#include "util/assert.h"
#include "util/log.h"

#include "dot/dot_serializer.h"

#include <boost/lexical_cast.hpp>

#include <iostream>
#include <random>

namespace algos {

namespace {

class EdgeContainer {
public:
  virtual void add(graph::Edge edge, int order) = 0;
  virtual graph::Edge pop() = 0;

  virtual ~EdgeContainer() = default;
};

class SimpleEdgeContainer : public EdgeContainer {
public:
  SimpleEdgeContainer(std::mt19937_64& rng) : rng_(rng) {}

  void add(graph::Edge edge, int) override {
    edges_.push_back(edge);
  }

  graph::Edge pop() override {
    ASSERT_FATAL(!edges_.empty());

    std::uniform_int_distribution<std::mt19937_64::result_type> edge_dist(0, edges_.size() - 1);
    auto edge_id = edge_dist(rng_);

    std::swap(edges_[edge_id], edges_.back());

    auto res = edges_.back();
    edges_.pop_back();

    // DEBUG() << "Pop " << res.from() << " " << res.to();

    return res;
  }

private:
  std::mt19937_64& rng_;

  std::vector<graph::Edge> edges_;
};

class OrderedEdgeContainer : public EdgeContainer {
public:
  OrderedEdgeContainer(std::mt19937_64& rng) : rng_(rng) {}

  void add(graph::Edge edge, int order) override {
    edges_[order].push_back(edge);
  }

  graph::Edge pop() override {
    ASSERT_FATAL(!edges_.empty());

    auto& first = edges_.begin()->second;

    ASSERT_FATAL(!first.empty());

    std::uniform_int_distribution<std::mt19937_64::result_type> edge_dist(0, first.size() - 1);
    auto edge_id = edge_dist(rng_);

    std::swap(first[edge_id], first.back());

    auto res = first.back();
    // DEBUG() << "Pop " << res.from() << " " << res.to() << " with order " << edges_.begin()->first;

    first.pop_back();

    if (first.empty()) {
      edges_.erase(edges_.begin());
    }

    // DEBUG() << NAMED(edges_.size());
    return res;
  }

private:
  std::mt19937_64& rng_;

  std::map<int, std::vector<graph::Edge>> edges_;
};

class DfsVisitor {
public:
  DfsVisitor(
    std::mt19937_64& rng,
    const graph::Graph& graph,
    const std::vector<graph::Graph>& already_selected,
    double max_depth,
    std::unique_ptr<EdgeContainer> skipped_edges,
    bool reset_depth,
    int vertex_limit,
    bool greedy_size
  )
  : rng_(rng)
  , graph_(graph)
  , already_selected_(already_selected)
  , max_depth_(max_depth * graph.vertices().size())
  , skipped_edges_(std::move(skipped_edges))
  , reset_depth_(reset_depth)
  , vertex_limit_(vertex_limit)
  , greedy_size_(greedy_size) {}

  void run(graph::Vertex vertex) {
    dot::DotSerializer serializer("dfs");

    serializer.markVertex(vertex, "red");
    serializer.addGraph(graph_, "initial_graph", "black");
    visit(vertex, 0);
    size_t cnt = 1;
    serializer.addGraph(tree_, "tree_" + boost::lexical_cast<std::string>(cnt), "red");

    while (visited_.size() != graph_.vertices().size()) {
      auto edge = skipped_edges_->pop();

      if (!visited_.count(edge.from())) {
        tree_.addEdge(edge);
        if (reset_depth_) {
          visit(edge.from(), 0);
        } else {
          visit(edge.from(), depth_[edge.to()] + 1);
        }

        cnt++;
        serializer.addGraph(tree_, "tree_" + boost::lexical_cast<std::string>(cnt), "red");
      }
    }

    serializer.serialize(std::cout);
  }

  void visit(graph::Vertex vertex, size_t depth) {
    tree_.addVertex(vertex);
    visited_.insert(vertex);

    depth_[vertex] = depth;

    auto set_connections = graph_.inversedConnections(vertex);
    auto connections = std::vector(set_connections.begin(), set_connections.end());
    std::shuffle(connections.begin(), connections.end(), rng_);

    auto limit = connections.size();
    if (vertex_limit_ > 0) {
      limit = std::min<size_t>(connections.size(), vertex_limit_);
    } else if (vertex_limit_ < 0) {
      limit = connections.size() - std::min<size_t>(-vertex_limit_, connections.size());
    }

    // DEBUG() << NAMED(limit) << " " << NAMED(connections.size()) << " " << NAMED(vertex_limit_);
    for (auto connection : std::vector(connections.begin() + limit, connections.end())) {
      // DEBUG() << "Skip " << connection << " " << vertex;
      skipped_edges_->add({connection, vertex}, 1);
    }

    for (auto connection : std::vector(connections.begin(), connections.begin() + limit)) {
      int already_selected_cnt = 0;
      for (const auto& graph : already_selected_) {
        if (graph.has({connection, vertex})) {
          already_selected_cnt++;
        }
      }

      INFO() << "order: " << (greedy_size_ ? already_selected_cnt + 1 : 2);

      if (already_selected_cnt > 0) {
        skipped_edges_->add({connection, vertex}, greedy_size_ ? already_selected_cnt + 1 : 2);
        continue;
      }

      // DEBUG() << NAMED(depth) << " " << NAMED(max_depth_);

      if (depth > max_depth_) {
        skipped_edges_->add({connection, vertex}, 1);
        continue;
      }

      if (!visited_.count(connection)) {
        visit(connection, depth + 1);

        tree_.addEdge({connection, vertex});
      }
    }
  }

  graph::Graph get() {
    return tree_;
  }

private:
  std::mt19937_64& rng_;
  size_t max_depth_;

  std::unordered_map<graph::Vertex, size_t> depth_;
  std::unordered_set<graph::Vertex> visited_;
  const graph::Graph& graph_;
  const std::vector<graph::Graph>& already_selected_;
  std::unique_ptr<EdgeContainer> skipped_edges_;
  bool reset_depth_;
  int vertex_limit_;
  bool greedy_size_;

  graph::Graph tree_;
};

} // namespace

std::istream& operator>>(std::istream& is, EdgeSkippingPolicy& edge_skipping_policy) {
  std::string s;
  is >> s;

  if (s == "simple") {
    edge_skipping_policy = EdgeSkippingPolicy::Simple;
  } else if (s == "ordered") {
    edge_skipping_policy = EdgeSkippingPolicy::Ordered;
  } else {
    ASSERT_FATAL(false, "Unknown policy " << s);
  }

  return is;
}

std::ostream& operator<<(std::ostream& os, EdgeSkippingPolicy edge_skipping_policy) {
  switch (edge_skipping_policy) {
  case EdgeSkippingPolicy::Simple: return os << "simple";
  case EdgeSkippingPolicy::Ordered: return os << "ordered";
  }
}

util::Expected<std::vector<graph::Graph>, std::string> makeDfsRedundantTrees(
  size_t cnt,
  uint64_t seed,
  const graph::Graph& graph,
  graph::Vertex root,
  double max_depth,
  EdgeSkippingPolicy edge_skipping_policy,
  bool reset_depth,
  int passed_vertex_limit,
  bool greedy_size) {

  std::vector<graph::Graph> trees;
  std::vector<graph::Graph> already_selected;

  std::mt19937_64 rng(seed);

  int vertex_limit = 0;

  if (passed_vertex_limit < 0) {
    vertex_limit = -cnt + 1;
  } else if (passed_vertex_limit > 0) {
    vertex_limit = 1;
  }

  for (size_t i = 0; i < cnt; i++) {
    std::unique_ptr<EdgeContainer> container = [edge_skipping_policy, &rng]() -> std::unique_ptr<EdgeContainer> {
      switch (edge_skipping_policy) {
      case EdgeSkippingPolicy::Simple: return std::make_unique<SimpleEdgeContainer>(rng);
      case EdgeSkippingPolicy::Ordered: return std::make_unique<OrderedEdgeContainer>(rng);
      }
    }();

    DfsVisitor visitor(
      rng,
      graph,
      already_selected,
      max_depth,
      std::move(container),
      reset_depth,
      vertex_limit,
      greedy_size);

    visitor.run(root);
    auto tree = visitor.get();

    already_selected.push_back(tree);

    trees.push_back(std::move(tree));

    if (vertex_limit != 0) {
      vertex_limit++;
    }
  }

  std::map<int, int> cnts;
  for (auto edge : graph.edges()) {
    int edge_cnt = 0;
    for (auto& tree: already_selected) {
      if (tree.has(edge)) {
        edge_cnt++;
      }
    }
    cnts[edge_cnt]++;
  }

  for (auto [k, v] : cnts) {
    DEBUG() << v << " edges are in " << k << " graphs at the same time";
  }

  return trees;
}

} // namespace algos

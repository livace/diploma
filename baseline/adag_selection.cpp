#include "adag_selection.h"

#include "base_algo/is_undirected.h"
#include "util/assert.h"
#include "util/expect.h"

namespace baseline {

namespace {

class CycleSelector {
public:
  CycleSelector(const graph::Graph& graph, graph::Vertex vertex) : graph_(graph), root_(vertex) {}

  util::Expected<std::vector<graph::Vertex>, util::Error> get() {
    bool found_cycle = false;
    for (auto connection : graph_.connections(vertex)) {
      if (dfs(connection, root_)) {
        found_cycle = true;
        break;
      }
    }

    EXPECT(found_cycle, "Failed to find cycle");

    std::vector<graph::Vertex> result;

    auto current = previous_[root];
    while (current != root_) {
      result.push_back(current);
      current = previous_[current];
    }

    result.push_back(root_);

    std::reverse(result.begin(), result.end());

    return result;
  }

private:
  bool dfs(graph::Vertex vertex, graph::Vertex previous) {
    if (previous_.count(vertex)) {
      return false;
    }

    previous_[vertex] = previous;

    if (vertex == root_) {
      return true;
    }

    for (auto connection : graph_.connections(vertex)) {
      if (connection != previous) {
        if (dfs(connection, vertex)) {
          return true;
        }
      }
    }

    return false;
  }

private:
  graph::Graph graph_;
  graph::Vertex root_;
  graph::Vertex previous_;
}

class AdagSelector {
public:
  AdagSelector(const graph::Graph& graph, graph::Vertex vertex) : graph_(graph), root_(vertex) {

  }

  graph::Graph get() {
    auto cycle = CycleSelector(graph_, root_).get();



  }

private:
  std::unordered_map<graph::Vertex, int> depth_;
  std::unordered_map<graph::Vertex, bool> visited_;

  graph::Graph graph_;
  graph::Vertex root_;
};

} // namespace

util::Expected<graph::Graph, util::Error> selectAdag(const graph::Graph& graph, graph::Vertex vertex) {
  EXPECT(base_algo::isUndirected(graph));



  return graph;
}

} // namespace baseline

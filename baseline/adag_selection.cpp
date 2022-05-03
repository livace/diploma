#include "adag_selection.h"

#include "graph/builder.h"

#include "base_algo/is_undirected.h"
#include "base_algo/make_undirected.h"

#include "util/assert.h"
#include "util/expect.h"

namespace baseline {

AdagSelector::AdagSelector(const graph::Graph& graph, graph::Vertex vertex) : graph_(graph), root_(vertex) {
  for (auto vertex : graph_.vertices()) {
    gadag_.addVertex(vertex);
    dfs_tree_.addVertex(vertex);
  }
}

util::Expected<void, util::Error> AdagSelector::build() {
  graph_ = base_algo::makeUndirected(graph_);

  dfs(root_);
  selectGadag();

  return {};
}

graph::Graph AdagSelector::get() {
  return gadag_;
}

graph::Graph AdagSelector::getDfsTree() {
  return dfs_tree_;
}

bool AdagSelector::dfs(graph::Vertex vertex) {
  if (visited_[vertex]) {
    return false;
  }

  visited_[vertex] = true;

  dfs_tree_.addVertex(vertex);

  depth_[vertex] = current_depth_++;
  lowpoint_number_[vertex] = graph_.vertices().size();

  for (auto connection : graph_.connections(vertex)) {
    if (dfs(connection)) {
      dfs_tree_.addEdge({vertex, connection});
      parent_[connection] = vertex;

      if (lowpoint_number_[vertex] > lowpoint_number_[connection]) {
        lowpoint_number_[vertex] = lowpoint_number_[connection];
        lowpoint_source_[vertex] = connection;
      }
    }
  }

  for (auto connection : graph_.connections(vertex)) {
    if (dfs_tree_.has({vertex, connection})) {
      continue;
    }

    if (lowpoint_number_[vertex] > depth_[connection]) {
      lowpoint_number_[vertex] = depth_[connection];
      lowpoint_source_[vertex] = connection;
    }
  }

  ASSERT_FATAL(lowpoint_number_[vertex] < graph_.vertices().size());

  return true;
}

void AdagSelector::selectGadag() {
  std::vector<graph::Vertex> stack;

  ready_.insert(root_);
  stack.push_back(root_);

  while (!stack.empty()) {
    auto current = stack.back();
    stack.pop_back();

    for (auto child : dfs_tree_.connections(current)) {
      if (!ready_.count(child)) {
        std::vector<graph::Vertex> helper_stack;

        gadag_.addEdge({current, child});
        while (!ready_.count(child)) {
          helper_stack.push_back(child);
          gadag_.addEdge({child, lowpoint_source_.at(child)});
          child = lowpoint_source_.at(child);
        }

        std::reverse(helper_stack.begin(), helper_stack.end());
        for (auto vertex : helper_stack) {
          stack.push_back(vertex);
          ready_.insert(vertex);
        }

        if (current == child) {
          for (auto vertex : helper_stack) {
            local_root_[vertex] = current;
          }
        } else {
          for (auto vertex : helper_stack) {
            local_root_[vertex] = local_root_[current];
          }
        }
      }
    }

    for (auto neighbor : graph_.connections(current)) {
      if (!ready_.count(neighbor)) {
        if (dfs_tree_.has({current, neighbor})) {
          continue;
        }

        std::vector<graph::Vertex> helper_stack;
        gadag_.addEdge({current, neighbor});
        while (!ready_.count(neighbor)) {
          helper_stack.push_back(neighbor);
          gadag_.addEdge({neighbor, parent_.at(neighbor)});
          neighbor = parent_.at(neighbor);
        }

        std::reverse(helper_stack.begin(), helper_stack.end());
        for (auto vertex : helper_stack) {
          stack.push_back(vertex);
          ready_.insert(vertex);
        }

        for (auto vertex : helper_stack) {
          local_root_[vertex] = local_root_[neighbor];
        }
      }
    }
  }
}

} // namespace baseline

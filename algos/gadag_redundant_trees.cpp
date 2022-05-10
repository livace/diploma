#include "gadag_redundant_trees.h"

#include "adag_selection.h"
#include "build_gadag_tree.h"

#include "base_algo/is_rooted_tree.h"
#include "base_algo/subtract.h"
#include "base_algo/intersect.h"

#include "dot/dot_serializer.h"

#include "util/assert.h"
#include "util/log.h"

#include <queue>

namespace algos {

// std::pair<graph::Graph, graph::Graph> makeTwoRedundantTrees(
//   const graph::Graph& gadag,
//   graph::Vertex root
// ) {
//   auto primary_tree = buildGadagTree(gadag, root, /* inverse */ false);
//   ASSERT_FATAL(base_algo::isRootedTree(primary_tree, root));
//   auto secondary_tree = buildGadagTree(gadag, root, /* inverse */ true);
//   ASSERT_FATAL(base_algo::isRootedTree(secondary_tree, root));

//   return {
//     primary_tree,
//     secondary_tree
//   };
// }

util::Expected<std::tuple<graph::Graph, graph::Graph, graph::Graph>, std::string> makeGadagThreeRedundantTrees(
  const graph::Graph& graph,
  graph::Vertex root
) {
  (void) graph;
  (void) root;

  EXPECT(false, "Not implemented");

  // auto selector = algos::AdagSelector(graph, root);

  // {
  //   auto res = selector.build();
  //   PROPAGATE_ERROR(res);
  // }

  // auto gadag = selector.get();

  // auto primary_tree = buildGadagTree(gadag, root, /* inverse */ false);
  // ASSERT_FATAL(base_algo::isRootedTree(primary_tree, root));
  // auto secondary_tree = buildGadagTree(gadag, root, /* inverse */ true);
  // ASSERT_FATAL(base_algo::isRootedTree(secondary_tree, root));

  // auto remaining_graph = base_algo::subtract(base_algo::subtract(graph, primary_tree), secondary_tree);

  // auto third_tree = buildGadagTree(remaining_graph, root, /* inverse */ false);

  // ASSERT_FATAL(base_algo::intersect(remaining_graph, third_tree) == third_tree);

  // EXPECT(base_algo::isRootedTree(third_tree, root), "Failed to select third tree");
  // EXPECT(third_tree.vertices().size() == graph.vertices().size(), "Failed to select third tree");

  // return {{
  //   primary_tree,
  //   secondary_tree,
  //   third_tree
  // }};
}


namespace {

std::pair<std::pair<graph::Vertex, graph::Vertex>, graph::Graph> splitGadag(
  const graph::Graph& gadag,
  graph::Vertex root) {

  graph::Graph new_gadag;

  auto r_plus = graph::Vertex(root.id() + "+");
  auto r_minus = graph::Vertex(root.id() + "-");

  new_gadag.addVertex(r_plus);
  new_gadag.addVertex(r_minus);

  for (auto vertex : gadag.vertices()) {
    if (vertex != root) {
      new_gadag.addVertex(vertex);
    }
  }

  for (auto edge : gadag.edges()) {
    if (edge.from() == root) {
      new_gadag.addEdge({r_minus, edge.to()});
    } else if (edge.to() == root) {
      new_gadag.addEdge({edge.from(), r_plus});
    } else {
      new_gadag.addEdge(edge);
    }
  }

  return {{r_minus, r_plus}, new_gadag};
}

class NodeTreeSelector {
public:
  NodeTreeSelector(
    const graph::Graph& gadag,
    graph::Vertex vertex,
    graph::Vertex root,
    std::unordered_map<graph::Vertex, graph::Vertex> local_root)
    : local_root_(local_root)
    , vertex_(vertex)
    , root_(root) {
    initial_graph_ = gadag;

    auto split_gadag = splitGadag(gadag, local_root_.at(vertex));
    graph_ = split_gadag.second;

    dot::DotSerializer serializer("split_gadag");
    serializer.addGraph(graph_, "split_gadag", "black");
    serializer.serialize(std::cout);

    r_minus_ = split_gadag.first.first;
    r_plus_ = split_gadag.first.second;
  }

  void visit() {
    directBfs();
    inverseBfs();

    direction_up_[local_root_.at(vertex_)] = direction_up_.at(r_plus_);
    direction_down_[local_root_.at(vertex_)] = direction_down_.at(r_minus_);

    for (auto vertex : initial_graph_.vertices()) {
      if (vertex == vertex_) {
        continue;
      }

      if (local_root_.at(vertex) == local_root_.at(vertex_)) {
        if (v_plus_.count(vertex)) {
          direction_down_[vertex] = direction_down_.at(local_root_.at(vertex_));
        } else if (v_minus_.count(vertex)) {
          direction_up_[vertex] = direction_up_.at(local_root_.at(vertex_));
        } else {
          direction_up_[vertex] = direction_up_.at(local_root_.at(vertex_));
          direction_down_[vertex] = direction_down_.at(local_root_.at(vertex_));
        }
      }
    }

    direction_up_[root_] = direction_up_.at(local_root_.at(vertex_));
    direction_down_[root_] = direction_down_.at(local_root_.at(vertex_));

    for (auto vertex : initial_graph_.vertices()) {
      if (vertex != root_ && vertex != vertex_) {
        setEdge(vertex);
      }
    }
  }

  graph::Vertex directionUp(graph::Vertex to) {
    auto vertex = direction_up_.at(to);
    if (vertex == r_plus_) {
      return local_root_.at(vertex_);
    }

    ASSERT_FATAL(vertex != r_minus_);

    return vertex;
  }

  graph::Vertex directionDown(graph::Vertex to) {
    auto vertex = direction_down_.at(to);
    if (vertex == r_minus_) {
      return local_root_.at(vertex_);
    }

    ASSERT_FATAL(vertex != r_plus_);

    return vertex;
  }


private:
  void setEdge(graph::Vertex vertex) {
    ASSERT_FATAL(direction_up_.count(vertex) == direction_down_.count(vertex));

    if (!direction_up_.count(vertex)) {
      setEdge(local_root_.at(vertex));

      direction_up_[vertex] = direction_up_.at(local_root_.at(vertex));
      direction_down_[vertex] = direction_down_.at(local_root_.at(vertex));
    }
  }

  void directBfs() {
    std::queue<graph::Vertex> q;
    q.push(vertex_);

    std::unordered_set<graph::Vertex> visited;
    std::unordered_map<graph::Vertex, graph::Vertex> first_edge;

    while (!q.empty()) {
      graph::Vertex current = q.front();
      q.pop();


      for (auto connection : graph_.connections(current)) {
        if (connection != r_plus_ &&
          local_root_.at(connection) != local_root_.at(vertex_) &&
          local_root_.at(connection) != vertex_) {
          continue;
        }

        if (!visited.count(connection)) {
          visited.insert(connection);
          q.push(connection);

          v_plus_.insert(connection);

          if (current == vertex_) {
            first_edge[connection] = connection;
          } else {
            first_edge[connection] = first_edge.at(current);
          }

          direction_up_[connection] = first_edge.at(connection);
        }
      }
    }
  }

  void inverseBfs() {
    std::queue<graph::Vertex> q;
    q.push(vertex_);

    std::unordered_set<graph::Vertex> visited;
    std::unordered_map<graph::Vertex, graph::Vertex> first_edge;

    while (!q.empty()) {
      graph::Vertex current = q.front();
      q.pop();

      for (auto connection : graph_.inversedConnections(current)) {
        if (connection != r_minus_ &&
          local_root_[connection] != local_root_[vertex_] &&
          local_root_[connection] != vertex_) {
          continue;
        }

        if (!visited.count(connection)) {
          visited.insert(connection);
          q.push(connection);

          v_minus_.insert(connection);

          if (current == vertex_) {
            first_edge[connection] = connection;
          } else {
            first_edge[connection] = first_edge.at(current);
          }

          direction_down_[connection] = first_edge.at(connection);
        }
      }
    }
  }

private:
  graph::Vertex root_;
  graph::Vertex vertex_;
  std::unordered_map<graph::Vertex, graph::Vertex> local_root_;

  graph::Graph initial_graph_;
  graph::Graph graph_;

  graph::Vertex r_plus_;
  graph::Vertex r_minus_;

  std::unordered_set<graph::Vertex> v_plus_;
  std::unordered_set<graph::Vertex> v_minus_;

  std::unordered_map<graph::Vertex, graph::Vertex> direction_up_;
  std::unordered_map<graph::Vertex, graph::Vertex> direction_down_;
};

} // namespace

std::pair<graph::Graph, graph::Graph> makeTwoRedundantTrees(
  const graph::Graph& gadag,
  std::unordered_map<graph::Vertex, graph::Vertex> local_root,
  graph::Vertex root
) {
  graph::Graph tree_up;
  graph::Graph tree_down;

  for (auto vertex : gadag.vertices()) {
    if (vertex == root) {
      continue;
    }

    NodeTreeSelector selector(
      gadag,
      vertex,
      root,
      local_root
    );


    selector.visit();

    tree_up.addVertex(vertex);
    tree_up.addEdge({vertex, selector.directionUp(root)});

    tree_down.addVertex(vertex);
    tree_down.addEdge({vertex, selector.directionDown(root)});
  }

  return {
    tree_up,
    tree_down
  };
}

} // namespace algos

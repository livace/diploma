#include "dfs_gadag_redundant_trees.h"

#include "build_gadag_tree.h"
#include "dfs_tree_selector.h"
#include "adag_selection.h"

#include "base_algo/is_rooted_tree.h"
#include "base_algo/contains.h"
#include "base_algo/intersect.h"
#include "base_algo/subtract.h"

#include "dot/dot_serializer.h"

#include "util/assert.h"

#include <queue>

namespace algos {

util::Expected<std::tuple<graph::Graph, graph::Graph, graph::Graph>, std::string> makeDfsGadagThreeRedundantTrees(
  const graph::Graph& graph,
  graph::Vertex root
) {
  DfsTreeSelector visitor(graph);
  visitor.visit(root);
  auto dfs_tree = visitor.get();

  EXPECT(dfs_tree.vertices().size() == graph.vertices().size(),
    "Failed to get dfs tree, tree size " <<
    dfs_tree.vertices().size() <<
    ", graph size " <<
    graph.vertices().size());

  auto remaining_graph = base_algo::subtract(graph, dfs_tree);

  dot::DotSerializer serializer("after_first_dfs");
  serializer.addGraph(dfs_tree, "dfs_tree", "red");
  serializer.addGraph(remaining_graph, "remaining_graph", "blue");
  serializer.serialize(std::cout);

  auto selector = algos::AdagSelector(remaining_graph, root);

  {
    auto res = selector.build();
    PROPAGATE_ERROR(res);
  }

  auto gadag = selector.get();
  ASSERT_FATAL(base_algo::intersect(gadag, dfs_tree).edges().empty());

  for (auto edge : gadag.edges()) {
    EXPECT(gadag.has({edge.to(), edge.from()}), "Adag has undirected edge that will go to both trees");
  }

  auto primary_tree = buildGadagTree(gadag, root, /* inverse */ false);
  ASSERT_FATAL(base_algo::isRootedTree(primary_tree, root));
  auto secondary_tree = buildGadagTree(gadag, root, /* inverse */ true);
  ASSERT_FATAL(base_algo::isRootedTree(secondary_tree, root));

  ASSERT_FATAL(base_algo::intersect(gadag, primary_tree) == primary_tree);

  return {{
    dfs_tree,
    primary_tree,
    secondary_tree,
  }};
}

} // namespace algos

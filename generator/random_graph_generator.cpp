#include "random_graph_generator.h"

#include "base_algo/join.h"

#include "util/assert.h"
#include <boost/lexical_cast.hpp>

namespace generator {

RandomGraphGenerator::RandomGraphGenerator(
  int connectivity,
  int vertices,
  int edges,
  uint64_t seed)
  : connectivity_(connectivity)
  , vertices_(vertices)
  , edges_(edges)
  , rng_(seed)
{
  ASSERT_FATAL(edges_ + 1 >= vertices_);
}

graph::Graph RandomGraphGenerator::generate() {
  graph::Graph graph;

  for (int i = 0; i < connectivity_; i++) {
    auto new_tree = generateTree(graph);

    graph = base_algo::join(graph, new_tree);
  }

  addEdges(graph, edges_);

  return graph;
}


graph::Graph RandomGraphGenerator::generateTree(const graph::Graph& previous_trees) {
  graph::Graph result;

  std::vector<graph::Vertex> unused_vertices;
  std::vector<graph::Vertex> used_vertices;
  for (int i = 0; i < vertices_; i++) {
    unused_vertices.push_back(graph::Vertex(boost::lexical_cast<std::string>(i)));
  }

  {
    std::uniform_int_distribution<std::mt19937_64::result_type> unused_vertex_dist(0, unused_vertices.size() - 1);
    auto unused_vertex_index = unused_vertex_dist(rng_);
    std::swap(unused_vertices[unused_vertex_index], unused_vertices.back());
  }

  used_vertices.push_back(unused_vertices.back());
  unused_vertices.pop_back();

  while (!unused_vertices.empty()) {
    std::uniform_int_distribution<std::mt19937_64::result_type> unused_vertex_dist(0, unused_vertices.size() - 1);
    auto unused_vertex_index = unused_vertex_dist(rng_);
    std::swap(unused_vertices[unused_vertex_index], unused_vertices.back());

    auto unused_vertex = unused_vertices.back();

    std::uniform_int_distribution<std::mt19937_64::result_type> used_vertex_dist(0, used_vertices.size() - 1);
    auto used_index = used_vertex_dist(rng_);
    auto used_vertex = used_vertices[used_index];

    if (previous_trees.has({unused_vertex, used_vertex})) {
      continue;
    }

    result.addEdge({unused_vertex, used_vertex});
    result.addEdge({used_vertex, unused_vertex});

    unused_vertices.pop_back();
    used_vertices.push_back(unused_vertex);
  }

  ASSERT_FATAL(result.vertices().size() == vertices_, result.vertices().size() << " != " << vertices_);
  ASSERT_FATAL(result.edges().size() == 2 * (vertices_ - 1), result.edges().size() << " != " <<  2 * (vertices_ - 1));

  return result;
}

void RandomGraphGenerator::addEdges(graph::Graph& graph, int edges) {
  std::uniform_int_distribution<std::mt19937_64::result_type> dist(0, graph.vertices().size() - 1);

  while (graph.edges().size() < 2 * edges) {
    auto first_index = dist(rng_);
    auto second_index = dist(rng_);

    if (first_index == second_index) {
      continue;
    }

    graph.addEdge({graph.vertices()[first_index], graph.vertices()[second_index]});
    graph.addEdge({graph.vertices()[second_index], graph.vertices()[first_index]});
  }
}

} // namespace generator

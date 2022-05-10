#include "full_graph_generator.h"

#include <boost/lexical_cast.hpp>

namespace generator {

FullGraphGenerator::FullGraphGenerator(size_t n) : n_(n) {}

graph::Graph FullGraphGenerator::generate() {
  std::vector<graph::Vertex> vertices;
  for (size_t i = 0; i < n_; i++) {
    vertices.emplace_back(boost::lexical_cast<std::string>(i));
  }

  std::unordered_map<graph::Vertex, std::unordered_set<graph::Vertex>> connectivity_list;

  for (size_t i = 0; i < n_; i++) {
    for (size_t j = 0; j < n_; j++) {
      if (i == j) {
        continue;
      }

      connectivity_list[vertices[i]].insert(vertices[j]);
    }
  }

  return graph::Graph(connectivity_list);
}

} // namespace generator

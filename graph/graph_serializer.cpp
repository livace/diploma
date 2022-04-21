#include "graph_serializer.h"

#include <fstream>

namespace graph {

void serialize(std::ostream& stream, const Graph& graph) {
  const auto& vertices = graph.vertices();
  stream << vertices.size() << "\n";

  for (const auto& vertex : vertices) {
    const auto& connections = graph.connections(vertex);
    stream << vertex << " " << connections.size() << "\n";

    for (const auto& connection : connections) {
      stream << connection << " ";
    }
    stream << "\n";
  }
}

void serialize(const std::filesystem::path& filename, const Graph& graph) {
  std::ofstream osteam(filename.string());

  return serialize(osteam, graph);
}

Graph deserialize(std::istream& stream) {
  std::unordered_map<graph::Vertex, std::unordered_set<graph::Vertex>> connectivity_list;

  size_t size;
  stream >> size;

  for (size_t i = 0; i < size; i++) {
    graph::Vertex v;
    stream >> v;

    size_t connections_size;
    stream >> connections_size;
    for (size_t j = 0; j < connections_size; j++) {
      graph::Vertex connection;
      stream >> connection;
      connectivity_list[v].insert(connection);
    }
  }

  return Graph(connectivity_list);
}

Graph deserialize(const std::filesystem::path& filename) {
  std::ifstream isteam(filename.string());

  return deserialize(isteam);
}

} // namespace graph

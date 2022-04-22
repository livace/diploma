#include "builder.h"

namespace graph {

void Builder::addVertex(Vertex vertex) {
  connectivity_list_[vertex];
}

void Builder::addEdge(Edge edge) {
  addVertex(edge.from());
  addVertex(edge.to());

  connectivity_list_[edge.from()].insert(edge.to());
}

Graph Builder::build() {
  return graph::Graph(connectivity_list_);
}

} // namespace graph

#pragma once

#include "graph/graph.h"

namespace base_algo {

graph::Graph without(const graph::Graph& graph, graph::Vertex vertex);

graph::Graph without(const graph::Graph& graph, graph::Edge edge);

graph::Graph without(const graph::Graph& graph, const graph::Graph& other_graph);

} // namespace base_algo

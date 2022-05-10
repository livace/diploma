#pragma once

#include "graph/graph.h"

namespace base_algo {

graph::Graph without(const graph::Graph& graph, graph::Vertex vertex);

graph::Graph without(const graph::Graph& graph, graph::Edge edge);

} // namespace base_algo

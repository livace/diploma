#pragma once

#include "graph/graph.h"
#include "graph/vertex.h"

namespace base_algo {

bool isAdag(const graph::Graph& graph, graph::Vertex vertex);
bool isDag(const graph::Graph& graph);

} // namespace base_algo

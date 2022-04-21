#pragma once

#include "graph/graph.h"

namespace base_algo {

std::unordered_map<graph::Vertex, bool> dfs(const graph::Graph& graph, graph::Vertex vertex);
std::unordered_map<graph::Vertex, bool> inverseDfs(const graph::Graph& graph, graph::Vertex vertex);

} // namespace base_algo

#include "is_dag.h"

#include "dfs.h"
#include "without.h"

namespace base_algo {

class DagListener : public DfsListener {
public:
  void onAfterVertex(graph::Vertex vertex) override {
    depth_[vertex] = current_depth_++;
  }

  bool check(const graph::Graph& graph) {
    for (auto edge : graph.edges()) {
      if (depth_[edge.from()] < depth_[edge.to()]) {
        return false;
      }
    }

    return true;
  }

private:
  int current_depth_ = 0;

  std::unordered_map<graph::Vertex, int> depth_;
};

bool isDag(const graph::Graph& graph) {
  auto listener = DagListener();

  dfsAll(graph, &listener);

  return listener.check(graph);
}

bool isAdag(const graph::Graph& graph, graph::Vertex root) {
  return isDag(without(graph, root));
}

} // namespace base_algo

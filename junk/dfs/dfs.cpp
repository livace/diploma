#include "base_algo/dfs.h"
#include "graph/graph.h"
#include "graph/graph_serializer.h"
#include "util/argparse.h"
#include "util/signal_handler.h"

#include <iostream>

struct CommandLineOptions {
  graph::Vertex vertex;
  std::filesystem::path path;
};

CommandLineOptions parseArgs(int argc, char** argv) {
  CommandLineOptions result;
  util::ArgumentParser("Full Graph Generator")(
    "vertex,v",
    boost::program_options::value<graph::Vertex>(&result.vertex)->required(),
    "start from vertex"
  )(
    "path,p",
    boost::program_options::value<std::filesystem::path>(&result.path)->required(),
    "graph path"
  ).parse(argc, argv);

  return result;
}

int main(int argc, char** argv) {
  util::initDefaultSignalHandler();

  auto args = parseArgs(argc, argv);
  auto graph = graph::deserialize(args.path);
  auto res = base_algo::dfs(graph, args.vertex);
  for (auto [vertex, visited] : res) {
    std::cout << vertex << ": " << std::boolalpha << visited << "\n";
  }
}

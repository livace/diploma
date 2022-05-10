#include "graph/graph_serializer.h"

struct CommandLineOptions {
  std::filesystem::path path;
};

CommandLineOptions parseArgs(int argc, char** argv) {
  CommandLineOptions result;
  util::ArgumentParser("Test two redundant tree selection")(
    "dest,d",
    boost::program_options::value<std::filesystem::path>(&result.dest)->required(),
    "graph path"
  ).parse(argc, argv);

  return result;
}

int main(int argc, char** argv) {
  auto args = parseArgs(argc, argv);

  auto graph = graph::deserialize(args.path);

  auto [tree1, tree2] = algos::cycleSelection();
}

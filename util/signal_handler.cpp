#include "signal_handler.h"

#include "util/stacktrace.h"
#include "util/assert.h"

#include <iostream>
#include <csignal>
#include <unistd.h>
#include <sstream>

namespace util {

void defaultSignalHandler(int signum) {
  std::stringstream message_gen;
  message_gen << "signal caught: " << signum << std::endl;
  if (signum != SIGINT) {
    message_gen << util::stacktrace() << std::endl;
  }
  std::string message = message_gen.str();
  std::cerr << "\n\n" << message;

  std::_Exit(-signum);
}

void setSignalHandler(int signum, SignalHandler handler) {
  struct sigaction action{};
  action.sa_handler = *handler.target<void (*)(int)>();
  ASSERT_FATAL(sigaction(signum, &action, NULL) == 0, "unable to set signal handler, errno: " << errno);
}

void initDefaultSignalHandler() {
  for (auto signal : kDefaultSignals) {
    setSignalHandler(signal, defaultSignalHandler);
  }
}

}  // namespace util

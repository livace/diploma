#pragma once
#include <functional>
#include <signal.h>

namespace util {

using SignalHandler = std::function<void(int)>;
constexpr std::array kDefaultSignals = {SIGINT, SIGTERM, SIGSEGV, SIGQUIT, SIGFPE, SIGABRT, SIGILL};

void defaultSignalHandler(int signum);
void setSignalHandler(int signum, SignalHandler handler);

void initDefaultSignalHandler();

}  // namespace util

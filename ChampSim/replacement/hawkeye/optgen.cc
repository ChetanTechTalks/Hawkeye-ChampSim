#include "optgen.h"

#include <stdexcept>

OPTgen::OPTgen(std::size_t num_sets, std::size_t associativity, std::size_t history_multiplier)
    : associativity_(associativity), history_length_(associativity * history_multiplier), histories_(num_sets)
{
  if (associativity_ == 0 || history_length_ == 0)
    throw std::invalid_argument("OPTgen requires nonzero associativity and history length");

  for (auto& history : histories_)
    history.occupancy.assign(history_length_, 0);
}

bool OPTgen::access(std::size_t set_idx, uint64_t address)
{
  if (set_idx >= histories_.size())
    throw std::out_of_range("OPTgen set index out of range");

  auto& history = histories_[set_idx];
  const auto now = history.timestamp;

  // This position is leaving the 8x-capacity history window, so any
  // intervals recorded there can no longer affect a future decision.
  history.occupancy[now % history_length_] = 0;

  bool opt_hit = false;
  const auto previous = history.last_access.find(address);
  if (previous != history.last_access.end() && now - previous->second < history_length_) {
    opt_hit = true;
    for (std::size_t time = previous->second; time < now; ++time) {
      if (history.occupancy[time % history_length_] >= associativity_) {
        opt_hit = false;
        break;
      }
    }

    if (opt_hit) {
      for (std::size_t time = previous->second; time < now; ++time)
        ++history.occupancy[time % history_length_];
    }
  }

  history.last_access[address] = now;
  ++history.timestamp;
  return opt_hit;
}

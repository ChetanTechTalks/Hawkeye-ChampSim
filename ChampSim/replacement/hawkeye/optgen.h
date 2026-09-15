#ifndef HAWKEYE_OPTGEN_H
#define HAWKEYE_OPTGEN_H

#include <cstddef>
#include <cstdint>
#include <unordered_map>
#include <vector>

class OPTgen {
public:
  OPTgen(std::size_t num_sets, std::size_t associativity, std::size_t history_multiplier = 8);

  bool access(std::size_t set_idx, uint64_t address);

private:
  struct SetHistory {
    std::vector<std::size_t> occupancy;
    std::unordered_map<uint64_t, std::size_t> last_access;
    std::size_t timestamp = 0;
  };

  std::size_t associativity_;
  std::size_t history_length_;
  std::vector<SetHistory> histories_;
};

#endif

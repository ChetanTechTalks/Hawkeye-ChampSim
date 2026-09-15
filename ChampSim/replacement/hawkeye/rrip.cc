#include "rrip.h"

#include <stdexcept>

namespace {
constexpr int MAX_RRPV = 7;
}

void update_rrpv(std::vector<int>& rrpv, std::size_t way, Classification cls, bool is_hit)
{
  if (way >= rrpv.size())
    throw std::out_of_range("RRIP way index out of range");

  if (is_hit || cls == Classification::CACHE_FRIENDLY)
    rrpv[way] = 0;
  else
    rrpv[way] = MAX_RRPV;
}

std::size_t find_victim(std::vector<int>& rrpv)
{
  if (rrpv.empty())
    throw std::invalid_argument("RRIP cannot select a victim from an empty set");

  while (true) {
    for (std::size_t way = 0; way < rrpv.size(); ++way) {
      if (rrpv[way] >= MAX_RRPV)
        return way;
    }

    for (auto& value : rrpv) {
      if (value < MAX_RRPV)
        ++value;
    }
  }
}

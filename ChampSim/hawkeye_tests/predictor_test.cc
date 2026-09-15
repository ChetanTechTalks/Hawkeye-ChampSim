#include "../replacement/hawkeye/predictor.h"
#include <iostream>
#include <vector>
#include <utility>
#include <cstdint>

int main()
{
  HawkeyePredictor pred;
 
  std::vector<std::pair<uint64_t, bool>> train_events = {
  };
 
  std::vector<uint64_t> query_pcs = {
  };


  for (auto& [pc, opt_hit] : train_events) {
    pred.train(pc, opt_hit);
  }
  for (uint64_t pc : query_pcs) {
    std::cout << std::hex << pc << std::dec
              << ": counter=" << pred.get_counter(pc)
              << " predict=" << pred.predict(pc) << "\n";
  }
}

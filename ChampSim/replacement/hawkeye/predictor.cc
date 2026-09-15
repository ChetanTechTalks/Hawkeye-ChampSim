#include "predictor.h"

#include <stdexcept>

HawkeyePredictor::HawkeyePredictor(std::size_t num_entries, int counter_bits)
    : counters_(num_entries, 0), counter_bits_(counter_bits), maximum_counter_(0), friendly_threshold_(0)
{
  if (num_entries == 0)
    throw std::invalid_argument("HawkeyePredictor requires at least one entry");
  if (counter_bits_ <= 0 || counter_bits_ >= static_cast<int>(sizeof(int) * 8))
    throw std::invalid_argument("HawkeyePredictor counter width is invalid");

  maximum_counter_ = (1 << counter_bits_) - 1;
  friendly_threshold_ = 1 << (counter_bits_ - 1);
}

void HawkeyePredictor::train(uint64_t pc, bool opt_hit)
{
  auto& counter = counters_[index(pc)];
  if (opt_hit) {
    if (counter < maximum_counter_)
      ++counter;
  } else if (counter > 0) {
    --counter;
  }
}

bool HawkeyePredictor::predict(uint64_t pc) const
{
  return get_counter(pc) >= friendly_threshold_;
}

int HawkeyePredictor::get_counter(uint64_t pc) const
{
  return counters_[index(pc)];
}

std::size_t HawkeyePredictor::index(uint64_t pc) const
{
  return static_cast<std::size_t>(pc % counters_.size());
}

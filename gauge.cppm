export module gauge;
import pog;
import ranged;

namespace gauge {
export struct compos {
  pog::sparse_set<ranged> gauges{};
  pog::dense_set<float> drainers{};
};
} // namespace gauge

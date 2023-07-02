export module collision;
import pog;

namespace collision {
export using compo = pog::quadtree;
export struct compos {
  virtual compo &bodies() noexcept = 0;
};
} // namespace collision

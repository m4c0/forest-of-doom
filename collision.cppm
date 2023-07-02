export module collision;
import pog;

namespace collision {
struct compos {
  virtual pog::quadtree bodies() const = 0;
};
} // namespace collision

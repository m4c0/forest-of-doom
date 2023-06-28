export module anime;
import pog;

namespace anime {
export struct c {
  unsigned start_x;
  unsigned y;
  unsigned num_frames;
  unsigned ticks{};
};

export using compo = pog::sparse_set<c>;
} // namespace anime

export module chunk;
import pog;

namespace chunk {
export using compo = pog::sparse_set<unsigned>;

export unsigned &current() {
  static unsigned i{};
  return i;
}
} // namespace chunk

export module chunk;
import pog;

namespace chunk {
export using c = unsigned;
export using compo = pog::sparse_set<c>;

export c &current() {
  static c i{};
  return i;
}
} // namespace chunk

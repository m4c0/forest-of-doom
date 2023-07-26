export module sprite;
import pog;
export import rect;

export namespace sprite {
enum class layers { terrain, camping, scout, last };

struct c {
  rect pos;
  rect uv;
  layers layer;
};
using compo = pog::sparse_set<c>;

struct compos {
  pog::sparse_set<c> sprites{};
};
} // namespace sprite

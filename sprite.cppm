export module sprite;
import pog;
export import rect;

export namespace sprite {
struct c {
  rect pos;
  rect uv;
};
using compo = pog::sparse_set<c>;
} // namespace sprite

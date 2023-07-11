export module sprite;
import pog;
export import rect;

export struct sprite {
  using compo = pog::sparse_set<sprite>;

  rect pos;
  rect uv;
  unsigned layer;
};

export module sprite;
import pog;
export import rect;

export struct sprite {
  using compo = pog::sparse_set<sprite>;

  rect pos;
  rect uv;
  unsigned layer;
};
export void sort_sprites(sprite::compo &set) {
  set.sort([](sprite &a, sprite &b) -> int {
    auto l = a.layer - b.layer;
    if (l != 0)
      return l;
    return a.pos.y - b.pos.y;
  });
}

export module sprite;
import pog;
export import rect;

export struct sprite {
  static constexpr const auto max_sprites = 1024;
  using compo = pog::sparse_set<sprite>;

  rect pos;
  rect uv;
};

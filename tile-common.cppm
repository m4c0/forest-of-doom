export module tile:common;
export import area;
export import collision;
export import pog;
export import rect;
export import sprite;

namespace tile {
using c_t = unsigned;

export constexpr rect uv(c_t t) {
  return rect{
      .x = static_cast<float>((t >> 24) & 0xFFU),
      .y = static_cast<float>((t >> 16) & 0xFFU),
      .w = static_cast<float>((t >> 8) & 0xFFU),
      .h = static_cast<float>((t >> 0) & 0xFFU),
  };
}
template <typename C>
class compos : public virtual area::compos, public virtual collision::compos {
  pog::sparse_set<C> m_tiles{};
  pog::sparse_set<sprite> m_sprites{};

public:
  using tile_t = C;

  auto &tiles() noexcept { return m_tiles; }
  auto &sprites() noexcept { return m_sprites; }
};

template <typename C> auto add_tile(compos<C> *ec, C t, float x, float y) {
  rect r = uv(t);
  r.x = x;
  r.y = y;

  auto id = ec->e().alloc();
  area::add(ec, id, r);
  ec->tiles().add(id, t);
  return id;
}

template <typename C>
void update_tile_pos(compos<C> *ec, pog::eid id, float x, float y) {
  auto t = ec->tiles().get(id);
  rect r = uv(t);
  r.x = x;
  r.y = y;

  // TODO: update collisor?
  area::remove(ec, id);
  area::add(ec, id, r);
}

template <typename C> void remove_tile(compos<C> *ec, pog::eid id) {
  collision::remove(ec, id);
  area::remove(ec, id);
  ec->tiles().remove(id);
  ec->e().dealloc(id);
}

template <typename C> void populate(compos<C> *ec, float cx, float cy) {
  constexpr const auto radius = 16;
  area::c a{cx - radius, cy - radius, cx + radius, cy + radius};

  ec->sprites().remove_if([](auto, auto) { return true; });
  ec->areas().for_each_in(a, [&](pog::eid id, auto area) {
    auto t = ec->tiles().get(id);
    if (t == 0)
      return;

    sprite s{
        .pos = rect_of(area),
        .uv = uv(t),
    };

    ec->sprites().add(id, s);
  });
}
} // namespace tile

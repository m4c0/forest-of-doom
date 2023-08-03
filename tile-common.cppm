export module tile:common;
export import area;
export import collision;
export import pog;
export import rect;
export import sprite;

export namespace tile {
using c_t = unsigned;

constexpr rect uv(c_t t) {
  return rect{
      .x = static_cast<float>((t >> 24) & 0xFFU),
      .y = static_cast<float>((t >> 16) & 0xFFU),
      .w = static_cast<float>((t >> 8) & 0xFFU),
      .h = static_cast<float>((t >> 0) & 0xFFU),
  };
}
struct compos : virtual collision::compos, virtual sprite::compos {
  pog::sparse_set<c_t> tiles{};
};

auto add_tile(compos *ec, c_t t, rect uv, sprite::layers l, float x, float y) {
  sprite::c s{
      .uv = uv,
      .layer = l,
  };
  rect r{
      .x = x,
      .y = y,
      .w = s.uv.w,
      .h = s.uv.h,
  };

  auto id = sprite::add(ec, s, r);
  ec->tiles.add(id, t);
  return id;
}

void remove_tile(compos *ec, pog::eid id) {
  ec->tiles.remove(id);
  collision::remove(ec, id);
  sprite::remove(ec, id);
}
} // namespace tile

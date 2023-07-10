export module tiles;
import collision;
import pog;
import rect;
import sprite;
import tile;

namespace tiles {
export struct compos : tile::camping::compos {
  virtual pog::entity_list &e() noexcept = 0;
  virtual sprite::compo &sprites() noexcept = 0;
};

export constexpr rect tile_uv(tile::camping::c t) {
  auto ut = static_cast<unsigned>(t);
  return rect{
      .x = static_cast<float>((t >> 24) & 0xFFU),
      .y = static_cast<float>((t >> 16) & 0xFFU),
      .w = static_cast<float>((t >> 8) & 0xFFU),
      .h = static_cast<float>((t >> 0) & 0xFFU),
  };
}

export pog::eid add_tile(compos *ec, tile::camping::c t, float x, float y) {
  auto uv = tile_uv(t);
  sprite spr{
      .pos = {x, y, uv.w, uv.h},
      .uv = uv,
  };

  auto id = ec->e().alloc();
  tile::camping::add_tile(ec, id, t, x, y);
  ec->sprites().add(id, spr);
  return id;
}
export void update_tile(pog::eid id, compos *ec, tile::camping::c t) {
  auto spr = ec->sprites().get(id);
  spr.uv = tile_uv(t);
  spr.pos.w = spr.uv.w;
  spr.pos.h = spr.uv.h;
  ec->sprites().update(id, spr);
}
export void remove_tile(pog::eid id, compos *ec) {
  tile::camping::remove_tile(ec, id);
  ec->sprites().remove(id);
  ec->e().dealloc(id);
}
} // namespace tiles

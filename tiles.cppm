export module tiles;
import pog;
import rect;
import sprite;
import tile;

namespace tiles {
export constexpr rect tile_uv(tile t) {
  auto ut = static_cast<unsigned>(t);
  return rect{
      .x = static_cast<float>((t >> 24) & 0xFFU),
      .y = static_cast<float>((t >> 16) & 0xFFU),
      .w = static_cast<float>((t >> 8) & 0xFFU),
      .h = static_cast<float>((t >> 0) & 0xFFU),
  };
}

export struct builder {
  pog::entity_list *e;
  sprite::compo *sprites;
};
export pog::eid add_tile(tiles::builder b, tile t, float x, float y) {
  auto uv = tile_uv(t);
  sprite spr{
      .pos = {x, y, uv.w, uv.h},
      .uv = uv,
  };

  auto id = b.e->alloc();
  b.sprites->add(id, spr);
  return id;
}
export void update_tile(pog::eid id, sprite::compo *sprites, tile t) {
  auto spr = sprites->get(id);
  spr.uv = tile_uv(t);
  spr.pos.w = spr.uv.w;
  spr.pos.h = spr.uv.h;
  sprites->update(id, spr);
}
export void remove_tile(pog::eid id, tiles::builder b) {
  b.sprites->remove(id);
  b.e->dealloc(id);
}
} // namespace tiles

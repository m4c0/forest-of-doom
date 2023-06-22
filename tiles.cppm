export module tiles;
import pog;
import rect;
import sprite;
import tile;

namespace tiles {
constexpr rect tile_uv(tile t) {
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
export void add_tile(tiles::builder b, tile t, float x, float y) {
  auto uv = tile_uv(t);
  sprite spr{
      .pos = {x, y, uv.w, uv.h},
      .uv = uv,
  };

  auto id = b.e->alloc();
  b.sprites->add(id, spr);
}
} // namespace tiles

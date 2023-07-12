export module tile;
import area;
import collision;
import pog;
import rect;
import sprite;

namespace tile {
export constexpr rect uv(unsigned t) {
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

export namespace tile::camping {
enum c : unsigned {
  blank = 0,
  island_tl = 0x00010101,
  island_t = 0x01010101,
  island_tr = 0x03010101,
  island_l = 0x00020101,
  grass_0 = 0x01020101,
  grass_1 = 0x02020101,
  island_r = 0x03020101,
  island_bl = 0x00030102,
  island_b = 0x01030102,
  island_br = 0x03030102,
  lake_tl = 0x04010102,
  lake_tr = 0x07010102,
  lake_bl = 0x04040101,
  lake_br = 0x07040101,
  water = 0x05030101,
};
using compos = tile::compos<c>;

auto add_tile(compos *ec, c t, float x, float y) {
  auto id = tile::add_tile(ec, t, x, y);

  switch (t) {
  case water:
    collision::add(ec, id, x, y, 1, 1);
    break;
  case island_bl:
  case island_b:
  case island_br:
    collision::add(ec, id, x, y + 0.7, 1, 1.3);
    break;
  default:
    break;
  }

  return id;
}
void update_tile_pos(compos *ec, pog::eid id, float x, float y) {
  tile::update_tile_pos(ec, id, x, y);
}
void remove_tile(compos *ec, pog::eid id) { tile::remove_tile(ec, id); }
void populate(compos *ec, float cx, float cy) { tile::populate(ec, cx, cy); }
} // namespace tile::camping

export namespace tile::terrain {
enum c : unsigned {
  blank = 0,
  island_tl = 0x08060101,
  island_t = 0x09060101,
  island_tr = 0x0a060101,
  island_l = 0x08070101,
  grass_0 = 0x0b080101,
  grass_1 = 0x09080101,
  island_r = 0x0a080101,
  island_bl = 0x08090101,
  island_b = 0x09090101,
  island_br = 0x0a090101,
  lake_tl = 0x0b0a0101,
  lake_tr = 0x0c0a0101,
  lake_bl = 0x0b0a0101,
  lake_br = 0x0c0a0101,
  water = 0x0c080101,
};
using compos = tile::compos<c>;

auto add_tile(compos *ec, c t, float x, float y) {
  auto id = tile::add_tile(ec, t, x, y);

  switch (t) {
  case water:
    collision::add(ec, id, x, y, 1, 1);
    break;
  case island_bl:
  case island_b:
  case island_br:
    // collision::add(ec, id, x, y + 0.7, 1, 1.3);
    break;
  default:
    break;
  }

  return id;
}
void update_tile_pos(compos *ec, pog::eid id, float x, float y) {
  tile::update_tile_pos(ec, id, x, y);
}
void remove_tile(compos *ec, pog::eid id) { tile::remove_tile(ec, id); }
void populate(compos *ec, float cx, float cy) { tile::populate(ec, cx, cy); }
} // namespace tile::terrain

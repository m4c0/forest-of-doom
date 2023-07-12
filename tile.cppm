export module tile;
import area;
import collision;
import pog;
import rect;

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
using compo = pog::sparse_set<c>;

class compos : public area::compos, public virtual collision::compos {
  compo m_camping_tiles{};
  pog::sparse_set<unsigned> m_sprite_layer{};

public:
  virtual pog::entity_list &e() noexcept = 0;

  compo &camping_tiles() noexcept { return m_camping_tiles; }
  auto &sprite_layer() noexcept { return m_sprite_layer; }
};

constexpr rect uv(c t) {
  auto ut = static_cast<unsigned>(t);
  return rect{
      .x = static_cast<float>((t >> 24) & 0xFFU),
      .y = static_cast<float>((t >> 16) & 0xFFU),
      .w = static_cast<float>((t >> 8) & 0xFFU),
      .h = static_cast<float>((t >> 0) & 0xFFU),
  };
}

auto add_tile(compos *ec, c t, float x, float y) {
  rect r = uv(t);
  r.x = x;
  r.y = y;

  auto id = ec->e().alloc();
  area::add(ec, id, r);
  ec->camping_tiles().add(id, t);

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
  auto t = ec->camping_tiles().get(id);
  rect r = uv(t);
  r.x = x;
  r.y = y;

  // TODO: update collisor?
  area::remove(ec, id);
  area::add(ec, id, r);
}

void remove_tile(compos *ec, pog::eid id) {
  collision::remove(ec, id);
  area::remove(ec, id);
  ec->camping_tiles().remove(id);
  ec->e().dealloc(id);
}
} // namespace tile::camping

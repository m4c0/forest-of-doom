export module tile:terrain;
import :common;

export namespace tile::terrain {
enum c : c_t {
  blank = 0,
  grass_tl = 0x00060101,
  grass_t = 0x01060101,
  grass_tr = 0x02060101,
  grass_l = 0x00070101,
  grass_r = 0x02070101,
  grass_bl = 0x00080101,
  grass_b = 0x01080101,
  grass_br = 0x02080101,
  dirt_0 = 0x04080101,
  dirt_tl = 0x03060101,
  dirt_tr = 0x04060101,
  dirt_bl = 0x03070101,
  dirt_br = 0x04070101,
  island_tl = 0x08060101,
  island_t = 0x09060101,
  island_tr = 0x0a060101,
  island_l = 0x08070101,
  grass_0 = 0x0b080101,
  grass_1 = 0x09070101,
  island_r = 0x0a070101,
  island_bl = 0x08080101,
  island_b = 0x09080101,
  island_br = 0x0a080101,
  lake_tl = 0x0b060101,
  lake_tr = 0x0c060101,
  lake_bl = 0x0b070101,
  lake_br = 0x0c070101,
  water = 0x0c080101,
  sidewalk_tl = 0x00100101,
  sidewalk_t = 0x01100101,
  sidewalk_tr = 0x02100101,
  sidewalk_l = 0x00110101,
  sidewalk_r = 0x02110101,
  sidewalk_bl = 0x00120101,
  sidewalk_b = 0x01120101,
  sidewalk_br = 0x02120101,
  sidewalkr_tl = 0x03100101,
  sidewalkr_tr = 0x04100101,
  sidewalkr_bl = 0x03110101,
  sidewalkr_br = 0x04110101,
  cobble_0 = 0x19090101,
  cobble_1 = 0x1a090101,
  cobble_2 = 0x190a0101,
  cobble_3 = 0x1a0a0101,
  brick_0 = 0x1b090101,
  brick_1 = 0x1c090101,
  brick_2 = 0x1b0a0101,
  brick_3 = 0x1c0a0101,
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
    collision::add(ec, id, x, y + 0.7, 1, 0.3);
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
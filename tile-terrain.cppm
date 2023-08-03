export module tile:terrain;
import :common;
import sprite;

export namespace tile::terrain {
enum c {
  blank,
  grass_tl,
  grass_t,
  grass_tr,
  grass_l,
  grass_r,
  grass_bl,
  grass_b,
  grass_br,
  dirt_0,
  dirt_tl,
  dirt_tr,
  dirt_bl,
  dirt_br,
  island_tl,
  island_t,
  island_tr,
  island_l,
  grass_0,
  grass_1,
  island_r,
  island_bl,
  island_b,
  island_br,
  lake_tl,
  lake_tr,
  lake_bl,
  lake_br,
  water,
  sidewalk_tl,
  sidewalk_t,
  sidewalk_tr,
  sidewalk_l,
  sidewalk_r,
  sidewalk_bl,
  sidewalk_b,
  sidewalk_br,
  sidewalkr_tl,
  sidewalkr_tr,
  sidewalkr_bl,
  sidewalkr_br,
  cobble_0,
  cobble_1,
  cobble_2,
  cobble_3,
  brick_0,
  brick_1,
  brick_2,
  brick_3,
  last
};
constexpr unsigned tile_id(c cc) {
  switch (cc) {
  case blank:
    return 0;
  case grass_tl:
    return 0x00060101;
  case grass_t:
    return 0x01060101;
  case grass_tr:
    return 0x02060101;
  case grass_l:
    return 0x00070101;
  case grass_r:
    return 0x02070101;
  case grass_bl:
    return 0x00080101;
  case grass_b:
    return 0x01080101;
  case grass_br:
    return 0x02080101;
  case dirt_0:
    return 0x04080101;
  case dirt_tl:
    return 0x03060101;
  case dirt_tr:
    return 0x04060101;
  case dirt_bl:
    return 0x03070101;
  case dirt_br:
    return 0x04070101;
  case island_tl:
    return 0x08060101;
  case island_t:
    return 0x09060101;
  case island_tr:
    return 0x0a060101;
  case island_l:
    return 0x08070101;
  case grass_0:
    return 0x0b080101;
  case grass_1:
    return 0x09070101;
  case island_r:
    return 0x0a070101;
  case island_bl:
    return 0x08080101;
  case island_b:
    return 0x09080101;
  case island_br:
    return 0x0a080101;
  case lake_tl:
    return 0x0b060101;
  case lake_tr:
    return 0x0c060101;
  case lake_bl:
    return 0x0b070101;
  case lake_br:
    return 0x0c070101;
  case water:
    return 0x0c080101;
  case sidewalk_tl:
    return 0x00100101;
  case sidewalk_t:
    return 0x01100101;
  case sidewalk_tr:
    return 0x02100101;
  case sidewalk_l:
    return 0x00110101;
  case sidewalk_r:
    return 0x02110101;
  case sidewalk_bl:
    return 0x00120101;
  case sidewalk_b:
    return 0x01120101;
  case sidewalk_br:
    return 0x02120101;
  case sidewalkr_tl:
    return 0x03100101;
  case sidewalkr_tr:
    return 0x04100101;
  case sidewalkr_bl:
    return 0x03110101;
  case sidewalkr_br:
    return 0x04110101;
  case cobble_0:
    return 0x19090101;
  case cobble_1:
    return 0x1a090101;
  case cobble_2:
    return 0x190a0101;
  case cobble_3:
    return 0x1a0a0101;
  case brick_0:
    return 0x1b090101;
  case brick_1:
    return 0x1c090101;
  case brick_2:
    return 0x1b0a0101;
  case brick_3:
    return 0x1c0a0101;
  case last:
    return 0;
  }
}

struct compos : tile::compos {};

auto add_tile(compos *ec, c t, float x, float y) {
  auto tuv = uv(tile_id(t));
  auto id = tile::add_tile(ec, t, tuv, sprite::layers::terrain, x, y);

  switch (t) {
  case water:
    collision::add(ec, id, x, y, 1, 1);
    break;
  case island_bl:
  case island_b:
  case island_br:
    collision::add(ec, id, x, y + 0.5, 1, 0.5);
    break;
  default:
    break;
  }

  return id;
}
} // namespace tile::terrain

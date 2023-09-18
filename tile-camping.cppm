#pragma leco add_resource "11_Camping_16x16.png"
export module tile:camping;
import :common;
import sprite;

export namespace tile::camping {
enum c : c_t {
  blank = 0,
  backpack_a = 0x10220102,
  backpack_b = 0x11220102,
  backpack_c = 0x12220102,
};

auto add_tile(compos *ec, c t, float x, float y) {
  auto tuv = uv(t);
  auto id = tile::add_tile(ec, t, tuv, sprite::layers::camping, x, y);

  switch (t) {
  case backpack_a:
  case backpack_b:
  case backpack_c:
    collision::add(ec, id, x, y + 0.7, 1, 0.4);
    break;
  default:
    break;
  }

  return id;
}
} // namespace tile::camping

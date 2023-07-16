export module tile:camping;
import :common;

export namespace tile::camping {
enum c : c_t {
  blank = 0,
};
using compos = tile::compos<c>;

auto add_tile(compos *ec, c t, float x, float y) {
  auto id = tile::add_tile(ec, t, x, y);

  switch (t) {
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

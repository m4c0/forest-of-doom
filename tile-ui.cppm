export module tile:ui;
import :common;

export namespace tile::ui {
enum c : c_t {
  blank = 0,
  border_tl = 0x00000101,
  border_t = 0x01000101,
  border_tr = 0x02000101,
  border_l = 0x00010101,
  border_bg = 0x01010101,
  border_r = 0x02010101,
  border_bl = 0x00020101,
  border_b = 0x01020101,
  border_br = 0x02020101,
};
using compos = tile::compos<c>;

auto add_tile(compos *ec, c t, float x, float y) {
  return tile::add_tile(ec, t, x, y);
}
void update_tile_pos(compos *ec, pog::eid id, float x, float y) {
  tile::update_tile_pos(ec, id, x, y);
}
void remove_tile(compos *ec, pog::eid id) { tile::remove_tile(ec, id); }
void populate(compos *ec, float cx, float cy) { tile::populate(ec, cx, cy); }
} // namespace tile::ui

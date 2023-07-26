export module tile:ui;
import :common;
import sprite;

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
  time_l = 0x080a0101,
  mind_l = 0x090a0101,
  energy_l = 0x0a0a0101,
  heart_l = 0x0b0a0101,
  food_l = 0x0c0a0101,
  bat_l = 0x10070101,
  bat_r = 0x12070101,
  bat_empty = 0x10080101,
  bat_g_1 = 0x12080101,
  bat_g_2 = 0x10090101,
  bat_g_3 = 0x12090101,
  bat_g_4 = 0x11070101,
  bat_y_1 = 0x120b0101,
  bat_y_2 = 0x100c0101,
  bat_y_3 = 0x120c0101,
  bat_y_4 = 0x110a0101,
  bat_r_1 = 0x120e0101,
  bat_r_2 = 0x100f0101,
  bat_r_3 = 0x120f0101,
  bat_r_4 = 0x110d0101,
  bat_b_1 = 0x12110101,
  bat_b_2 = 0x10120101,
  bat_b_3 = 0x12120101,
  bat_b_4 = 0x11100101,
  bat_0_1 = 0x12140101,
  bat_0_2 = 0x10150101,
  bat_0_3 = 0x12150101,
  bat_0_4 = 0x11130101,
};
struct compos : tile::compos {};

auto add_tile(compos *ec, c t, float x, float y) {
  return tile::add_tile(ec, t, sprite::layers::ui, x, y);
}
} // namespace tile::ui

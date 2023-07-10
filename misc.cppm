export module misc;
import player;
import qsu;
import sprite;
import tile;

namespace misc {
export void follow_player(qsu::main *q, player::compos *ec,
                          tile::camping::compos *ec1) {
  auto area = player::get_area(ec);
  auto x = area.x + area.w / 2;
  auto y = area.y + area.h / 2;

  q->center_at(x, y);

  sprite::compo spr{};
  tile::camping::populate(ec1, &spr, x, y);
  q->fill_sprites(spr);
}
} // namespace misc

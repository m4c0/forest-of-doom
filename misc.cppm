export module misc;
import player;
import qsu;
import tile;

namespace misc {
export auto follow_player(qsu::main *q, player::compos *ec) {
  auto area = player::get_area(ec);
  auto x = area.x + area.w / 2;
  auto y = area.y + area.h / 2;
  q->center_at(x, y);
  return q->center();
}
} // namespace misc

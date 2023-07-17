export module movement;
import collision;
import pog;
import sprite;

namespace movement {
export struct c {
  float sx;
  float sy;
};
export using compo = pog::sparse_set<c>;
export class compos : public virtual collision::compos {
  compo m_movements{};

public:
  compo &movements() noexcept { return m_movements; }
};
export void update_sprites(compos *c, sprite::compo &sprites, int millis) {
  float ms = millis;
  for (auto [id, m] : c->movements()) {
    auto dx = m.sx * ms;
    auto dy = m.sy * ms;

    if (c->bodies().has(id)) {
      if (collision::move_by(c, id, dx, dy)) {
      } else if (collision::move_by(c, id, 0, dy)) {
        dx = 0;
      } else if (collision::move_by(c, id, dx, 0)) {
        dy = 0;
      } else {
        // TODO: do something
        continue;
      }
    }

    auto spr = sprites.get(id);
    spr.pos.x += dx;
    spr.pos.y += dy;
    sprites.update(id, spr);
  }
}
} // namespace movement

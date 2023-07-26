export module movement;
import collision;
import pog;
import sprite;
import stopwatch;

namespace movement {
export struct c {
  float sx;
  float sy;
};
export using compo = pog::sparse_set<c>;
export class compos : public virtual collision::compos,
                      public virtual sprite::compos,
                      public virtual stopwatch {
  compo m_movements{};

public:
  compo &movements() noexcept { return m_movements; }
};
export void update_sprites(compos *c) {
  float ms = c->current_millis();
  for (auto [id, m] : c->movements()) {
    auto dx = m.sx * ms;
    auto dy = m.sy * ms;

    if (c->bodies.has(id)) {
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

    sprite::move_by(c, id, dx, dy);
  }
}
} // namespace movement

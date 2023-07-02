export module movement;
import collision;
import move;
import sprite;

namespace movement {
export struct compos : collision::compos {
  virtual move::compo &movements() noexcept = 0;
};
export void update_sprites(compos *c, sprite::compo &sprites, int millis) {
  float ms = millis;
  for (auto [m, id] : c->movements()) {
    auto dx = m.sx * ms;
    auto dy = m.sy * ms;

    auto spr = sprites.get(id);
    spr.pos.x += dx;
    spr.pos.y += dy;
    sprites.update(id, spr);

    if (c->bodies().has(id)) {
      collision::move_by(c, id, dx, dy);
    }
  }
}
} // namespace movement

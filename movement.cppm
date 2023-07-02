export module movement;
import collision;
import move;
import sprite;

namespace movement {
export struct compos : collision::compo {
  virtual move::compo &movements() noexcept = 0;
};
export void update_sprites(compos &c, sprite::compo &sprites, int millis) {
  float ms = millis;
  for (auto [m, id] : c.movements()) {
    auto spr = sprites.get(id);
    spr.pos.x += m.sx * ms;
    spr.pos.y += m.sy * ms;
    sprites.update(id, spr);
    // if (bodies.has()) bodies.update()
  }
}
} // namespace movement

export module movement;
import move;
import sprite;

namespace movement {
export void update_sprites(move::compo &moves, sprite::compo &sprites,
                           int millis) {
  float ms = millis;
  for (auto [m, id] : moves) {
    auto spr = sprites.get(id);
    spr.pos.x += m.sx * ms;
    spr.pos.y += m.sy * ms;
    sprites.update(id, spr);
  }
}
} // namespace movement

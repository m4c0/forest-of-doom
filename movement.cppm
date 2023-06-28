export module movement;
import move;
import sprite;

namespace movement {
export void update_sprites(move::compo &moves, sprite::compo &sprites) {
  for (auto [m, id] : moves) {
    auto spr = sprites.get(id);
    spr.pos.x += m.sx;
    spr.pos.y += m.sy;
    sprites.update(id, spr);
  }
}
} // namespace movement

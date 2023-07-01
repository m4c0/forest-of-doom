export module animation;
import anime;
import pog;
import sprite;

namespace animation {
export void update_animes(anime::compo &set, sprite::compo &sprites,
                          int millis) {
  constexpr const auto ticks_per_frame = 100000;

  for (auto &[a, id] : set) {
    a.ticks += millis;

    auto spr = sprites.get(id);
    spr.uv.x = a.start_x + ((a.ticks / ticks_per_frame) % a.num_frames);
    spr.uv.y = a.y;
    sprites.update(id, spr);
  }
}
} // namespace animation

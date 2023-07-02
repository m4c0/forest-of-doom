export module animation;
import anime;
import pog;
import sprite;

namespace animation {
export void update_animes(anime::compo &set, sprite::compo &sprites,
                          int millis) {
  constexpr const auto frames_per_sec = 24;

  for (auto &[a, id] : set) {
    a.ticks += millis;

    auto frame = frames_per_sec * a.ticks / 1000;

    auto spr = sprites.get(id);
    spr.uv.x = a.start_x + (frame % a.num_frames);
    spr.uv.y = a.y;
    sprites.update(id, spr);
  }
}
} // namespace animation

export module animation;
import pog;
import sprite;

namespace animation {
export struct c {
  unsigned start_x;
  unsigned y;
  unsigned num_frames;
  unsigned frames_per_sec;
  unsigned ticks{};
};
export using compo = pog::sparse_set<c>;

export void update_animes(compo &set, sprite::compo &sprites, int millis) {
  for (auto &[a, id] : set) {
    a.ticks += millis;

    auto frame = a.frames_per_sec * a.ticks / 1000;

    auto spr = sprites.get(id);
    spr.uv.x = a.start_x + (frame % a.num_frames);
    spr.uv.y = a.y;
    sprites.update(id, spr);
  }
}
} // namespace animation

export module animation;
import pog;
import sprite;
import stopwatch;

namespace animation {
export struct c {
  unsigned start_x;
  unsigned y;
  unsigned num_frames;
  unsigned frames_per_sec;
  unsigned ticks{};
};
export using compo = pog::sparse_set<c>;
export class compos : public virtual stopwatch {
  animation::compo m_animations{};

public:
  animation::compo &animations() noexcept { return m_animations; }
};

export void update_animes(compos *ec, sprite::compo &sprites) {
  for (auto &[id, a] : ec->animations()) {
    a.ticks += ec->current_millis();

    auto frame = a.frames_per_sec * a.ticks / 1000;

    auto spr = sprites.get(id);
    spr.uv.x = a.start_x + (frame % a.num_frames);
    spr.uv.y = a.y;
    sprites.update(id, spr);
  }
}
} // namespace animation

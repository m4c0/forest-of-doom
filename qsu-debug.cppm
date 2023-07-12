export module qsu:debug;
import collision;
import pog;
import quack;

namespace qsu {
class debug_layer {
  quack::ilayout m_spr;

public:
  debug_layer(quack::renderer *m_r, unsigned max_sprites)
      : m_spr{m_r, max_sprites} {}

  void fill(collision::compo &set) {
    // TODO: center this based on current player pos
    constexpr const pog::aabb area{{-100, -100}, {100, 100}};

    auto size = 0;
    set.for_each_in(area, [&](auto, auto) { size++; });
    m_spr->set_count(size);

    m_spr->map_colours([&](auto *cs) {
      for (auto i = 0; i < size; i++) {
        cs[i] = {1, 1, 1, 0.5};
      }
    });
    m_spr->map_multipliers([&](auto *ms) {
      for (auto i = 0; i < size; i++) {
        ms[i] = {};
      }
    });
    m_spr->map_positions([&](auto *ps) {
      set.for_each_in(area, [&](auto, auto aabb) {
        *ps++ = quack::rect{aabb.a.x, aabb.a.y, aabb.b.x - aabb.a.x,
                            aabb.b.y - aabb.a.y};
      });
    });
    m_spr->map_uvs([&](auto *uvs) {
      for (auto i = 0; i < size; i++) {
        uvs[i] = {};
      }
    });
  }

  [[nodiscard]] constexpr auto &operator*() noexcept { return m_spr; }
  [[nodiscard]] constexpr auto &operator*() const noexcept { return m_spr; }
};
} // namespace qsu

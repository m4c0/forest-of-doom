export module qsu:debug;
import casein;
import collision;
import pog;
import silog;
import quack;

namespace qsu {
class debug_layer {
  quack::ilayout m_spr;
  unsigned m_max_sprites{};

public:
  debug_layer(quack::renderer *m_r, unsigned max_sprites)
      : m_spr{m_r, max_sprites} {}

  void process_event(const casein::event &e) {
    m_spr.process_event(e);

    if (e.type() == casein::QUIT)
      silog::log(silog::info, "[qsu] %d colliders debugged", m_max_sprites);
  }

  void fill(collision::compos *ec) {
    // TODO: center this based on current player pos
    constexpr const pog::aabb area{{-100, -100}, {100, 100}};

    m_spr->map_all([&](auto all) {
      auto cs = all.colours;
      auto ms = all.multipliers;
      auto ps = all.positions;
      auto us = all.uvs;

      auto size = 0;
      ec->bodies.for_each_in(area, [&](auto id, auto aabb) {
        if (ec->collisions.has(id)) {
          *cs++ = {1, 0, 0, 0.5};
        } else {
          *cs++ = {1, 1, 1, 0.5};
        }

        *ps++ = quack::rect{aabb.a.x, aabb.a.y, aabb.b.x - aabb.a.x,
                            aabb.b.y - aabb.a.y};

        *ms++ = {};
        *us++ = {};
        size++;
      });

      m_spr->set_count(size);
      if (size > m_max_sprites)
        m_max_sprites = size;
    });
  }

  [[nodiscard]] constexpr auto &operator*() noexcept { return m_spr; }
  [[nodiscard]] constexpr auto &operator*() const noexcept { return m_spr; }
};
} // namespace qsu

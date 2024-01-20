export module qsu:layer;
import casein;
import jute;
import pog;
import quack;
import silog;
import sprite;
import stubby;
import traits;

namespace qsu {
class layer {
  static constexpr const auto sprite_sz = 16.0f;

  quack::instance_batch m_spr;
  sprite::layers m_layer;
  jute::view m_atlas_name;
  float m_atlas_w{1};
  float m_atlas_h{1};
  unsigned m_max_sprites{};

public:
  layer(quack::instance_batch &&ib, sprite::layers l, jute::view atlas)
      : m_spr{traits::move(ib)}, m_layer{l}, m_atlas_name{atlas} {
    if (atlas == "") {
      m_spr.load_atlas(16, 16, [](auto) {});
    } else {
      m_spr.load_atlas(atlas);
    }
  }
  ~layer() {
    silog::log(silog::info, "[qsu] %d sprites for %s", m_max_sprites,
               m_atlas_name.cstr().data());
  }

  void fill(sprite::compos *ec) {
    auto [px, py] = m_spr.center();
    pog::aabb area{{px - 32, py - 32}, {px + 32, py + 32}};

    m_spr.map_all([&](auto all) {
      auto cs = all.colours;
      auto ms = all.multipliers;
      auto ps = all.positions;
      auto us = all.uvs;

      auto size = 0;
      ec->areas().for_each_in(area, [&](auto id, auto aabb) {
        auto spr = ec->sprites.get(id);
        if (spr.layer != m_layer)
          return;

        *cs++ = {spr.colour.r, spr.colour.g, spr.colour.b, spr.colour.a};
        *ms++ = {1, 1, 1, 1.0f - spr.dim};

        *ps++ = quack::rect{aabb.a.x, aabb.a.y, aabb.b.x - aabb.a.x,
                            aabb.b.y - aabb.a.y};

        *us++ = quack::uv{
            spr.uv.x * sprite_sz / m_atlas_w,
            spr.uv.y * sprite_sz / m_atlas_h,
            (spr.uv.x + spr.uv.w) * sprite_sz / m_atlas_w,
            (spr.uv.y + spr.uv.h) * sprite_sz / m_atlas_h,
        };

        size++;
      });

      m_spr.set_count(size);
      if (size > m_max_sprites)
        m_max_sprites = size;
    });
  }

  [[nodiscard]] constexpr auto *operator->() noexcept { return &m_spr; }
  [[nodiscard]] constexpr auto *operator->() const noexcept { return &m_spr; }
  [[nodiscard]] constexpr auto &operator*() noexcept { return m_spr; }
  [[nodiscard]] constexpr auto &operator*() const noexcept { return m_spr; }
};
} // namespace qsu

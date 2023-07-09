export module qsu;
import casein;
import collision;
import jute;
import pog;
import quack;
import silog;
import sprite;
import stubby;

namespace qsu {
class layer {
  static constexpr const auto sprite_sz = 16.0f;

  quack::ilayout m_spr;
  float m_atlas_w;
  float m_atlas_h;

public:
  layer(quack::renderer *m_r, unsigned max_sprites) : m_spr{m_r, max_sprites} {}

  void setup(jute::view atlas) {
    stbi::load_from_resource(atlas)
        .map([this](const auto &img) {
          m_atlas_w = img.width;
          m_atlas_h = img.height;
          m_spr->load_atlas(img.width, img.height, [&img](auto *p) {
            const auto pixies = img.width * img.height;
            const auto data = reinterpret_cast<const decltype(p)>(*img.data);
            for (auto i = 0; i < pixies; i++) {
              p[i] = data[i];
            }
          });
          silog::log(silog::info, "%dx%d atlas loaded", img.width, img.height);
        })
        .take([](auto err) {
          silog::log(silog::error, "Error loading atlas: %s", err);
        });
  }

  void fill(const sprite::compo &set) {
    m_spr->map_colours([&](auto *cs) {
      for (auto _ : set) {
        *cs++ = {};
      }
    });
    m_spr->map_multipliers([&](auto *ms) {
      for (auto [spr, _] : set) {
        *ms++ = {1, 1, 1, 1};
      }
    });
    m_spr->map_positions([&](auto *ps) {
      for (auto [spr, _] : set) {
        *ps++ = quack::rect{spr.pos.x, spr.pos.y, spr.pos.w, spr.pos.h};
      }
    });
    m_spr->map_uvs([&](auto *uvs) {
      for (auto [spr, _] : set) {
        *uvs++ = quack::uv{
            spr.uv.x * sprite_sz / m_atlas_w,
            spr.uv.y * sprite_sz / m_atlas_h,
            (spr.uv.x + spr.uv.w) * sprite_sz / m_atlas_w,
            (spr.uv.y + spr.uv.h) * sprite_sz / m_atlas_h,
        };
      }
    });
    m_spr->set_count(set.size());
  }

  [[nodiscard]] constexpr auto &operator*() noexcept { return m_spr; }
  [[nodiscard]] constexpr auto &operator*() const noexcept { return m_spr; }
};

class debug_layer {
  quack::ilayout m_spr;

public:
  debug_layer(quack::renderer *m_r, unsigned max_sprites)
      : m_spr{m_r, max_sprites} {}

  void fill(pog::rtree &set) {
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
}; // namespace qsu

export class main {
  static constexpr const auto max_player_sprites = 16;
  static constexpr const auto max_sprites = 1024;
  static constexpr const auto no_sprite = max_sprites + 1;

  quack::renderer m_r{3};
  layer m_spr{&m_r, max_sprites};
  layer m_player{&m_r, max_player_sprites};
  debug_layer m_debug{&m_r, max_sprites};
  quack::mouse_tracker m_mouse{};

public:
  void process_event(const casein::event &e) {
    m_r.process_event(e);
    (*m_spr).process_event(e);
    (*m_player).process_event(e);
    (*m_debug).process_event(e);
    m_mouse.process_event(e);

    if (e.type() == casein::CREATE_WINDOW) {
      m_spr.setup("11_Camping_16x16.png");
      m_player.setup("Modern_Exteriors_Characters_Scout_16x16_1.png");
    }
  }

  void center_at(float x, float y) {
    (*m_spr)->center_at(x, y);
    (*m_player)->center_at(x, y);
    (*m_debug)->center_at(x, y);
  }
  void set_grid(float w, float h) {
    (*m_spr)->set_grid(w, h);
    (*m_player)->set_grid(w, h);
    (*m_debug)->set_grid(w, h);
  }

  [[nodiscard]] auto mouse_pos() const noexcept {
    return m_mouse.current_mouse_pos(&**m_spr);
  }

  void fill_debug(pog::rtree &set) { m_debug.fill(set); }
  void fill_sprites(const sprite::compo &set) { m_spr.fill(set); }
  void fill_player_sprites(const sprite::compo &set) { m_player.fill(set); }
};
} // namespace qsu

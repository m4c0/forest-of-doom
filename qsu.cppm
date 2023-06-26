export module qsu;
import casein;
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

  void setup() {
    stbi::load_from_resource("11_Camping_16x16.png")
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
export class main {
  static constexpr const auto max_sprites = 1024;
  static constexpr const auto no_sprite = max_sprites + 1;

  quack::renderer m_r{2};
  layer m_spr{&m_r, max_sprites};
  quack::mouse_tracker m_mouse{};

  void setup() { m_spr.setup(); }

public:
  void process_event(const casein::event &e) {
    m_r.process_event(e);
    (*m_spr).process_event(e);
    m_mouse.process_event(e);

    if (e.type() == casein::CREATE_WINDOW)
      setup();
  }

  void center_at(float x, float y) { (*m_spr)->center_at(x, y); }
  void set_grid(float w, float h) { (*m_spr)->set_grid(w, h); }

  [[nodiscard]] auto mouse_pos() const noexcept {
    return m_mouse.current_mouse_pos(&**m_spr);
  }

  void fill_sprites(const sprite::compo &set) { m_spr.fill(set); }
};
} // namespace qsu

export module qsu;
import casein;
import pog;
import quack;
import silog;
import sprite;
import stubby;

namespace qsu {
export class main {
  static constexpr const auto sprite_sz = 16.0f;
  static constexpr const auto max_sprites = 1024;

  quack::renderer m_r{1};
  quack::ilayout m_spr{&m_r, max_sprites};
  float m_atlas_w;
  float m_atlas_h;

  void setup() {
    m_spr.set_grid(4, 4);
    stbi::load_from_resource("11_Camping_16x16.png")
        .map([this](const auto &img) {
          m_atlas_w = img.width;
          m_atlas_h = img.height;
          m_r.load_atlas(img.width, img.height, [&img](auto *p) {
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

public:
  void process_event(const casein::event &e) {
    m_r.process_event(e);
    m_spr.process_event(e);

    if (e.type() == casein::CREATE_WINDOW)
      setup();
  }

  template <unsigned N>
  void fill_sprites(const pog::sparse_set<sprite, N> &set) {
    m_spr.batch()->colours().map([&](auto *cs) {
      for (auto _ : set) {
        *cs++ = {};
      }
    });
    m_spr.batch()->multipliers().map([&](auto *ms) {
      for (auto [spr, _] : set) {
        *ms++ = {1, 1, 1, 1};
      }
    });
    m_spr.batch()->positions().map([&](auto *ps) {
      for (auto [spr, _] : set) {
        *ps++ = quack::rect{spr.pos.x, spr.pos.y, spr.pos.w, spr.pos.h};
      }
    });
    m_spr.batch()->uvs().map([&](auto *uvs) {
      for (auto [spr, _] : set) {
        *uvs++ = quack::uv{
            spr.uv.x * sprite_sz / m_atlas_w,
            spr.uv.y * sprite_sz / m_atlas_h,
            (spr.uv.x + spr.uv.w) * sprite_sz / m_atlas_w,
            (spr.uv.y + spr.uv.h) * sprite_sz / m_atlas_h,
        };
      }
    });
    m_spr.batch()->set_count(set.size());
  }
};
} // namespace qsu

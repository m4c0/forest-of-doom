export module qsu:layer;
import casein;
import jute;
import quack;
import silog;
import sprite;
import stubby;

namespace qsu {
class layer {
  static constexpr const auto sprite_sz = 16.0f;

  quack::ilayout m_spr;
  jute::view m_atlas_name;
  float m_atlas_w;
  float m_atlas_h;
  unsigned m_max_sprites{};

  void create_window() {
    stbi::load_from_resource(m_atlas_name)
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
  void quit() {
    silog::log(silog::info, "[qsu] %d sprites for %s", m_max_sprites,
               m_atlas_name.cstr().data());
  }

public:
  layer(quack::renderer *m_r, unsigned max_sprites, jute::view atlas)
      : m_spr{m_r, max_sprites}, m_atlas_name{atlas} {}

  void process_event(const casein::event &e) {
    m_spr.process_event(e);

    switch (e.type()) {
    case casein::CREATE_WINDOW:
      create_window();
      break;
    case casein::QUIT:
      quit();
      break;
    default:
      break;
    }
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
    if (set.size() > m_max_sprites) {
      m_max_sprites = set.size();
    }
  }

  [[nodiscard]] constexpr auto &operator*() noexcept { return m_spr; }
  [[nodiscard]] constexpr auto &operator*() const noexcept { return m_spr; }
};
} // namespace qsu

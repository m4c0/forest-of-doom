export module qsu;
import :debug;
import :layer;
import area;
import casein;
import pog;
import quack;
import sprite;
import tile;

namespace qsu {
export class main {
  static constexpr const auto max_player_sprites = 16;
  static constexpr const auto max_sprites = 4096;
  static constexpr const auto no_sprite = max_sprites + 1;

  quack::renderer m_r{3};
  layer m_spr{&m_r, max_sprites, "11_Camping_16x16.png"};
  layer m_player{&m_r, max_player_sprites,
                 "Modern_Exteriors_Characters_Scout_16x16_1.png"};
  debug_layer m_debug{&m_r, max_sprites};
  quack::mouse_tracker m_mouse{};

public:
  void process_event(const casein::event &e) {
    m_r.process_event(e);
    m_spr.process_event(e);
    m_player.process_event(e);
    (*m_debug).process_event(e);
    m_mouse.process_event(e);
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
  void fill_player_sprites(const sprite::compo &set) { m_player.fill(set); }

  void fill_sprites(tile::camping::compos *ec) {
    auto [cx, cy] = (*m_spr)->center();

    constexpr const auto radius = 16;
    area::c a{cx - radius, cy - radius, cx + radius, cy + radius};

    sprite::compo spr{};
    ec->areas().for_each_in(a, [&](pog::eid id, auto area) {
      auto t = ec->camping_tiles().get(id);
      if (t == tile::camping::blank)
        return;

      sprite s{
          .pos = rect_of(area),
          .uv = uv(t),
          .layer = ec->sprite_layer().get(id),
      };
      spr.add(id, s);
    });
    m_spr.fill(spr);
  }
};
} // namespace qsu

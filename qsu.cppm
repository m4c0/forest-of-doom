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
export enum class layers { terrain, camping, scout, last };

export class main {
  static constexpr const auto max_player_sprites = 16;
  static constexpr const auto max_sprites = 4096;
  static constexpr const auto no_sprite = max_sprites + 1;

  static constexpr const auto layer_count = static_cast<unsigned>(layers::last);
  static constexpr const auto extra_layer_count = 2;

  quack::renderer m_r{layer_count + extra_layer_count};
  layer m_layers[layer_count] = {
      {&m_r, max_sprites, "1_Terrains_and_Fences_16x16.png"},
      {&m_r, max_sprites, "11_Camping_16x16.png"},
      {&m_r, max_player_sprites,
       "Modern_Exteriors_Characters_Scout_16x16_1.png"},
  };
  layer m_hud{&m_r, max_sprites, "Modern_UI_Style_1.png"};
  debug_layer m_debug{&m_r, max_sprites};
  quack::mouse_tracker m_mouse{};

public:
  void process_event(const casein::event &e) {
    m_r.process_event(e);
    (*m_debug).process_event(e);
    m_hud.process_event(e);
    m_mouse.process_event(e);
    for (auto &l : m_layers) {
      l.process_event(e);
    }
  }

  void center_at(float x, float y) {
    for (auto &l : m_layers) {
      (*l)->center_at(x, y);
    }
    (*m_debug)->center_at(x, y);
  }
  [[nodiscard]] auto center() const noexcept {
    return (*m_layers[0])->center();
  }

  void set_grid(float w, float h) {
    for (auto &l : m_layers) {
      (*l)->set_grid(w, h);
    }
    (*m_debug)->set_grid(w, h);
    (*m_hud)->set_grid(16, 16);
  }

  [[nodiscard]] auto mouse_pos() const noexcept {
    return m_mouse.current_mouse_pos(&**m_layers[0]);
  }

  void fill_debug(pog::rtree &set) { m_debug.fill(set); }
  void fill_hud(const sprite::compo &set) { m_hud.fill(set); }
  void fill_sprites(layers l, const sprite::compo &set) {
    m_layers[static_cast<unsigned>(l)].fill(set);
  }
}; // namespace qsu
} // namespace qsu

export module qsu;
import :layer;
import area;
import casein;
import collision;
import pog;
import quack;
import sprite;
import tile;

namespace qsu {
export class main {
  static constexpr const auto max_player_sprites = 16;
  static constexpr const auto max_sprites = 4096;
  static constexpr const auto no_sprite = max_sprites + 1;

  static constexpr const auto layer_count =
      static_cast<unsigned>(sprite::layers::last);

  quack::renderer m_r{layer_count};
  layer m_layers[layer_count] = {
      {&m_r, sprite::layers::terrain, max_sprites,
       "1_Terrains_and_Fences_16x16.png"},
      {&m_r, sprite::layers::camping, max_sprites, "11_Camping_16x16.png"},
      {&m_r, sprite::layers::scout, max_player_sprites,
       "Modern_Exteriors_Characters_Scout_16x16_1.png"},
      {&m_r, sprite::layers::debug, max_sprites, {}},
      {&m_r, sprite::layers::ui, max_sprites, "Modern_UI_Style_1.png"},
  };
  quack::mouse_tracker m_mouse{};

  [[nodiscard]] auto &layer_of(sprite::layers l) {
    return m_layers[static_cast<unsigned>(l)];
  }
  [[nodiscard]] auto &layer_of(sprite::layers l) const {
    return m_layers[static_cast<unsigned>(l)];
  }

public:
  void process_event(const casein::event &e) {
    m_r.process_event(e);
    m_mouse.process_event(e);
    for (auto &l : m_layers) {
      l.process_event(e);
    }
  }

  void center_at(float x, float y) {
    for (auto &l : m_layers) {
      (*l)->center_at(x, y);
    }
    (*layer_of(sprite::layers::ui))->center_at(0, 0);
  }
  [[nodiscard]] auto center() const noexcept {
    return (*m_layers[0])->center();
  }

  void set_grid(float w, float h) {
    for (auto &l : m_layers) {
      (*l)->set_grid(w, h);
    }
    (*layer_of(sprite::layers::ui))->set_grid(16, 16);
  }
  [[nodiscard]] auto hud_grid_size() const noexcept {
    return (*layer_of(sprite::layers::ui))->grid_size();
  }

  [[nodiscard]] auto mouse_pos() const noexcept {
    return m_mouse.current_mouse_pos(&**m_layers[0]);
  }

  void fill(sprite::compos *ec) {
    for (auto &l : m_layers) {
      l.fill(ec);
    }
  }
}; // namespace qsu
} // namespace qsu

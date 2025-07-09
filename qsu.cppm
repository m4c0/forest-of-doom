#pragma leco add_resource_dir res

export module qsu;
import :layer;
import area;
import casein;
import collision;
import dotz;
import hai;
import pog;
import quack;
import sprite;
import tile;
import v;
import voo;

namespace qsu {
export class main {
  static constexpr const auto max_player_sprites = 16;
  static constexpr const auto max_sprites = 4096;

  static constexpr const auto layer_count =
      static_cast<unsigned>(sprite::layers::last);
  static constexpr const auto ui_layer_index =
      static_cast<unsigned>(sprite::layers::ui);

  quack::pipeline_stuff m_ps { *v::dq(), layer_count };

  layer m_layers[layer_count] {
    { m_ps, max_sprites,        sprite::layers::terrain, "1_Terrains_and_Fences_16x16.png" },
    { m_ps, max_sprites,        sprite::layers::camping, "11_Camping_16x16.png" },
    { m_ps, max_player_sprites, sprite::layers::scout,   "Modern_Exteriors_Characters_Scout_16x16_1.png" },
    { m_ps, max_sprites,        sprite::layers::ui,      "Modern_UI_Style_1.png" },
  };

  quack::upc m_ui_upc;
  quack::upc m_map_upc;

  dotz::vec2 m_grid_size;
  dotz::vec2 m_hud_grid_size;
  dotz::vec2 m_center;

  void for_each_layer(auto &&fn) {
    for (auto i = 0; i < layer_count; i++) {
      fn(m_layers[i]);
    }
  }
  void for_each_non_ui_layer(auto &&fn) {
    for (auto i = 0; i < ui_layer_index; i++) {
      fn(m_layers[i]);
    }
  }
  void for_each_ui_layer(auto &&fn) {
    for (auto i = ui_layer_index; i < layer_count; i++) {
      fn(m_layers[i]);
    }
  }

public:
  void fill(sprite::compos *ec) {
    for_each_non_ui_layer([ec, this](auto &l) { l.fill(ec, m_center); });
    for_each_ui_layer([ec](auto &l) { l.fill(ec, {}); });
  }

  void center_at(float x, float y) { m_center = {x, y}; }
  [[nodiscard]] auto center() noexcept { return m_center; }

  void set_grid(float w, float h) { m_grid_size = {w, h}; }

  [[nodiscard]] auto hud_grid_size() noexcept { return m_hud_grid_size; }

  [[nodiscard]] auto mouse_pos() noexcept {
    // TODO: fix mouse
    return dotz::vec2{};
  }

  void on_frame() {
    auto ui_upc = quack::adjust_aspect({
      .grid_pos { 8 * v::sw()->aspect(), -8.0f },
      .grid_size { 32, 32 },
    }, v::sw()->aspect());
    auto map_upc = quack::adjust_aspect({
      .grid_pos = m_center,
      .grid_size = m_grid_size,
    }, v::sw()->aspect());

    for_each_non_ui_layer([&](auto &l) {
      l.draw(m_ps, {
        .sw  = v::sw(),
        .scb = v::sw()->command_buffer(),
        .pc  = &map_upc,
      }); 
    });

    for_each_ui_layer([&](auto &l) {
      l.draw(m_ps, {
        .sw  = v::sw(),
        .scb = v::sw()->command_buffer(),
        .pc  = &ui_upc,
      }); 
    });
  }
};
}

#pragma leco add_resource "1_Terrains_and_Fences_16x16.png"
#pragma leco add_resource "11_Camping_16x16.png"
#pragma leco add_resource "Modern_Exteriors_Characters_Scout_16x16_1.png"
#pragma leco add_resource "Modern_UI_Style_1.png"

export module qsu;
import :layer;
import area;
import casein;
import collision;
import dotz;
import pog;
import quack;
import sprite;
import tile;
import voo;

namespace qsu {
export class main : voo::casein_thread {
  static constexpr const auto max_player_sprites = 16;
  static constexpr const auto max_sprites = 4096;

  static constexpr const auto layer_count =
      static_cast<unsigned>(sprite::layers::last);
  static constexpr const auto ui_layer_index =
      static_cast<unsigned>(sprite::layers::ui);

  layer *m_layers;
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

  void run() override {
    voo::device_and_queue dq{"fod", native_ptr()};

    while (!interrupted()) {
      voo::swapchain_and_stuff sw{dq};

      quack::pipeline_stuff ps{dq, sw, layer_count};
      layer layers[layer_count]{
          {dq, ps, max_sprites, sprite::layers::terrain,
           "1_Terrains_and_Fences_16x16.png"},
          {dq, ps, max_sprites, sprite::layers::camping,
           "11_Camping_16x16.png"},
          {dq, ps, max_player_sprites, sprite::layers::scout,
           "Modern_Exteriors_Characters_Scout_16x16_1.png"},
          {dq, ps, max_sprites, sprite::layers::debug, {}},
          {dq, ps, max_sprites, sprite::layers::ui, "Modern_UI_Style_1.png"},
      };
      m_layers = layers;

      release_init_lock();
      extent_loop(dq, sw, [&] {
        auto ui_upc = quack::adjust_aspect(
            {
                .grid_pos = {},
                .grid_size = {16, 16},
            },
            sw.aspect());
        auto map_upc = quack::adjust_aspect(
            {
                .grid_pos = m_center,
                .grid_size = m_grid_size,
            },
            sw.aspect());

        {
          voo::cmd_buf_one_time_submit pcb{sw.command_buffer()};
          auto scb = sw.cmd_render_pass(pcb);

          ps.cmd_push_vert_frag_constants(*scb, map_upc);
          for_each_non_ui_layer([&](auto &l) { l.run(ps, *scb); });

          ps.cmd_push_vert_frag_constants(*scb, ui_upc);
          for_each_ui_layer([&](auto &l) { l.run(ps, *scb); });
        }
        sw.queue_submit(dq);
      });

      m_layers = nullptr;
    }
  }

public:
  void fill(sprite::compos *ec) {
    wait_init();
    if (m_layers == nullptr)
      return;

    for_each_non_ui_layer([ec, this](auto &l) { l.fill(ec, m_center); });
    for_each_ui_layer([ec](auto &l) { l.fill(ec, {}); });
  }

  void center_at(float x, float y) { m_center = {x, y}; }
  [[nodiscard]] auto center() noexcept { return m_center; }

  void set_grid(float w, float h) { m_grid_size = {w, h}; }

  [[nodiscard]] auto hud_grid_size() noexcept { return m_hud_grid_size; }

  // TODO: fix mouse
  [[nodiscard]] auto mouse_pos() noexcept {
    wait_init();
    if (m_layers == nullptr)
      return dotz::vec2{};
    return dotz::vec2{};
  }

  void handle(const casein::event &e) {
    quack::mouse_tracker::instance().handle(e);
    casein_thread::handle(e);
  }
};
} // namespace qsu

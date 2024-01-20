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

  void for_each_layer(auto &&fn) {
    auto lck = wait_init();
    if (m_layers == nullptr)
      return;
    for (auto i = 0; i < layer_count; i++) {
      fn(m_layers[i]);
    }
  }
  void for_each_non_ui_layer(auto &&fn) {
    auto lck = wait_init();
    if (m_layers == nullptr)
      return;
    for (auto i = 0; i < ui_layer_index; i++) {
      fn(m_layers[i]);
    }
  }
  void for_each_ui_layer(auto &&fn) {
    auto lck = wait_init();
    if (m_layers == nullptr)
      return;
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
          {ps.create_batch(max_sprites), sprite::layers::terrain,
           "1_Terrains_and_Fences_16x16.png"},
          {ps.create_batch(max_sprites), sprite::layers::camping,
           "11_Camping_16x16.png"},
          {ps.create_batch(max_player_sprites), sprite::layers::scout,
           "Modern_Exteriors_Characters_Scout_16x16_1.png"},
          {ps.create_batch(max_sprites), sprite::layers::debug, {}},
          {ps.create_batch(max_sprites), sprite::layers::ui,
           "Modern_UI_Style_1.png"},
      };

      m_layers = layers;
      release_init_lock();

      for_each_ui_layer([](auto &l) {
        l->center_at(0, 0);
        l->set_grid(16, 16);
      });

      extent_loop(dq, sw, [&] {
        for (auto &l : layers) {
          (*l).submit_buffers(dq.queue());
        }

        sw.one_time_submit(dq, [&](auto &pcb) {
          auto scb = sw.cmd_render_pass(pcb);
          for (auto &l : layers) {
            ps.run(*scb, *l);
          }
        });
      });

      m_layers = nullptr;
    }
  }

public:
  void fill(sprite::compos *ec) {
    for_each_layer([ec](auto &l) { l.fill(ec); });
  }
  void set_grid(float w, float h) {
    for_each_non_ui_layer([w, h](auto &l) { l->set_grid(w, h); });
  }
  void center_at(float x, float y) {
    for_each_non_ui_layer([x, y](auto &l) { l->center_at(x, y); });
  }
  [[nodiscard]] auto center() noexcept {
    auto lck = wait_init();
    if (m_layers == nullptr)
      return dotz::vec2{};
    return m_layers[0]->center();
  }
  [[nodiscard]] auto mouse_pos() const noexcept { return dotz::vec2{}; }
  [[nodiscard]] auto hud_grid_size() noexcept {
    auto lck = wait_init();
    if (m_layers == nullptr)
      return dotz::vec2{1, 1};
    return m_layers[ui_layer_index]->grid_size();
  }

  void handle(const casein::event &e) {
    quack::mouse_tracker::instance().handle(e);
    casein_thread::handle(e);
  }
};
} // namespace qsu

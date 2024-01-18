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
export class main : public voo::casein_thread {
  static constexpr const auto max_player_sprites = 16;
  static constexpr const auto max_sprites = 4096;

  static constexpr const auto layer_count =
      static_cast<unsigned>(sprite::layers::last);

  layer *m_layers;

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

      /*
      ib.load_atlas(16, 32, atlas_image);
      ib.map_positions([](auto *ps) { ps[0] = {{0, 0}, {1, 1}}; });
      ib.map_colours([](auto *cs) { cs[0] = {0, 0, 0.1, 1.0}; });
      ib.map_uvs([](auto *us) { us[0] = {}; });
      ib.map_multipliers([](auto *ms) { ms[0] = {1, 1, 1, 1}; });
      ib.center_at(0.5, 0.5);
      ib.set_count(2);
      ib.set_grid(1, 1);
      */

      m_layers = layers;
      release_init_lock();
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
    }
  }

public:
  void fill(sprite::compos *ec) {}
  void set_grid(float w, float h) {}
  void center_at(float x, float y) {}
  [[nodiscard]] auto center() const noexcept { return dotz::vec2{}; }
  [[nodiscard]] auto mouse_pos() const noexcept { return dotz::vec2{}; }
  [[nodiscard]] auto hud_grid_size() const noexcept { return dotz::vec2{}; }

  /*
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
  */
};
} // namespace qsu

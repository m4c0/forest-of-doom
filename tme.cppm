export module main;

import casein;
import ecs;
import qsu;
import tilemap;

class game {
  static constexpr const auto step = 1.0f;

  ecs::ec m_ec{};
  qsu::main *m_q{};
  tilemap::map m_map{};

  float m_cx = tilemap::width / 2.0f;
  float m_cy = tilemap::height / 2.0f;

  void set_center() { m_q->center_at(m_cx, m_cy); }

public:
  void setup(qsu::main *q) {
    m_q = q;

    m_map.add_island(0, 0);
    m_map.add_island(4, 4);
    m_map.add_entities({&m_ec.e, &m_ec.sprites}, 0, 0);

    q->fill_sprites(m_ec.sprites);
    set_center();
  }

  void down() {
    m_cy += step;
    set_center();
  }
  void up() {
    m_cy -= step;
    set_center();
  }
  void left() {
    m_cx += step;
    set_center();
  }
  void right() {
    m_cx -= step;
    set_center();
  }
};

extern "C" void casein_handle(const casein::event &e) {
  static qsu::main q{};
  static game gg{};

  static constexpr const auto k_map = [] {
    casein::key_map res{};
    res[casein::K_DOWN] = [](auto) { gg.down(); };
    res[casein::K_LEFT] = [](auto) { gg.left(); };
    res[casein::K_RIGHT] = [](auto) { gg.right(); };
    res[casein::K_UP] = [](auto) { gg.up(); };
    return res;
  }();
  static constexpr const auto map = [] {
    casein::event_map res{};
    res[casein::CREATE_WINDOW] = [](auto) { gg.setup(&q); };
    res[casein::KEY_DOWN] = [](auto e) { k_map.handle(e); };
    return res;
  }();

  q.process_event(e);
  map.handle(e);
}

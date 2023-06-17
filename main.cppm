export module main;

import casein;
import ecs;
import qsu;
import sprite;

class game {
  ecs::ec m_ec{};

public:
  void setup(qsu::main *q) {
    for (auto y = 0.0f; y < 5; y++) {
      for (auto x = 0.0f; x < 4; x++) {
        sprite s{};
        s.pos = {x, y, 1, 1};
        s.uv = {x, y + 1, 1, 1};

        auto e = m_ec.e.alloc();
        m_ec.sprites.add(e, s);
      }
    }

    q->fill_sprites(m_ec.sprites);
  }
};

extern "C" void casein_handle(const casein::event &e) {
  static qsu::main q{};
  static game gg{};

  static constexpr const auto k_map = [] {
    casein::key_map res{};
    // res[casein::K_DOWN] = [](auto) { gg.down(); };
    // res[casein::K_LEFT] = [](auto) { gg.left(); };
    // res[casein::K_RIGHT] = [](auto) { gg.right(); };
    // res[casein::K_UP] = [](auto) { gg.up(); };
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

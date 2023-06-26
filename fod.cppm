export module fod;

import casein;
import ecs;
import prefabs;
import qsu;
import sprite;
import tile;
import tilemap;

class game {
  ecs::ec m_ec{};

public:
  void setup(qsu::main *q) {
    tilemap::map map = prefabs::island_0;
    map.add_entities({&m_ec.e, &m_ec.sprites}, &m_ec.chunks, 1, 0, 0);

    q->center_at(8, 8);
    q->set_grid(6, 6);
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

export module main;

import casein;
import ecs;
import qsu;
import sprite;
import tile;
import tilemap;

class game {
  ecs::ec m_ec{};

public:
  void setup(qsu::main *q) {
    tilemap map{};
    map.fill(water);
    map.set(0, 0, island_tl);
    map.set(1, 0, island_tr);
    map.set(0, 1, island_bl);
    map.set(1, 1, island_br);
    map.add_entities(&m_ec, 0, 0);

    q->center_at(2, 2);
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

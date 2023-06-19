export module main;

import casein;
import ecs;
import qsu;
import sprite;
import tile;
import tiles;

class game {
  ecs::ec m_ec{};

public:
  void setup(qsu::main *q) {
    tiles::add_tile(&m_ec, island_t, -2, -2);
    tiles::add_tile(&m_ec, island_tl, -3, -2);
    tiles::add_tile(&m_ec, grass_0, -2, -1);
    tiles::add_tile(&m_ec, island_l, -3, -1);
    tiles::add_tile(&m_ec, grass_1, -2, 0);
    tiles::add_tile(&m_ec, island_l, -3, 0);
    tiles::add_tile(&m_ec, island_b, -2, 1);
    tiles::add_tile(&m_ec, island_bl, -3, 1);

    tiles::add_tile(&m_ec, island_t, -1, -2);
    tiles::add_tile(&m_ec, island_t, 0, -2);
    tiles::add_tile(&m_ec, grass_0, -1, -1);
    tiles::add_tile(&m_ec, grass_1, 0, -1);
    tiles::add_tile(&m_ec, grass_1, -1, 0);
    tiles::add_tile(&m_ec, grass_0, 0, 0);
    tiles::add_tile(&m_ec, island_b, -1, 1);
    tiles::add_tile(&m_ec, island_b, 0, 1);

    tiles::add_tile(&m_ec, island_tr, 2, -2);
    tiles::add_tile(&m_ec, island_t, 1, -2);
    tiles::add_tile(&m_ec, island_r, 2, -1);
    tiles::add_tile(&m_ec, grass_1, 1, -1);
    tiles::add_tile(&m_ec, island_r, 2, 0);
    tiles::add_tile(&m_ec, grass_0, 1, 0);
    tiles::add_tile(&m_ec, island_br, 2, 1);
    tiles::add_tile(&m_ec, island_b, 1, 1);

    q->center_at(0, 0);
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

export module fod;

import casein;
import ecs;
import player;
import prefabs;
import qsu;
import sprite;
import tile;
import tilemap;

class game {
  qsu::main *m_q;
  ecs::ec m_ec{};
  unsigned m_arrows_down{};

public:
  explicit constexpr game(qsu::main *q) : m_q{q} {}

  void setup() {
    tilemap::map map = prefabs::island_0;
    map.add_entities(&m_ec, 1, 0, 0);

    player::add_entity(&m_ec);

    m_q->center_at(8.5, 9.25);
    m_q->set_grid(8, 8);
    m_q->fill_sprites(m_ec.sprites());
    m_q->fill_player_sprites(m_ec.player_sprites());
  }

  void tick() {
    player::update_animation(&m_ec);
    m_q->fill_player_sprites(m_ec.player_sprites());
  }

  void key_up(player::side s) {
    if (--m_arrows_down > 0)
      return;

    player::set_idle_animation(&m_ec, s);
    m_q->fill_player_sprites(m_ec.player_sprites());
  }
  void key_down(player::side s) {
    ++m_arrows_down;
    player::set_walk_animation(&m_ec, s);
    m_q->fill_player_sprites(m_ec.player_sprites());
  }
};

extern "C" void casein_handle(const casein::event &e) {
  static qsu::main q{};
  static game gg{&q};

  static constexpr const auto kd_map = [] {
    casein::key_map res{};
    res[casein::K_DOWN] = [](auto) { gg.key_down(player::p_down); };
    res[casein::K_LEFT] = [](auto) { gg.key_down(player::p_left); };
    res[casein::K_RIGHT] = [](auto) { gg.key_down(player::p_right); };
    res[casein::K_UP] = [](auto) { gg.key_down(player::p_up); };
    return res;
  }();
  static constexpr const auto ku_map = [] {
    casein::key_map res{};
    res[casein::K_DOWN] = [](auto) { gg.key_up(player::p_down); };
    res[casein::K_LEFT] = [](auto) { gg.key_up(player::p_left); };
    res[casein::K_RIGHT] = [](auto) { gg.key_up(player::p_right); };
    res[casein::K_UP] = [](auto) { gg.key_up(player::p_up); };
    return res;
  }();
  static constexpr const auto map = [] {
    casein::event_map res{};
    res[casein::CREATE_WINDOW] = [](auto) { gg.setup(); };
    res[casein::KEY_DOWN] = [](auto e) { kd_map.handle(e); };
    res[casein::KEY_UP] = [](auto e) { ku_map.handle(e); };
    res[casein::REPAINT] = [](auto) { gg.tick(); };
    return res;
  }();

  q.process_event(e);
  map.handle(e);
}

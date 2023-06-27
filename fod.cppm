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

  void update_player_sprite(player::side x) {
    player::set_side(&m_ec, x);
    m_q->fill_player_sprites(m_ec.player_sprites());
  }

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

  void right() { update_player_sprite(player::p_right); }
  void up() { update_player_sprite(player::p_up); }
  void left() { update_player_sprite(player::p_left); }
  void down() { update_player_sprite(player::p_down); }
};

extern "C" void casein_handle(const casein::event &e) {
  static qsu::main q{};
  static game gg{&q};

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
    res[casein::CREATE_WINDOW] = [](auto) { gg.setup(); };
    res[casein::KEY_DOWN] = [](auto e) { k_map.handle(e); };
    return res;
  }();

  q.process_event(e);
  map.handle(e);
}

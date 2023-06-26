export module fod;

import casein;
import ecs;
import prefabs;
import qsu;
import sprite;
import tile;
import tilemap;

class game {
  qsu::main *m_q;
  ecs::ec m_ec{};

  void update_player_sprite(float x) {
    auto pid = m_ec.player.get_id();
    auto spr = m_ec.player_sprites.get(pid);
    spr.uv.x = x;
    m_ec.player_sprites.update(pid, spr);
    m_q->fill_player_sprites(m_ec.player_sprites);
  }

public:
  void setup(qsu::main *q) {
    tilemap::map map = prefabs::island_0;
    map.add_entities({&m_ec.e, &m_ec.sprites}, &m_ec.chunks, 1, 0, 0);

    sprite spr{
        .pos = {8, 8, 1, 2},
        .uv = {0, 0, 1, 2},
    };
    auto pid = m_ec.e.alloc();
    m_ec.player.set(pid, {});
    m_ec.player_sprites.add(pid, spr);

    q->center_at(8.5, 9.25);
    q->set_grid(8, 8);
    q->fill_sprites(m_ec.sprites);
    q->fill_player_sprites(m_ec.player_sprites);

    m_q = q;
  }

  void right() { update_player_sprite(0); }
  void up() { update_player_sprite(1); }
  void left() { update_player_sprite(2); }
  void down() { update_player_sprite(3); }
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

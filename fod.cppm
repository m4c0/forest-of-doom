export module fod;

import animation;
import casein;
import ecs;
import input;
import player;
import prefabs;
import qsu;

class game {
  qsu::main m_q{};
  ecs::ec m_ec{};
  input::dual_axis m_input;

public:
  void setup() {
    prefabs::island_0.add_entities(&m_ec, 1, 0, 0);

    player::add_entity(&m_ec);

    m_q.center_at(8.5, 9.25);
    m_q.set_grid(8, 8);
    m_q.fill_sprites(m_ec.sprites());
    m_q.fill_player_sprites(m_ec.player_sprites());
  }

  void tick() {
    animation::update_animes(m_ec.animations(), m_ec.player_sprites());
    m_q.fill_player_sprites(m_ec.player_sprites());
  }

  void key_changed() {
    player::process_input(m_input, &m_ec);
    m_q.fill_player_sprites(m_ec.player_sprites());
  }

  void process_event(const casein::event &e) {
    m_q.process_event(e);
    m_input.process_event(e);

    switch (e.type()) {
    case casein::CREATE_WINDOW:
      setup();
      break;
    case casein::REPAINT:
      tick();
      break;
    case casein::KEY_DOWN:
    case casein::KEY_UP:
      key_changed();
      break;
    default:
      break;
    }
  }
};

extern "C" void casein_handle(const casein::event &e) {
  static game gg{};
  gg.process_event(e);
}

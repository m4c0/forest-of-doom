export module fod;

import animation;
import casein;
import ecs;
import input;
import misc;
import movement;
import player;
import prefabs;
import qsu;
import silog;
import sitime;
import sprite;
import tile;

class game {
  qsu::main m_q{};
  ecs::ec m_ec{};
  input::dual_axis m_input;
  sitime::stopwatch m_watch;

  void setup() {
    prefabs::island_0.add_entities(&m_ec, 1, 0, 0);
    prefabs::ocean_0.add_entities(&m_ec, 2, -16, 0);
    prefabs::ocean_0.add_entities(&m_ec, 3, 16, 0);
    prefabs::ocean_0.add_entities(&m_ec, 4, -16, -16);
    prefabs::ocean_0.add_entities(&m_ec, 5, 0, -16);
    prefabs::ocean_0.add_entities(&m_ec, 6, 16, -16);
    prefabs::ocean_0.add_entities(&m_ec, 7, -16, 16);
    prefabs::ocean_0.add_entities(&m_ec, 8, 0, 16);
    prefabs::ocean_0.add_entities(&m_ec, 9, 16, 16);

    player::add_entity(&m_ec);
    misc::follow_player(&m_q, &m_ec);

    m_q.set_grid(8, 8);
    m_q.fill_player_sprites(m_ec.player_sprites());
    fill_tiles();

    m_watch = {};
  }

  void tick() {
    animation::update_animes(m_ec.animations(), m_ec.player_sprites(),
                             m_watch.millis());
    movement::update_sprites(&m_ec, m_ec.player_sprites(), m_watch.millis());
    misc::follow_player(&m_q, &m_ec);
    m_q.fill_player_sprites(m_ec.player_sprites());
    fill_tiles();
    // m_q.fill_debug(m_ec.bodies());

    m_watch = {};
  }

  void key_changed() {
    player::process_input(m_input, &m_ec);
    m_q.fill_player_sprites(m_ec.player_sprites());
  }

  void dump_stats() {
    silog::log(silog::debug, "Max entities: %d", m_ec.e().max_elements());
    silog::log(silog::debug, "Animations: %d", m_ec.animations().size());
    silog::log(silog::debug, "Chunks: %d", m_ec.chunks().size());
    silog::log(silog::debug, "Movements: %d", m_ec.movements().size());
    silog::log(silog::debug, "P Sprites: %d", m_ec.player_sprites().size());
  }

  void fill_tiles() {
    sprite::compo spr{};
    auto area = player::get_area(&m_ec);
    auto x = area.x + area.w / 2;
    auto y = area.y + area.h / 2;
    tile::camping::populate(&m_ec, &spr, x, y);
    m_q.fill_sprites(spr);
  }

public:
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
    case casein::QUIT:
      dump_stats();
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

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
import tile;

class game {
  qsu::main m_q{};
  ecs::ec m_ec{};
  input::dual_axis m_input;
  sitime::stopwatch m_watch;

  void setup() {
    prefabs::island_0.add_entities(&m_ec, 0, 0);
    prefabs::ocean_0.add_entities(&m_ec, -16, 0);
    prefabs::ocean_0.add_entities(&m_ec, 16, 0);
    prefabs::ocean_0.add_entities(&m_ec, -16, -16);
    prefabs::ocean_0.add_entities(&m_ec, 0, -16);
    prefabs::ocean_0.add_entities(&m_ec, 16, -16);
    prefabs::ocean_0.add_entities(&m_ec, -16, 16);
    prefabs::ocean_0.add_entities(&m_ec, 0, 16);
    prefabs::ocean_0.add_entities(&m_ec, 16, 16);

    player::add_entity(&m_ec);

    m_q.set_grid(8, 8);
    repaint();

    m_watch = {};
  }

  void repaint() {
    auto [cx, cy] = misc::follow_player(&m_q, &m_ec);
    tile::camping::populate(&m_ec, cx, cy);
    tile::terrain::populate(&m_ec, cx, cy);
    m_q.fill_player_sprites(m_ec.player_sprites());
    m_q.fill_camping_sprites(
        static_cast<tile::camping::compos &>(m_ec).sprites());
    m_q.fill_terrain_sprites(
        static_cast<tile::terrain::compos &>(m_ec).sprites());
    // m_q.fill_debug(m_ec.bodies());
  }

  void tick() {
    animation::update_animes(m_ec.animations(), m_ec.player_sprites(),
                             m_watch.millis());
    movement::update_sprites(&m_ec, m_ec.player_sprites(), m_watch.millis());
    repaint();

    m_watch = {};
  }

  void key_changed() { player::process_input(m_input, &m_ec); }

  void dump_stats() {
    silog::log(silog::debug, "Max entities: %d", m_ec.e().max_elements());
    silog::log(silog::debug, "Animations: %d", m_ec.animations().size());
    silog::log(silog::debug, "Movements: %d", m_ec.movements().size());
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

export module fod;

import animation;
import casein;
import hud;
import gauge;
import input;
import looting;
import misc;
import movement;
import player;
import prefabs;
import qsu;
import silog;
import sitime;
import tile;
import tilemap;

struct ec : hud::compos, looting::compos, tile::terrain::compos {};

class game {
  qsu::main m_q{};
  ec m_ec{};

  void setup() {
    auto *ect = static_cast<tile::terrain::compos *>(&m_ec);
    prefabs::island_0(ect, 0, 0);
    prefabs::ocean_0(ect, -16, 0);
    prefabs::ocean_0(ect, 16, 0);
    prefabs::ocean_0(ect, -16, -16);
    prefabs::ocean_0(ect, 0, -16);
    prefabs::ocean_0(ect, 16, -16);
    prefabs::ocean_0(ect, -16, 16);
    prefabs::ocean_0(ect, 0, 16);
    prefabs::ocean_0(ect, 16, 16);
    looting::add_backpack(&m_ec, tile::camping::backpack_a, 9, 7);
    looting::add_backpack(&m_ec, tile::camping::backpack_b, 10, 7);
    looting::add_backpack(&m_ec, tile::camping::backpack_c, 11, 7);

    player::add_entity(&m_ec);
    hud::add_entities(&m_ec);

    m_q.set_grid(8, 8);
    repaint();
    m_ec.reset_watch();
  }

  void repaint() {
    auto [cx, cy] = misc::follow_player(&m_q, &m_ec);
    tile::camping::populate(&m_ec, cx, cy);
    tile::terrain::populate(&m_ec, cx, cy);
    m_q.fill(&m_ec);
    m_q.fill_sprites(sprite::layers::camping,
                     static_cast<tile::camping::compos &>(m_ec).sprites());
    m_q.fill_sprites(sprite::layers::terrain,
                     static_cast<tile::terrain::compos &>(m_ec).sprites());
    m_q.fill_hud(static_cast<hud::compos &>(m_ec).sprites());
    // m_q.fill_debug(&m_ec);
  }

  void tick() {
    player::tick(&m_ec);
    animation::update_animes(&m_ec);
    movement::update_sprites(&m_ec);
    gauge::run_drains(&m_ec);
    hud::update_batteries(&m_ec);
    looting::mark_lootable(&m_ec);
    repaint();
    m_ec.reset_watch();
  }

  void window_changed() {
    auto [gw, gh] = m_q.hud_grid_size();
    hud::update_layout(&m_ec, gw, gh);
  }

  void dump_stats() {
    silog::log(silog::debug, "Max entities: %d", m_ec.e().max_elements());
    silog::log(silog::debug, "Animations: %d", m_ec.animations().size());
    silog::log(silog::debug, "Gauges: %d", m_ec.gauges.size());
    silog::log(silog::debug, "Movements: %d", m_ec.movements().size());
  }

public:
  void process_event(const casein::event &e) {
    m_q.process_event(e);
    m_ec.input().process_event(e);

    switch (e.type()) {
    case casein::CREATE_WINDOW:
      setup();
      break;
    case casein::REPAINT:
      tick();
      break;
    case casein::RESIZE_WINDOW:
      window_changed();
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

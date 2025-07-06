#pragma leco app
export module fod;

import animation;
import debug;
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
import vinyl;

struct ec : debug::compos,
            hud::compos,
            looting::compos,
            tile::terrain::compos {};

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
    debug::show_collisions_around(&m_ec, cx, cy, 8);
    m_q.fill(&m_ec);
  }

  void tick() {
    // TODO: move most of these out of the on_frame code 
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
  game() {
    setup();
  }
  ~game() {
    dump_stats();
  }

  void on_resize() {
    m_q.on_resize();
    window_changed();
  }
  void on_frame() {
    m_q.on_frame();
    tick();
  }
} * g_g;

static struct app_init {
  app_init() {
    using namespace vinyl;
    on(START,  [] { g_g = new game{}; });
    on(RESIZE, [] { g_g->on_resize(); });
    on(FRAME,  [] { g_g->on_frame();  });
    on(STOP,   [] { delete g_g;       });
  }
} i;

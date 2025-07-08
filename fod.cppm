#pragma leco app
export module fod;

import animation;
import hud;
import gauge;
import input;
import jute;
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

struct ec : hud::compos,
            looting::compos,
            tile::terrain::compos {};

void load_prefab(ec * ec, jute::view name, int dx, int dy) {
  try {
    // TODO: cache? instance?
    auto o0 = prefabs::load(name);
    o0.for_each([&](float x, float y, const auto & def) {
      sprite::c s {
        .uv { def.tile.x, def.tile.y, def.tile.z, def.tile.w },
        .layer = sprite::layers::terrain,
      };
      rect r { dx + x, dy + y, def.tile.z, def.tile.w };
      auto id = sprite::add(ec, s, r);
      collision::add(ec, id,
          r.x + def.collision.x,
          r.y + def.collision.y,
          def.collision.z,
          def.collision.w);
    });
  } catch (const prefabs::error & e) {
    silog::log(silog::error, "%s:%d: %s", name.cstr().begin(), e.line_number, e.msg.begin());
  }
}

class game {
  qsu::main m_q{};
  ec m_ec{};

  void setup() {
    // TODO: speed of character depends on FPS
 
    load_prefab(&m_ec, "prefabs-ocean-0.txt", -16, -16);
    load_prefab(&m_ec, "prefabs-ocean-0.txt",   0, -16);
    load_prefab(&m_ec, "prefabs-ocean-0.txt",  16, -16);
    load_prefab(&m_ec, "prefabs-ocean-0.txt", -16,   0);
    load_prefab(&m_ec, "prefabs-ocean-0.txt",  16,   0);
    load_prefab(&m_ec, "prefabs-ocean-0.txt", -16,  16);
    load_prefab(&m_ec, "prefabs-ocean-0.txt",   0,  16);
    load_prefab(&m_ec, "prefabs-ocean-0.txt",  16,  16);

    load_prefab(&m_ec, "prefabs-island-0.txt", 0, 0);

    looting::add_backpack(&m_ec, tile::camping::backpack_a, 9, 7);
    looting::add_backpack(&m_ec, tile::camping::backpack_b, 10, 7);
    looting::add_backpack(&m_ec, tile::camping::backpack_c, 11, 7);

    player::add_entity(&m_ec);
    hud::add_entities(&m_ec);

    m_q.set_grid(32, 32);
    repaint();
    m_ec.reset_watch();
  }

  void repaint() {
    misc::follow_player(&m_q, &m_ec);
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

struct app_init {
  app_init() {
    using namespace vinyl;
    on(START,  [] { g_g = new game{}; });
    on(RESIZE, [] { g_g->on_resize(); });
    on(FRAME,  [] { g_g->on_frame();  });
    on(STOP,   [] { delete g_g;       });
  }
} i;

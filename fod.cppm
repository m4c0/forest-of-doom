#pragma leco app
export module fod;

import animation;
import backpack;
import dotz;
import fox;
import hud;
import gauge;
import input;
import jute;
import looting;
import movement;
import player;
import prefabs;
import qsu;
import silog;
import sitime;
import tile;
import v;

struct ec : hud::compos, looting::compos {} g_ec;

void load_prefab(fox::memiter * m, jute::view name, int dx, int dy) {
  try {
    auto o0 = prefabs::load(name);
    o0->for_each([&](float x, float y, const auto & def) {
      *m += fox::sprite {
        .pos   { dx + x, dy + y },
        .uv    = def.tile.uv,
        .size  = def.tile.size,
        .texid = def.tile.texid,
      };
    });
    o0->for_each([&](float x, float y, const auto & def) {
      if (dotz::length(def.collision) > 0) {
        auto id = g_ec.e().alloc();
        collision::add(&g_ec, id,
            dx + x + def.collision.x,
            dy + y + def.collision.y,
            def.collision.z,
            def.collision.w);

      }

      if (*def.behaviour == "backpack") backpack::add(fox::sprite {
        .pos   { dx + x, dy + y },
        .uv    = def.entity.uv,
        .size  = def.entity.size,
        .texid = def.entity.texid,
      });
    });
  } catch (const prefabs::error & e) {
    silog::log(silog::error, "%s:%d: %s", name.cstr().begin(), e.line_number, e.msg.begin());
  }
}

fox::main * g_fox {};
qsu::main * g_q {};

static void repaint() {
  auto [x, y] = player::center(&g_ec);
  g_q->center_at(x, y);
  g_q->fill(&g_ec);
}

static void on_start() {
  g_q = new qsu::main {};
  g_fox = new fox::main {};
  
  g_fox->load(fox::layers::floor, [](auto * m) {
    load_prefab(m, "prefabs-ocean-0.txt", -16, -16);
    load_prefab(m, "prefabs-ocean-0.txt",   0, -16);
    load_prefab(m, "prefabs-ocean-0.txt",  16, -16);
    load_prefab(m, "prefabs-ocean-0.txt", -16,   0);
    load_prefab(m, "prefabs-ocean-0.txt",  16,   0);
    load_prefab(m, "prefabs-ocean-0.txt", -16,  16);
    load_prefab(m, "prefabs-ocean-0.txt",   0,  16);
    load_prefab(m, "prefabs-ocean-0.txt",  16,  16);
  
    load_prefab(m, "prefabs-island-0.txt", 0, 0);
  });
  g_fox->load(fox::layers::entities, [](auto * m) {
    backpack::load(m);
  });

  player::add_entity(&g_ec);
  hud::add_entities(&g_ec);

  g_q->set_grid(32, 32);
  repaint();
  g_ec.reset_watch();
}

static void on_frame() {
  // TODO: speed of character depends on FPS
  // TODO: move most of these out of the on_frame code 
  player::tick(&g_ec);
  animation::update_animes(&g_ec);
  movement::update_sprites(&g_ec);
  gauge::run_drains(&g_ec);
  hud::update_batteries(&g_ec);
  looting::mark_lootable(&g_ec);
  repaint();
  g_ec.reset_watch();
  g_fox->on_frame(16, player::center(&g_ec));
  g_q->on_frame();
}

static void on_resize() {
  auto [gw, gh] = g_q->hud_grid_size();
  hud::update_layout(&g_ec, gw, gh);
}

static void on_stop() {
  delete g_q;
  delete g_fox;
}

const int i = [] {
  v::on_start  = on_start;
  v::on_resize = on_resize;
  v::on_frame  = on_frame;
  v::on_stop   = on_stop;
  return 0;
}();

#pragma leco app
export module fod;

import backpack;
import collision;
import dotz;
import fox;
import hud;
import input;
import jute;
import player;
import prefabs;
import silog;
import sitime;
import v;

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
      if (dotz::length(def.collision.zw()) > 0) {
        auto aa = def.collision.xy() + dotz::vec2 { dx + x, dy + y };
        auto bb = aa + def.collision.zw();
        collision::bodies().add_aabb(aa, bb, 'body', 1);
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
sitime::stopwatch g_timer {};

static void on_start() {
  g_timer = {};
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
}

static void on_frame() {
  // TODO: speed of character depends on FPS
  // TODO: move most of these out of the on_frame code 
  float ms = g_timer.millis();
  player::tick(ms);
  g_timer = {};

  g_fox->load(fox::layers::entities, [](auto * m) {
    auto p = player::center();
    backpack::load(p, m);
  });
  g_fox->load(fox::layers::player, [](auto * m) {
    player::load(m);
  });
  g_fox->load_ui([](auto * m) {
    hud::load_ui(m, g_fox->aspect() * 8);
  });
  g_fox->on_frame(16, 16, player::center());
}

static void on_resize() {
}

static void on_stop() {
  delete g_fox;
}

const int i = [] {
  v::on_start  = on_start;
  v::on_resize = on_resize;
  v::on_frame  = on_frame;
  v::on_stop   = on_stop;
  input::setup();
  return 0;
}();

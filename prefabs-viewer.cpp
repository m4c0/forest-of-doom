#pragma leco app

import dotz;
import fox;
import jute;
import prefabs;
import silog;
import v;

fox::main * g_fox;

const int i = [] {
  v::on_start = [] {
    g_fox = new fox::main {};

    jute::view name = "prefabs-plains-0.lsp";
    auto o0 = prefabs::load(name);
    g_fox->load(fox::layers::floor, [&](auto * m) {
      o0->for_each([&](float x, float y, const auto & def) {
        *m += fox::sprite {
          .pos   { x, y },
          .uv    = def.tile.uv,
          .size  = def.tile.size,
          .texid = static_cast<fox::texids>(def.tile.texid),
        };
      });
    });
    g_fox->load(fox::layers::entities, [&](auto * m) {
      o0->for_each([&](float x, float y, const auto & def) {
        if (dotz::length(def.entity.size))
          *m += fox::sprite {
            .pos   { x, y },
            .uv    = def.entity.uv,
            .size  = def.entity.size,
            .texid = static_cast<fox::texids>(def.entity.texid),
          };
      });
    });
    g_fox->load(fox::layers::player, [&](auto * m) {
      o0->for_each([&](float x, float y, const auto & def) {
        if (dotz::length(def.collision))
          *m += fox::sprite {
            .pos   { x + def.collision.x, y + def.collision.y },
            .uv    { 2, 2 },
            .size  = def.collision.zw(),
            .texid = fox::texids::ui_paper,
          };
      });
    });
  };
  v::on_frame = [] {
    g_fox->on_frame(16, 32, 8);
  };
  v::on_stop = [] {
    delete g_fox;
  };
  return 0;
}();

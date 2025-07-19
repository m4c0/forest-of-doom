#pragma leco app

import casein;
import dotz;
import fox;
import silog;
import voo;
import v;

fox::main * g_fox;
dotz::ivec2 g_cursor;
unsigned g_texid {};

static void load() {
  g_fox->load(fox::layers::floor, [](auto * m) {
    *m += {
      .pos { 0, 0 },
      .uv { 0, 0 },
      .size { 512, 512 },
      .texid = static_cast<fox::texids>(g_texid),
    };
  });
  g_fox->load(fox::layers::entities, [](auto * m) {
    for (dotz::ivec2 p = -1; p.y <= 1; p.y++) {
      for (p.x = -1; p.x <= 1; p.x++) {
        if (p.x == 0 && p.y == 0) continue;
        *m += {
          .pos = g_cursor + p,
          .uv = p + 1,
          .size { 1, 1 },
          .texid = fox::texids::ui_style,
        };
      }
    }
  });
}
static constexpr const auto cursor(int dx, int dy) {
  return [=] {
    g_cursor = g_cursor + dotz::ivec2 { dx, dy };
    if (g_cursor.x < 0) g_cursor.x = 0;
    if (g_cursor.y < 0) g_cursor.y = 0;
    load();
  };
}

const int i = [] {
  v::on_start = [] {
    g_fox = new fox::main {};
    load();
  };
  v::on_frame = [] {
    g_fox->on_frame(16, 32, g_cursor);
  };
  v::on_resize = [] {};
  v::on_stop = [] {
    delete g_fox;
  };

  using namespace casein;
  handle(KEY_DOWN, K_LEFT,  cursor(-1, 0));
  handle(KEY_DOWN, K_RIGHT, cursor(+1, 0));
  handle(KEY_DOWN, K_UP,    cursor(0, -1));
  handle(KEY_DOWN, K_DOWN,  cursor(0, +1));

  handle(KEY_DOWN, K_SPACE, [] {
    g_texid++;
    if (g_texid == static_cast<unsigned>(fox::texids::max)) g_texid = 0;
    load();
  });

  handle(KEY_DOWN, K_ENTER, [] {
    silog::log(silog::info, "> tile %d %d 1 1", g_cursor.x, g_cursor.y);
  });

  return 0;
}();

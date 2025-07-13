#pragma leco add_resource_dir res
#pragma leco app

import casein;
import dotz;
import fox;
import silog;
import voo;
import v;

fox::main * g_fox;
dotz::ivec2 g_cursor;

static void load() {
  g_fox->load([](auto * m) {
    *m += {
      .pos = g_cursor,
      .uv = g_cursor,
      .size { 1, 1 },
      .texid = 1,
    };
    *m += {
      .pos { 0, 0 },
      .uv { 0, 0 },
      .size { 64, 64 },
      .alpha = 0.7,
      .texid = 1,
    };
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
    g_fox->on_frame(16, g_cursor);
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

  handle(KEY_DOWN, K_ENTER, [] {
    silog::log(silog::info, "> tile %d %d 1 1", g_cursor.x, g_cursor.y);
  });

  return 0;
}();

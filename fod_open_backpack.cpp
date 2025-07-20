module fod;
import backpack;
import dotz;
import fox;
import input;
import player;

static constexpr const auto inv_w = 8U;
static constexpr const auto inv_h = 8U;

static dotz::ivec2 g_cursor {};
static dotz::ivec2 g_sel {-1};
static dotz::vec2 g_inventory[inv_w * inv_h] {
  {  8, 0 },
  { 11, 3 },
  { 12, 3 },
  { 12, 2 },
};

static auto & inv(dotz::ivec2 p) {
  return g_inventory[p.y * inv_w + p.x];
}

static void on_frame(float ms) {
  fox::g->load_ui([](auto * m) {
    const dotz::vec2 size { inv_w, inv_h };
    const dotz::vec2 tl = -(size + 3) / 2.0;
    const dotz::vec2 gtl = 0.3f;
    const dotz::vec2 csz = 1.2f;

    const auto sp = [&](dotz::vec2 pos, dotz::vec2 uv, dotz::vec2 size = 1) {
      *m += { .pos = tl + pos, .uv = uv, .size = size, .texid = fox::texids::ui_paper };
    };
    const auto box = [&] {
      sp({ 0, 0 }, { 1, 1 });
      for (auto x = 0; x < size.x; x++) sp({ x + 1, 0 }, { 2, 1 });
      sp({ size.x + 1, 0.f }, { 3, 1 });

      for (auto y = 0; y < size.y; y++) {
        sp({ 0, y + 1 }, { 1, 2 });
        for (auto x = 0; x < size.x; x++) sp({ x + 1, y + 1 }, { 2, 2 });
        sp({ size.x + 1, y + 1.f }, { 3, 2 });
      }

      sp({ 0.0f, size.y + 1 }, { 1, 3 });
      for (auto x = 0; x < size.x; x++) sp({ x + 1.f, size.y + 1 }, { 2, 3 });
      sp({ size.x + 1, size.y + 1 }, { 3, 3 });
    };

    box();

    for (dotz::ivec2 p = 0; p.y < inv_h; p.y++) {
      for (p.x = 0; p.x < inv_w; p.x++) {
        auto i = inv(p);

        dotz::vec2 uv {};
        if (g_sel.x < 0) {
          uv = i.x || i.y ? dotz::vec2 { 5, 7 } : dotz::vec2 { 5, 9 };
        } else {
          uv = (p == g_sel) ? dotz::vec2 { 5, 8 } : dotz::vec2 { 5, 7 };
        }
        sp(p * csz + gtl, uv);
      }
    }

    for (dotz::ivec2 p = 0; p.y < inv_h; p.y++) {
      for (p.x = 0; p.x < inv_w; p.x++) {
        auto i = inv(p);
        if (!i.x && !i.y) continue;
        *m += {
          .pos   = tl + p * csz + gtl,
          .uv    = i,
          .size  = 1,
          .texid = fox::texids::ui_style,
        };
      }
    }

    sp(g_cursor * csz + gtl, { 15, 4 });
  });
  fox::g->on_frame(16, 16, player::center());
}

static void on_action() {
  if (g_sel == -1) {
    auto i = inv(g_cursor);
    if (!i.x && !i.y) return;
    g_sel = g_cursor;
    return;
  }

  auto tmp = inv(g_sel);
  inv(g_sel) = inv(g_cursor);
  inv(g_cursor) = tmp;

  g_sel = -1;
}

static constexpr auto cursor(int dx, int dy) {
  return [=] {
    g_cursor = dotz::clamp(g_cursor + dotz::ivec2 { dx, dy }, {0}, {7});
  };
}

void fod::open_backpack() {
  fod::on_frame = ::on_frame;
  g_cursor = {};
  g_sel = -1;

  using namespace input;
  reset();
  on_key_down(keys::ACTION, on_action);
  on_key_down(keys::CANCEL, fod::poc);
  on_key_down(keys::MOVE_DOWN,  cursor( 0,  1));
  on_key_down(keys::MOVE_UP,    cursor( 0, -1));
  on_key_down(keys::MOVE_LEFT,  cursor(-1,  0));
  on_key_down(keys::MOVE_RIGHT, cursor( 1,  0));
}

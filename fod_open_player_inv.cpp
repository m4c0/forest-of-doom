module fod;
import dotz;
import fox;
import fui;
import input;
import player;

static dotz::ivec2 g_cursor {};
static dotz::ivec2 g_sel {};
static enum {
  inv_player,
  inv_drop,
  inv_garbage,
} g_cur_inv;

static auto player_inv() {
  return fui::inv { &player::inv::inv(), {}, g_sel };
}
static auto drop_inv() {
  return fui::slot { { -1, 2 }, { 40, 19 } };
}
static auto garbage_inv() {
  return fui::slot { { 0, 2 }, { 42, 18 } };
}

static void on_frame(float ms) {
  fox::g->load_ui([](auto * m) {
    player_inv().load(m, g_cursor);
    drop_inv().load(m, g_cur_inv == inv_drop);
    garbage_inv().load(m, g_cur_inv == inv_garbage);
  });
  fox::g->on_frame(16, 16, player::center());
}

static void on_action() {
  if (g_sel == -1) {
    if (g_cur_inv == inv_drop) return;
    if (g_cur_inv == inv_garbage) return;

    auto item = player_inv().at(g_cursor);
    if (!item) return;
    auto i = item->sprite;
    if (!i.x && !i.y) return;

    g_sel = g_cursor;
    return;
  }

  auto sp = player_inv().at(g_sel);
  auto cp = player_inv().at(g_cursor);
  if (sp && cp) {
    auto tmp = *sp;
    *sp = *cp;
    *cp = tmp;
  }

  g_sel = -1;
}

static constexpr auto move_cursor(dotz::ivec2 d) {
  return [=] {
    switch (g_cur_inv) {
      case inv_drop:
        if (d.x == 1) g_cur_inv = inv_garbage;
        else if (d.y == -1) {
          g_cursor = { 3, fui::inv::h - 1 };
          g_cur_inv = inv_player;
        }
        break;
      case inv_garbage:
        if (d.x == -1) g_cur_inv = inv_drop;
        else if (d.y == -1) {
          g_cursor = { 4, fui::inv::h - 1 };
          g_cur_inv = inv_player;
        }
        break;
      case inv_player:
        auto c = g_cursor + d;
        if (c.x < 0) return;
        if (c.x >= fui::inv::w) return;
        if (c.y < 0) return;
        if (c.y >= fui::inv::h) {
          g_cur_inv = g_cursor.x <= 3 ? inv_drop : inv_garbage;
          g_cursor = -1;
          return;
        }
        g_cursor = c;
    }
  };
}

void fod::open_player_inv() {
  g_cur_inv = inv_player;
  g_cursor = {};
  g_sel = -1;

  fod::on_frame = ::on_frame;

  using namespace input;
  reset();
  on_key_down(keys::ACTION, on_action);
  on_key_down(keys::CANCEL, fod::poc); // TODO: discard selection or quit
  on_key_down(keys::MOVE_DOWN,  move_cursor({ 0,  1}));
  on_key_down(keys::MOVE_UP,    move_cursor({ 0, -1}));
  on_key_down(keys::MOVE_LEFT,  move_cursor({-1,  0}));
  on_key_down(keys::MOVE_RIGHT, move_cursor({ 1,  0}));
}

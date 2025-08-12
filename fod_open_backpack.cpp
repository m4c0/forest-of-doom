module fod;
import dotz;
import drops;
import fox;
import fui;
import input;
import loots;
import player;
import silog;

static hai::array<loots::item> * g_inv;
static enum inv_e {
  inv_backpack,
  inv_player,
  inv_drop,
  inv_garbage,
} g_cur_inv, g_sel_inv;
static dotz::ivec2 g_cursor {};
static dotz::ivec2 g_sel {};
static loots::item g_drop {};
static loots::item g_garbage {};

static auto cursor(inv_e inv) {
  return inv == g_cur_inv ? g_cursor : dotz::ivec2 { -1 };
}
static auto sel(inv_e inv) {
  return inv == g_sel_inv ? g_sel : dotz::ivec2 { -1 };
}

static auto open_inv() {
  return fui::inv { g_inv, { 0, -2 }, sel(inv_backpack) };
}
static auto player_inv() {
  return fui::inv { &player::inv::inv(), { 0, 2 }, sel(inv_player) };
}
static auto drop_inv() {
  return fui::slot { { -1, 4 }, { 40, 19 }, &g_drop, g_cur_inv == inv_drop, g_sel_inv == inv_drop };
}
static auto garbage_inv() {
  return fui::slot { { 0, 4 }, { 42, 18 }, &g_garbage, g_cur_inv == inv_garbage, g_sel_inv == inv_garbage };
}

static loots::item * at(inv_e i, dotz::ivec2 p) {
  switch (i) {
    case inv_backpack: return open_inv().at(p);
    case inv_player:   return player_inv().at(p);
    case inv_drop:     return &g_drop;
    case inv_garbage:  return &g_garbage;
    default: silog::die("unreachable: invalid inventory");
  }
}

static void drop() {
  if (!g_drop) return;
  drops::add({
    .sprite {
      .pos   = player::center(),
      .uv    = g_drop.sprite,
      .size  = 1,
      .texid = fox::texids::ui_style,
    },
  });
  g_drop = {};
}

static void on_frame(float ms) {
  fox::g->load_ui([](auto * m) {
    open_inv().load(m, cursor(inv_backpack));
    player_inv().load(m, cursor(inv_player));
    drop_inv().load(m);
    garbage_inv().load(m);
  });
  fox::g->on_frame(16, 16, player::center());
}

static void on_action() {
  if (g_sel == -1) {
    auto item = at(g_cur_inv, g_cursor);
    if (!item || !*item) return;

    g_sel = g_cursor;
    g_sel_inv = g_cur_inv;
    return;
  }

  if (g_cur_inv == inv_garbage) g_garbage = {};
  if (g_cur_inv == inv_drop) drop();

  auto sp = at(g_sel_inv, g_sel);
  auto cp = at(g_cur_inv, g_cursor);
  if (sp && cp) {
    auto tmp = *sp;
    *sp = *cp;
    *cp = tmp;
  }

  g_sel = -1;
  g_sel_inv = {};
}

static constexpr auto move_cursor(dotz::ivec2 d) {
  return [=] {
    if (g_cur_inv == inv_drop) {
      if (d.x == 1) g_cur_inv = inv_garbage;
      else if (d.y == -1) {
        g_cursor = { 3, fui::inv::h - 1 };
        g_cur_inv = inv_player;
      }
      return;
    }
    if (g_cur_inv == inv_garbage) {
      if (d.x == -1) g_cur_inv = inv_drop;
      else if (d.y == -1) {
        g_cursor = { 4, fui::inv::h - 1 };
        g_cur_inv = inv_player;
      }
      return;
    }

    auto c = g_cursor + d;
    if (c.x < 0) return;
    if (c.x >= fui::inv::w) return;
    if (c.y < 0) {
      if (g_cur_inv == inv_backpack) return;
      g_cur_inv = inv_backpack;
      c.y += fui::inv::h;
    } else if (c.y >= fui::inv::h) {
      if (g_cur_inv == inv_player) {
        g_cur_inv = g_cursor.x <= 3 ? inv_drop : inv_garbage;
        return;
      }
      g_cur_inv = inv_player;
      c.y -= fui::inv::h;
    }
    g_cursor = c;
  };
}

static void on_cancel() {
  if (g_sel != -1) {
    g_sel = -1;
    g_sel_inv = {};
    return;
  }

  drop();
  g_drop = g_garbage = {};
  fod::poc();
}

void fod::open_backpack(hai::array<loots::item> * inv) {
  g_inv = inv;

  g_cursor = {};
  g_cur_inv = {};
  g_sel = -1;
  g_sel_inv = {};

  fod::on_frame = ::on_frame;

  using namespace input;
  reset();
  on_key_down(keys::ACTION, on_action);
  on_key_down(keys::CANCEL, on_cancel);
  on_key_down(keys::MOVE_DOWN,  move_cursor({ 0,  1}));
  on_key_down(keys::MOVE_UP,    move_cursor({ 0, -1}));
  on_key_down(keys::MOVE_LEFT,  move_cursor({-1,  0}));
  on_key_down(keys::MOVE_RIGHT, move_cursor({ 1,  0}));
}

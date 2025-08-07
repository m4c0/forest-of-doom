module fod;
import dotz;
import fox;
import fui;
import input;
import loots;
import player;
import silog;

static hai::array<loots::item> * g_inv;
static enum inv_e {
  inv_backpack = 0,
  inv_player,
  inv_count,
} g_cur_inv, g_sel_inv;
static dotz::ivec2 g_cursor {};
static dotz::ivec2 g_sel {};

static auto cursor(inv_e inv) {
  return inv == g_cur_inv ? g_cursor : dotz::ivec2 { -1 };
}
static auto sel(inv_e inv) {
  return inv == g_sel_inv ? g_sel : dotz::ivec2 { -1 };
}
static auto inv(inv_e inv) {
  switch (inv) {
    case inv_backpack: return g_inv;
    case inv_player: return &player::inv::inv();
    default: silog::die("unreachable: invalid inventory");
  }
}

static loots::item * at(inv_e i, dotz::ivec2 p) {
  auto idx = fui::inv::idx(p);
  auto ii = inv(i);
  if (idx >= ii->size()) return nullptr;
  return &(*ii)[idx];
}

static auto open_inv() {
  return fui::inv { g_inv, { 0, -2 }, sel(inv_backpack) };
}
static auto player_inv() {
  return fui::inv { &player::inv::inv(), { 0, 2 }, sel(inv_player) };
}

static void on_frame(float ms) {
  fox::g->load_ui([](auto * m) {
    open_inv().load(m, cursor(inv_backpack));
    player_inv().load(m, cursor(inv_player));
  });
  fox::g->on_frame(16, 16, player::center());
}

static void on_action() {
  if (g_sel == -1) {
    auto item = at(g_cur_inv, g_cursor);
    if (!item) return;

    auto i = item->sprite;
    if (!i.x && !i.y) return;
    g_sel = g_cursor;
    g_sel_inv = g_cur_inv;
    return;
  }

  auto sp = at(g_sel_inv, g_sel);
  auto cp = at(g_cur_inv, g_cursor);
  if (sp && cp) {
    auto tmp = *sp;
    *sp = *cp;
    *cp = tmp;
  }

  g_sel = -1;
}

static constexpr auto move_cursor(dotz::ivec2 d) {
  return [=] {
    auto c = g_cursor + d;
    if (c.x < 0) return;
    if (c.x >= fui::inv::w) return;
    if (c.y < 0) {
      if (g_cur_inv == inv_backpack) return;
      g_cur_inv = inv_backpack;
      c.y += fui::inv::h;
    } else if (c.y >= fui::inv::h) {
      if (g_cur_inv == inv_player) return;
      g_cur_inv = inv_player;
      c.y -= fui::inv::h;
    }
    g_cursor = c;
  };
}

void fod::open_backpack(hai::array<loots::item> * inv) {
  g_inv = inv;

  fod::on_frame = ::on_frame;
  g_cursor = {};
  g_cur_inv = inv_backpack;
  g_sel = -1;
  g_sel_inv = inv_backpack;

  using namespace input;
  reset();
  on_key_down(keys::ACTION, on_action);
  on_key_down(keys::CANCEL, fod::poc); // TODO: discard selection or quit
  on_key_down(keys::MOVE_DOWN,  move_cursor({ 0,  1}));
  on_key_down(keys::MOVE_UP,    move_cursor({ 0, -1}));
  on_key_down(keys::MOVE_LEFT,  move_cursor({-1,  0}));
  on_key_down(keys::MOVE_RIGHT, move_cursor({ 1,  0}));
}

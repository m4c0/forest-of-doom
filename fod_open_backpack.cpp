module fod;
import dotz;
import fox;
import input;
import loots;
import player;

namespace fui {
  class inv {
    static constexpr const auto w = 8;
    static constexpr const auto h = 3;
    static constexpr const dotz::vec2 size { w, h };
    static constexpr const dotz::vec2 tl = -(size + 2) / 2.0;
    static constexpr const dotz::vec2 gtl = 0.5f;
    static constexpr const dotz::vec2 csz = 1.0f;

    dotz::ivec2 m_cursor {};
    dotz::ivec2 m_sel {-1};
    hai::array<loots::item> * m_inventory;
    dotz::vec2 m_pos;

    constexpr auto idx(dotz::ivec2 p) {
      return p.y * w + p.x;
    }
    constexpr loots::item & at(dotz::ivec2 p) {
      return (*m_inventory)[idx(p)];
    }

    // Sprite aligned to box.
    void sb(auto * m, dotz::vec2 pos, dotz::vec2 uv) {
      *m += { .pos = m_pos + tl + pos, .uv = uv, .size = 1, .texid = fox::texids::ui_paper };
    }
    // Sprite aligned to inventory.
    void sp(auto * m, dotz::vec2 pos, dotz::vec2 uv, fox::texids texid) {
      *m += { .pos = m_pos + tl + pos * csz + gtl, .uv = uv, .size = 1, .texid = texid };
    }

    void load_box(auto * m) {
      sb(m, { 0, 0 }, { 1, 1 });
      for (auto x = 1; x < size.x; x++) sb(m, { x, 0 }, { 2, 1 });
      sb(m, { w, 0 }, { 3, 1 });

      for (auto y = 1; y < size.y; y++) {
        sb(m, { 0, y }, { 1, 2 });
        for (auto x = 1; x < size.x; x++) sb(m, { x, y }, { 2, 2 });
        sb(m, { w, y }, { 3, 2 });
      }

      sb(m, { 0.0f, size.y }, { 1, 3 });
      for (auto x = 0; x < size.x; x++) sb(m, { x, h }, { 2, 3 });
      sb(m, size, { 3, 3 });
    }
    void load_slots(auto * m) {
      for (dotz::ivec2 p = 0; p.y < h; p.y++) {
        for (p.x = 0; p.x < w; p.x++) {
          if (idx(p) >= m_inventory->size()) {
            sp(m, p, { 5, 9 }, fox::texids::ui_paper);
            continue;
          }

          auto i = at(p).sprite;

          dotz::vec2 uv {};
          if (m_sel.x < 0) {
            uv = i.x || i.y ? dotz::vec2 { 5, 7 } : dotz::vec2 { 5, 9 };
          } else {
            uv = (p == m_sel) ? dotz::vec2 { 5, 8 } : dotz::vec2 { 5, 7 };
          }
          sp(m, p, uv, fox::texids::ui_paper);
        }
      }
    }
    void load_inv(auto * m) {
      for (dotz::ivec2 p = 0; p.y < h; p.y++) {
        for (p.x = 0; p.x < w; p.x++) {
          if (idx(p) >= m_inventory->size()) continue;

          auto i = at(p).sprite;
          // TODO: merge this loop with previous?
          if (!i.x && !i.y) continue;
          sp(m, p, i, fox::texids::ui_style);
        }
      }
    }

  public:
    constexpr inv() = default;

    constexpr inv(hai::array<loots::item> * i, dotz::vec2 p) :
      m_inventory { i }
    , m_pos { p }
    {}

    void load(auto * m) {
      load_box(m);
      load_slots(m);
      load_inv(m);

      sp(m, m_cursor, { 15, 4 }, fox::texids::ui_paper);
    }
  };
}

static fui::inv g_inv {};
static fui::inv g_p_inv {};

static void on_frame(float ms) {
  fox::g->load_ui([](auto * m) {
    g_inv.load(m);
    g_p_inv.load(m);
  });
  fox::g->on_frame(16, 16, player::center());
}

static void on_action() {
  //if (g_sel == -1) {
  //  auto i = inv(g_cursor).sprite;
  //  if (!i.x && !i.y) return;
  //  g_sel = g_cursor;
  //  return;
  //}

  //auto tmp = inv(g_sel);
  //inv(g_sel) = inv(g_cursor);
  //inv(g_cursor) = tmp;

  //g_sel = -1;
}

static constexpr auto cursor(int dx, int dy) {
  return [=] {
    //g_cursor = dotz::clamp(g_cursor + dotz::ivec2 { dx, dy }, {0}, {7});
  };
}

void fod::open_backpack(hai::array<loots::item> * inv) {
  g_inv = fui::inv { inv, { 0, -2 } };
  g_p_inv = fui::inv { &player::inv::inv(), { 0, 2 } };

  fod::on_frame = ::on_frame;
  //g_cursor = {};
  //g_sel = -1;

  using namespace input;
  reset();
  on_key_down(keys::ACTION, on_action);
  on_key_down(keys::CANCEL, fod::poc);
  on_key_down(keys::MOVE_DOWN,  cursor( 0,  1));
  on_key_down(keys::MOVE_UP,    cursor( 0, -1));
  on_key_down(keys::MOVE_LEFT,  cursor(-1,  0));
  on_key_down(keys::MOVE_RIGHT, cursor( 1,  0));
}

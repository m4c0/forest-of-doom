export module fui;
import dotz;
import hai;
import fox;
import loots;

namespace fui {
  export class inv {
  public:
    static constexpr const auto w = 8;
    static constexpr const auto h = 3;

    static constexpr auto idx(dotz::ivec2 p) {
      return p.y * w + p.x;
    }

  private:
    static constexpr const dotz::vec2 size { w, h };
    static constexpr const dotz::vec2 tl = -(size + 2) / 2.0;
    static constexpr const dotz::vec2 gtl = 0.5f;
    static constexpr const dotz::vec2 csz = 1.0f;

    hai::array<loots::item> * m_inventory;
    dotz::vec2 m_pos;
    dotz::ivec2 m_sel;

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

          if (i.x || i.y) sp(m, p, i, fox::texids::ui_style);
        }
      }
    }

  public:
    constexpr inv() = default;

    constexpr inv(hai::array<loots::item> * i, dotz::vec2 p, dotz::ivec2 sel) :
      m_inventory { i }
    , m_pos { p }
    , m_sel { sel }
    {}

    void load(auto * m, dotz::ivec2 cursor) {
      load_box(m);
      load_slots(m);

      if (cursor.x >= 0) sp(m, cursor, { 15, 4 }, fox::texids::ui_paper);
    }
  };
}

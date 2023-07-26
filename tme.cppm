export module main;

import casein;
import collision;
import cursor;
import hai;
import jute;
import missingno;
import pog;
import prefabs;
import qsu;
import silog;
import sprite;
import tile;
import tilemap;
import yoyo_libc;

template <typename C> class palette {
  hai::varray<C> m_tiles;
  unsigned m_index{};

public:
  constexpr palette(auto... n) : m_tiles{sizeof...(n)} {
    (m_tiles.push_back(n), ...);
  }

  [[nodiscard]] constexpr auto &get() noexcept { return m_tiles[m_index]; }

  void operator++() noexcept { m_index = (m_index + 1) % m_tiles.size(); }
  void operator--() noexcept {
    m_index = (m_index + m_tiles.size() - 1) % m_tiles.size();
  }
};
template <typename Tp> palette(Tp, auto...) -> palette<Tp>;

static void fail(const char *msg) {
  silog::log(silog::error, "Error: %s", msg);
  throw 0;
}

namespace camping_set {
static constexpr const auto tname = "tile::camping";
static constexpr const auto tfill = sprite::layers::camping;
namespace t = tile::camping;

static auto pals() { return palette<palette<tile::c_t>>{}; }
} // namespace camping_set

namespace terrain_set {
static constexpr const auto tname = "tile::terrain";
static constexpr const auto tfill = sprite::layers::terrain;
namespace t = tile::terrain;

static auto pals() {
  return palette{
      palette{t::island_tl, t::island_t, t::island_tr, t::island_r,
              t::island_br, t::island_b, t::island_bl, t::island_l},
      palette{t::lake_tl, t::island_b, t::lake_tr, t::island_l, t::lake_br,
              t::island_t, t::lake_bl, t::island_r},
      palette{t::grass_tl, t::grass_t, t::grass_tr, t::grass_r, t::grass_br,
              t::grass_b, t::grass_bl, t::grass_l},
      palette{t::dirt_tl, t::grass_b, t::dirt_tr, t::grass_l, t::dirt_br,
              t::grass_t, t::dirt_bl, t::grass_r},
      palette{t::sidewalk_tl, t::sidewalk_t, t::sidewalk_tr, t::sidewalk_r,
              t::sidewalk_br, t::sidewalk_b, t::sidewalk_bl, t::sidewalk_l},
      palette{t::sidewalkr_tl, t::sidewalk_b, t::sidewalkr_tr, t::sidewalk_l,
              t::sidewalkr_br, t::sidewalk_t, t::sidewalkr_bl, t::sidewalk_r},
      palette{t::grass_0, t::grass_1, t::water, t::dirt_0, t::cobble_0,
              t::cobble_1, t::cobble_2, t::cobble_3, t::brick_0, t::brick_1,
              t::brick_2, t::brick_3},
  };
}
} // namespace terrain_set

static constexpr const auto prefab = &prefabs::ocean_0;
static constexpr const auto fname = "prefabs-ocean_0.cppm";
static constexpr const auto mname = "ocean_0";

using namespace terrain_set;

struct ec : cursor::compos, t::compos {};

class game {
  ec m_ec{};
  qsu::main *m_q{};
  tilemap::map m_undo{};
  tile::c_t m_brush{};

  decltype(pals()) m_pal = pals();

  void fill_sprites() {
    tile::populate(&m_ec, 0, 0);
    cursor::add_sprite(&m_ec, m_ec.sprites());
    m_q->fill_sprites(tfill, m_ec.sprites());
  }

  bool replace_tile(float x, float y, tile::c_t old, tile::c_t brush) {
    for (auto &[id, spr] : m_ec.sprites()) {
      auto [rx, ry, rw, rh] = spr.pos;
      if (rx != x || ry != y)
        continue;

      auto t = m_ec.tiles().get(id);
      if (t != old && old != brush)
        continue;

      m_undo.set(x, y, old);
      tile::remove_tile(&m_ec, id);
      tile::add_tile(&m_ec, brush, x, y);
      return true;
    }
    return false;
  }

  void flood_fill_at(auto x, auto y, tile::c_t old) {
    if (!replace_tile(x, y, old, m_brush))
      return;

    flood_fill_at(x + 1, y, old);
    flood_fill_at(x - 1, y, old);
    flood_fill_at(x, y + 1, old);
    flood_fill_at(x, y - 1, old);
  }

public:
  void setup(qsu::main *q) {
    m_q = q;

    q->set_grid(tilemap::width, tilemap::height);
    q->center_at(tilemap::width / 2, tilemap::height / 2);

    cursor::add_entity(&m_ec);
    prefab(&m_ec, 0, 0);
    fill_sprites();
  }

  void set_brush(tile::c_t t) {
    m_brush = t;
    cursor::update_tile(&m_ec, t);
    fill_sprites();
  }

  void next_island_brush() {
    ++m_pal;
    set_brush(m_pal.get().get());
  }
  void prev_island_brush() {
    --m_pal;
    set_brush(m_pal.get().get());
  }
  void next_lake_brush() {
    ++(m_pal.get());
    set_brush(m_pal.get().get());
  }
  void prev_lake_brush() {
    --(m_pal.get());
    set_brush(m_pal.get().get());
  }
  void next_land_brush() {}

  void mouse_moved() {
    auto [x, y] = m_q->mouse_pos();
    cursor::update_pos(&m_ec, x, y);
    fill_sprites();
  }
  void mouse_down() {
    auto [x, y] = m_q->mouse_pos();
    replace_tile(x, y, m_brush, m_brush);

    auto [tx, ty, tw, th] = tile::uv(m_brush);
    for (auto dy = 0; dy < th; dy++) {
      for (auto dx = 0; dx < tw; dx++) {
        if (dx == 0 && dy == 0)
          continue;
        replace_tile(x + dx, y + dy, 0, 0);
      }
    }
    cursor::update_pos(&m_ec, x, y);
    fill_sprites();
  }

  void flood_fill() {
    auto [x, y] = m_q->mouse_pos();
    tile::c_t old{};
    for (auto [id, spr] : m_ec.sprites()) {
      auto [rx, ry, rw, rh] = spr.pos;
      if (rx != x || ry != y)
        return;

      old = m_ec.tiles().get(id);
      if (old == m_brush)
        return;

      flood_fill_at(x, y, old);
      fill_sprites();
      return;
    }
  }

  void undo() {
    static_cast<tile::compos &>(m_ec) = {};
    m_undo.add_entities(&m_ec, 0, 0);
    fill_sprites();
  }

  void dump_map() {
    using namespace jute::literals;

    try {
      yoyo::file_writer out{fname};
      out.writef("export module prefabs:%s;\n", mname).take(fail);
      out.write("import tile;\n"_s).take(fail);
      out.write("\n"_s).take(fail);
      out.writef("using namespace %s;\n", tname).take(fail);
      out.write("\n"_s).take(fail);
      out.write("namespace prefabs {\n"_s).take(fail);
      out.writef("export void %s(compos *ec, float x, float y) {\n", mname)
          .take(fail);

      for (auto &[id, spr] : m_ec.sprites()) {
        auto t = m_ec.tiles().get(id);
        if (!t)
          continue;

        auto x = static_cast<int>(spr.pos.x);
        auto y = static_cast<int>(spr.pos.y);
        out.writef(
               "  %s::add_tile(ec, static_cast<c>(0x%08x), x + %d, y + %d);\n",
               tname, t, x, y)
            .take(fail);
      }

      out.write("}\n"_s).take(fail);
      out.write("} // namespace prefabs\n"_s).take(fail);

      silog::log(silog::info, "Source saved");
    } catch (...) {
    }
  }
};

extern "C" void casein_handle(const casein::event &e) {
  static qsu::main q{};
  static game gg{};

  static constexpr const auto kmap = [] {
    casein::key_map res{};
    res[casein::K_Q] = [](auto) { gg.prev_island_brush(); };
    res[casein::K_W] = [](auto) { gg.next_island_brush(); };
    res[casein::K_A] = [](auto) { gg.prev_lake_brush(); };
    res[casein::K_S] = [](auto) { gg.next_lake_brush(); };
    res[casein::K_E] = [](auto) { gg.next_land_brush(); };
    res[casein::K_D] = [](auto) { gg.flood_fill(); };
    res[casein::K_U] = [](auto) { gg.undo(); };
    res[casein::K_Z] = [](auto) { gg.dump_map(); };
    res[casein::K_SPACE] = [](auto) { gg.set_brush(0); };
    return res;
  }();
  static constexpr const auto map = [] {
    casein::event_map res{};
    res[casein::CREATE_WINDOW] = [](auto) { gg.setup(&q); };
    res[casein::KEY_DOWN] = [](auto e) { kmap.handle(e); };
    res[casein::MOUSE_MOVE] = [](auto e) { gg.mouse_moved(); };
    res[casein::MOUSE_DOWN] = [](auto e) { gg.mouse_down(); };
    return res;
  }();

  q.process_event(e);
  map.handle(e);
}
